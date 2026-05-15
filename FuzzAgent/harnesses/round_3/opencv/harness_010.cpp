#include <cstddef>
#include <cstdint>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/gapi.hpp>
#include <opencv2/gapi/core.hpp>
#include <opencv2/gapi/imgproc.hpp>
#include <opencv2/gapi/cpu/gcpukernel.hpp>

namespace harness_010 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for basic parameters
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image dimensions and parameters
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    
    // Consume parameters for GAPI operations
    int blur_ksize = fdp.ConsumeIntegralInRange<int>(1, 31);
    if (blur_ksize % 2 == 0) blur_ksize++; // Ensure odd kernel size
    
    int sobel_ddepth = fdp.ConsumeIntegral<uint8_t>() % 3; // 0: CV_8U, 1: CV_16U, 2: CV_32F
    int sobel_dx = fdp.ConsumeIntegralInRange<int>(0, 2);
    int sobel_dy = fdp.ConsumeIntegralInRange<int>(0, 2);
    int sobel_ksize = fdp.ConsumeIntegralInRange<int>(1, 7);
    if (sobel_ksize % 2 == 0) sobel_ksize++; // Ensure odd kernel size
    
    // Use remaining bytes as image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Create input matrix from the image data
    if (image_data.size() < static_cast<size_t>(width * height)) {
        // Not enough data for the specified dimensions
        return 0;
    }
    
    try {
        // Create input matrix (grayscale image for equalizeHist)
        cv::Mat input_mat(height, width, CV_8UC1, image_data.data());
        
        // Define G-API computation graph
        cv::GMat in;  // Input node
        
        // Pipeline: input -> equalizeHist -> blur -> Sobel -> output
        cv::GMat equalized = cv::gapi::equalizeHist(in);
        cv::GMat blurred = cv::gapi::blur(equalized, cv::Size(blur_ksize, blur_ksize));
        
        // Convert ddepth code to actual OpenCV depth
        int actual_ddepth;
        switch (sobel_ddepth) {
            case 0: actual_ddepth = CV_8U; break;
            case 1: actual_ddepth = CV_16U; break;
            case 2: actual_ddepth = CV_32F; break;
            default: actual_ddepth = CV_8U; break;
        }
        
        cv::GMat output = cv::gapi::Sobel(blurred, actual_ddepth, sobel_dx, sobel_dy, sobel_ksize);
        
        // Create GComputation from the graph
        cv::GComputation pipeline(in, output);
        
        // Compile the graph for CPU backend - use cv::Size instead of cv::gapi::own::Size
        auto compiled = pipeline.compile(cv::GMatDesc{CV_8U, 1, cv::Size(width, height)});
        
        // Prepare output matrix
        cv::Mat output_mat;
        
        // Apply the compiled pipeline
        compiled(cv::gin(input_mat), cv::gout(output_mat));
        
        // Validate output
        if (!output_mat.empty()) {
            // Check basic properties
            int out_rows = output_mat.rows;
            int out_cols = output_mat.cols;
            int out_type = output_mat.type();
            
            // Access some pixels to exercise the code (if matrix is not empty)
            if (out_rows > 0 && out_cols > 0) {
                // Just access a pixel to ensure the output is valid
                if (output_mat.type() == CV_8U) {
                    volatile uint8_t pixel = output_mat.at<uint8_t>(0, 0);
                    (void)pixel; // Suppress unused variable warning
                } else if (output_mat.type() == CV_16U) {
                    volatile uint16_t pixel = output_mat.at<uint16_t>(0, 0);
                    (void)pixel;
                } else if (output_mat.type() == CV_32F) {
                    volatile float pixel = output_mat.at<float>(0, 0);
                    (void)pixel;
                }
            }
        }
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected during fuzzing
        // Just return 0 to indicate normal execution
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_010
