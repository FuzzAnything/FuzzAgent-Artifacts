#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace harness_013 {
// OpenCV pipeline/graph-like operations fuzzing harness
// Targets: Traditional OpenCV pipeline operations that parallel G-API functionality (0% coverage across 3276 G-API functions)
// harness_013.cpp - Focuses on sequential image processing pipelines
// Differentiates from previous harnesses by creating multi-step pipelines that mirror G-API graph concepts


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic pipeline operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        using namespace cv;
        
        // Determine pipeline type from fuzzer input
        uint8_t pipeline_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        // Create test matrix from fuzzer input
        int rows = fdp.ConsumeIntegralInRange<int>(10, 100);
        int cols = fdp.ConsumeIntegralInRange<int>(10, 100);
        
        size_t image_size = rows * cols;
        if (fdp.remaining_bytes() < image_size) {
            return 0;
        }
        
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
        Mat test_image(rows, cols, CV_8UC1, image_data.data());
        
        if (test_image.empty()) {
            return 0;
        }
        
        Mat intermediate, output;
        
        switch (pipeline_type) {
            case 0: {
                // Pipeline 0: Blur -> Threshold
                blur(test_image, intermediate, Size(3, 3));
                double thresh = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
                threshold(intermediate, output, thresh, 255.0, THRESH_BINARY);
                break;
            }
            
            case 1: {
                // Pipeline 1: Gaussian Blur -> Sobel edge detection
                GaussianBlur(test_image, intermediate, Size(3, 3), 0);
                Mat grad_x, grad_y;
                Sobel(intermediate, grad_x, CV_32F, 1, 0);
                Sobel(intermediate, grad_y, CV_32F, 0, 1);
                magnitude(grad_x, grad_y, output);
                convertScaleAbs(output, output);
                break;
            }
            
            case 2: {
                // Pipeline 2: Resize -> Median Blur -> Adaptive Threshold
                resize(test_image, intermediate, Size(), 0.5, 0.5);
                medianBlur(intermediate, output, 3);
                double max_val = fdp.ConsumeFloatingPointInRange<double>(100.0, 255.0);
                adaptiveThreshold(output, output, max_val, ADAPTIVE_THRESH_MEAN_C, 
                                  THRESH_BINARY, 3, 2.0);
                break;
            }
            
            case 3: {
                // Pipeline 3: Erode -> Dilate (morphological operations)
                int ksize = fdp.ConsumeIntegralInRange<int>(1, 5);
                Mat kernel = getStructuringElement(MORPH_RECT, Size(ksize, ksize));
                erode(test_image, intermediate, kernel);
                dilate(intermediate, output, kernel);
                break;
            }
            
            case 4: {
                // Pipeline 4: Multiple operations in sequence
                // Step 1: Blur
                blur(test_image, intermediate, Size(3, 3));
                
                // Step 2: Edge detection
                Mat edges;
                Canny(intermediate, edges, 50, 150);
                
                // Step 3: Add weighted combination
                double alpha = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
                double beta = 1.0 - alpha;
                addWeighted(intermediate, alpha, edges, beta, 0.0, output);
                break;
            }
            
            case 5: {
                // Pipeline 5: Color conversion (if image allows) and operations
                // Convert single channel to 3 channels for more operations
                Mat color_image;
                cvtColor(test_image, color_image, COLOR_GRAY2BGR);
                
                // Apply bilateral filter (edge-preserving)
                bilateralFilter(color_image, intermediate, 5, 50, 50);
                
                // Convert back to grayscale
                cvtColor(intermediate, output, COLOR_BGR2GRAY);
                break;
            }
        }
        
        // Test with different parameter combinations if we have more data
        if (fdp.remaining_bytes() >= 8) {
            // Create another test image
            int rows2 = fdp.ConsumeIntegralInRange<int>(5, 50);
            int cols2 = fdp.ConsumeIntegralInRange<int>(5, 50);
            
            size_t image_size2 = rows2 * cols2;
            if (fdp.remaining_bytes() >= image_size2) {
                std::vector<uint8_t> image_data2 = fdp.ConsumeBytes<uint8_t>(image_size2);
                Mat test_image2(rows2, cols2, CV_8UC1, image_data2.data());
                
                if (!test_image2.empty()) {
                    // Test pyramid operations (multi-scale processing)
                    std::vector<Mat> pyramid;
                    buildPyramid(test_image2, pyramid, 2);
                    
                    // Process each level
                    for (size_t i = 0; i < pyramid.size(); i++) {
                        Mat level_processed;
                        GaussianBlur(pyramid[i], level_processed, Size(3, 3), 0);
                    }
                }
            }
        }
        
    } catch (const cv::Exception& e) {
        // Silently catch OpenCV exceptions - they're expected during fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_013
