#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/gapi.hpp>
#include <opencv4/opencv2/gapi/core.hpp>
#include <opencv4/opencv2/gapi/imgproc.hpp>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_009 {
// OpenCV fuzzing harness targeting G-API (Graph API) module
// Focuses on G-API module which currently has 0% coverage with 24,142 lines of uncovered code
// APIs targeted: G-API graph construction and compilation APIs, G-API kernel execution and data flow APIs
// Follows G-API workflow: create input matrices/images, build computation graph using gapi::GComputation,
// add computational kernels (e.g., blur, threshold, Sobel), compile and apply graph, validate output
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for image generation and G-API parameters
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // ===== 1. Create input matrices/images for G-API testing =====
        // Determine image dimensions and type from fuzzer input
        int image_width = fdp.ConsumeIntegralInRange<int>(64, 320);
        int image_height = fdp.ConsumeIntegralInRange<int>(64, 240);
        cv::Size image_size(image_width, image_height);
        
        // Choose image type: grayscale (CV_8UC1) or color (CV_8UC3)
        bool use_color = fdp.ConsumeBool();
        int image_type = use_color ? CV_8UC3 : CV_8UC1;
        
        // Create synthetic test image
        cv::Mat input_image(image_size, image_type);
        
        // Fill image with random pixel values
        cv::RNG rng(fdp.ConsumeIntegral<uint64_t>());
        rng.fill(input_image, cv::RNG::UNIFORM, 0, 256);
        
        // ===== 2. Build G-API computation graphs =====
        
        // Test 1: Simple graph with single operation (blur)
        {
            cv::GMat g_in;
            cv::GMat g_blur = cv::gapi::blur(g_in, cv::Size(3, 3));
            cv::GComputation blur_graph(cv::GIn(g_in), cv::GOut(g_blur));
            
            cv::Mat output_blur;
            blur_graph.apply(input_image, output_blur);
        }
        
        // Test 2: Graph with multiple operations (threshold + Sobel)
        {
            cv::GMat g_in;
            // Choose threshold type based on fuzzer input
            int thresh_type = fdp.ConsumeIntegralInRange<int>(0, 4);
            double thresh_value = fdp.ConsumeFloatingPointInRange<double>(50, 200);
            double max_value = fdp.ConsumeFloatingPointInRange<double>(200, 255);
            
            cv::GMat g_thresh = cv::gapi::threshold(g_in, thresh_value, max_value, thresh_type);
            // Add Sobel edge detection
            int dx = fdp.ConsumeIntegralInRange<int>(0, 2);
            int dy = fdp.ConsumeIntegralInRange<int>(0, 2);
            int ddepth = fdp.ConsumeIntegralInRange<int>(-1, CV_32F);
            int ksize = fdp.ConsumeIntegralInRange<int>(1, 7);
            double scale = fdp.ConsumeFloatingPointInRange<double>(0.5, 4.0);
            double delta = fdp.ConsumeFloatingPointInRange<double>(-50, 50);
            
            cv::GMat g_sobel = cv::gapi::Sobel(g_thresh, ddepth, dx, dy, ksize, scale, delta);
            
            cv::GComputation multi_op_graph(cv::GIn(g_in), cv::GOut(g_sobel));
            
            cv::Mat output_multi;
            multi_op_graph.apply(input_image, output_multi);
        }
        
        // Test 3: Graph with Gaussian blur and Canny edge detection
        {
            cv::GMat g_in;
            
            // Gaussian blur parameters
            cv::Size ksize_gauss;
            ksize_gauss.width = fdp.ConsumeIntegralInRange<int>(1, 11) * 2 + 1; // odd number
            ksize_gauss.height = fdp.ConsumeIntegralInRange<int>(1, 11) * 2 + 1;
            double sigmaX = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
            double sigmaY = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
            
            cv::GMat g_gauss = cv::gapi::gaussianBlur(g_in, ksize_gauss, sigmaX, sigmaY);
            
            // Canny edge detection parameters
            double threshold1 = fdp.ConsumeFloatingPointInRange<double>(10, 100);
            double threshold2 = fdp.ConsumeFloatingPointInRange<double>(50, 200);
            int apertureSize = fdp.ConsumeIntegralInRange<int>(3, 7);
            bool L2gradient = fdp.ConsumeBool();
            
            cv::GMat g_canny = cv::gapi::Canny(g_gauss, threshold1, threshold2, apertureSize, L2gradient);
            
            cv::GComputation canny_graph(cv::GIn(g_in), cv::GOut(g_canny));
            
            cv::Mat output_canny;
            canny_graph.apply(input_image, output_canny);
        }
        // Test 4: Graph with multiple inputs and outputs
        {
            cv::GMat g_in1, g_in2;
            
            // Create second input image
            cv::Mat input_image2(image_size, image_type);
            rng.fill(input_image2, cv::RNG::UNIFORM, 0, 256);
            
            // Add the two images
            cv::GMat g_add = cv::gapi::add(g_in1, g_in2);
            
            // Subtract them
            cv::GMat g_sub = cv::gapi::sub(g_in1, g_in2);
            // Multiply by scalar
            double scale_mul = fdp.ConsumeFloatingPointInRange<double>(0.1, 3.0);
            cv::GScalar g_scalar = cv::GScalar(std::abs(scale_mul));
            cv::GMat g_mul = cv::gapi::mulC(g_add, g_scalar);
            
            cv::GComputation multi_io_graph(cv::GIn(g_in1, g_in2), cv::GOut(g_add, g_sub, g_mul));
            
            std::vector<cv::Mat> inputs = {input_image, input_image2};
            std::vector<cv::Mat> outputs(3);
            multi_io_graph.apply(inputs, outputs);
        }
        
        // Test 5: Graph with morphological operations
        {
            cv::GMat g_in;
            
            // Create structuring element
            int morph_shape = fdp.ConsumeIntegralInRange<int>(0, 2);
            cv::MorphShapes shape = static_cast<cv::MorphShapes>(morph_shape);
            cv::Size ksize_morph(fdp.ConsumeIntegralInRange<int>(1, 5), 
                                fdp.ConsumeIntegralInRange<int>(1, 5));
            
            cv::Mat kernel = cv::getStructuringElement(shape, ksize_morph);
            
            // Erosion
            cv::GMat g_erode = cv::gapi::erode(g_in, kernel);
            
            // Dilation  
            cv::GMat g_dilate = cv::gapi::dilate(g_in, kernel);
            
            // Opening (erosion followed by dilation)
            cv::GMat g_open = cv::gapi::dilate(cv::gapi::erode(g_in, kernel), kernel);
            
            // Closing (dilation followed by erosion)
            cv::GMat g_close = cv::gapi::erode(cv::gapi::dilate(g_in, kernel), kernel);
            
            cv::GComputation morph_graph(cv::GIn(g_in), 
                                         cv::GOut(g_erode, g_dilate, g_open, g_close));
            
            std::vector<cv::Mat> morph_outputs(4);
            morph_graph.apply(input_image, morph_outputs);
        }
        
        // Test 6: Graph compilation and re-execution
        {
            cv::GMat g_in;
            
            // Choose operation based on fuzzer input
            int operation = fdp.ConsumeIntegralInRange<int>(0, 3);
            
            cv::GMat g_out;
            switch (operation) {
                case 0:
                    g_out = cv::gapi::boxFilter(g_in, -1, cv::Size(3, 3));
                    break;
                case 1:
                    g_out = cv::gapi::medianBlur(g_in, 3);
                    break;
                case 2:
                    g_out = cv::gapi::Laplacian(g_in, -1, 3);
                    break;
                case 3:
                    g_out = cv::gapi::convertTo(g_in, -1, 1.5, 10);
                    break;
                default:
                    g_out = g_in;
            }
            
            cv::GComputation dynamic_graph(cv::GIn(g_in), cv::GOut(g_out));
            
            // Compile the graph
            auto compiled = dynamic_graph.compile(cv::descr_of(input_image));
            
            // Execute compiled graph
            cv::Mat compiled_output;
            compiled(input_image, compiled_output);
            
            // Re-execute with different input
            cv::Mat input_image3(image_size, image_type);
            rng.fill(input_image3, cv::RNG::UNIFORM, 0, 256);
            compiled(input_image3, compiled_output);
        }
        
        // Test 7: Graph with color conversion operations
        if (use_color) {
            cv::GMat g_in;
            
            // Choose color conversion operation based on fuzzer input
            int conv_op = fdp.ConsumeIntegralInRange<int>(0, 4);
            
            cv::GMat g_convert;
            
            // Declare variables outside switch to avoid scope issues
            float rY = 0.299f, gY = 0.587f, bY = 0.114f;
            
            switch (conv_op) {
                case 0:
                    // Convert BGR to Grayscale
                    g_convert = cv::gapi::BGR2Gray(g_in);
                    break;
                case 1:
                    // Convert BGR to RGB
                    g_convert = cv::gapi::BGR2RGB(g_in);
                    break;
                case 2:
                    // Convert RGB to YUV
                    g_convert = cv::gapi::RGB2YUV(g_in);
                    break;
                case 3:
                    // Convert BGR to YUV
                    g_convert = cv::gapi::BGR2YUV(g_in);
                    break;
                case 4:
                    // Convert RGB to Gray with custom coefficients
                    rY = fdp.ConsumeFloatingPointInRange<float>(0.1f, 0.5f);
                    gY = fdp.ConsumeFloatingPointInRange<float>(0.1f, 0.5f);
                    bY = fdp.ConsumeFloatingPointInRange<float>(0.1f, 0.5f);
                    g_convert = cv::gapi::RGB2Gray(g_in, rY, gY, bY);
                    break;
                default:
                    g_convert = g_in;
            }
            
            cv::GComputation color_graph(cv::GIn(g_in), cv::GOut(g_convert));
            
            cv::Mat color_output;
            color_graph.apply(input_image, color_output);
        }
        
    } catch (const cv::Exception& e) {
        // Silently catch OpenCV exceptions during fuzzing
        // This is expected behavior for invalid inputs
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_009
