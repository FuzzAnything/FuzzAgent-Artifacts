#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace harness_002 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for matrix dimensions and type
    int rows = fdp.ConsumeIntegralInRange(1, 100);
    int cols = fdp.ConsumeIntegralInRange(1, 100);
    int mat_type_choice = fdp.ConsumeIntegralInRange(0, 4); // 0: CV_8U, 1: CV_16U, 2: CV_16S, 3: CV_32F, 4: CV_64F
    int mat_type;
    switch (mat_type_choice) {
        case 0: mat_type = CV_8U; break;
        case 1: mat_type = CV_16U; break;
        case 2: mat_type = CV_16S; break;
        case 3: mat_type = CV_32F; break;
        default: mat_type = CV_64F; break;
    }
    
    // Create random matrix for testing
    cv::Mat src(rows, cols, mat_type);
    
    // Fill matrix with random data from fuzzer input
    // We'll fill with random values using loops based on data type
    if (mat_type == CV_8U) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                src.at<uint8_t>(i, j) = fdp.ConsumeIntegral<uint8_t>();
            }
        }
    } else if (mat_type == CV_16U) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                src.at<uint16_t>(i, j) = fdp.ConsumeIntegral<uint16_t>();
            }
        }
    } else if (mat_type == CV_16S) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                src.at<int16_t>(i, j) = fdp.ConsumeIntegral<int16_t>();
            }
        }
    } else if (mat_type == CV_32F) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                src.at<float>(i, j) = fdp.ConsumeFloatingPoint<float>();
            }
        }
    } else { // CV_64F
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                src.at<double>(i, j) = fdp.ConsumeFloatingPoint<double>();
            }
        }
    }
    
    // Consume parameters for kernel/structure element
    int kernel_shape_choice = fdp.ConsumeIntegralInRange(0, 3); // 0: RECT, 1: CROSS, 2: ELLIPSE, 3: DIAMOND
    int kernel_shape;
    switch (kernel_shape_choice) {
        case 0: kernel_shape = cv::MORPH_RECT; break;
        case 1: kernel_shape = cv::MORPH_CROSS; break;
        case 2: kernel_shape = cv::MORPH_ELLIPSE; break;
        default: kernel_shape = cv::MORPH_DIAMOND; break;
    }
    
    int kernel_size_x = fdp.ConsumeIntegralInRange(1, 15);
    int kernel_size_y = fdp.ConsumeIntegralInRange(1, 15);
    
    // Create kernel/structure element
    cv::Mat kernel = cv::getStructuringElement(kernel_shape, 
                                              cv::Size(kernel_size_x, kernel_size_y));
    
    // Consume anchor position
    int anchor_x = fdp.ConsumeIntegralInRange(-1, kernel_size_x - 1);
    int anchor_y = fdp.ConsumeIntegralInRange(-1, kernel_size_y - 1);
    cv::Point anchor(anchor_x, anchor_y);
    
    // Consume iterations
    int iterations = fdp.ConsumeIntegralInRange(1, 5);
    
    // Consume border type
    int border_type_choice = fdp.ConsumeIntegralInRange(0, 4); // 0: CONSTANT, 1: REPLICATE, 2: REFLECT, 3: WRAP, 4: REFLECT_101
    int border_type;
    switch (border_type_choice) {
        case 0: border_type = cv::BORDER_CONSTANT; break;
        case 1: border_type = cv::BORDER_REPLICATE; break;
        case 2: border_type = cv::BORDER_REFLECT; break;
        case 3: border_type = cv::BORDER_WRAP; break;
        default: border_type = cv::BORDER_REFLECT_101; break;
    }
    
    try {
        // Output matrices
        cv::Mat dst_erode, dst_dilate, dst_morph;
        
        // Test erode operation
        cv::erode(src, dst_erode, kernel, anchor, iterations, border_type);
        
        // Test dilate operation  
        cv::dilate(src, dst_dilate, kernel, anchor, iterations, border_type);
        
        // Test various morphologyEx operations
        int morph_op_choice = fdp.ConsumeIntegralInRange(0, 7); // 0-7: MORPH_ERODE to MORPH_HITMISS
        int morph_op;
        switch (morph_op_choice) {
            case 0: morph_op = cv::MORPH_ERODE; break;
            case 1: morph_op = cv::MORPH_DILATE; break;
            case 2: morph_op = cv::MORPH_OPEN; break;
            case 3: morph_op = cv::MORPH_CLOSE; break;
            case 4: morph_op = cv::MORPH_GRADIENT; break;
            case 5: morph_op = cv::MORPH_TOPHAT; break;
            case 6: morph_op = cv::MORPH_BLACKHAT; break;
            default: morph_op = cv::MORPH_HITMISS; break;
        }
        
        cv::morphologyEx(src, dst_morph, morph_op, kernel, anchor, iterations, border_type);
        
        // Test with different kernel sizes (including empty kernel for default 3x3)
        if (fdp.ConsumeBool()) {
            cv::Mat empty_kernel;
            cv::Mat dst_default;
            cv::erode(src, dst_default, empty_kernel);
        }
        
        // Test with different anchor points
        if (fdp.ConsumeBool()) {
            cv::Mat dst_anchor_test;
            cv::Point center_anchor(-1, -1); // Default center anchor
            cv::erode(src, dst_anchor_test, kernel, center_anchor, iterations, border_type);
        }
        
        // Test with single iteration vs multiple iterations
        if (fdp.ConsumeBool()) {
            cv::Mat dst_single_iter;
            cv::dilate(src, dst_single_iter, kernel, anchor, 1, border_type);
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they are expected for invalid inputs
    } catch (...) {
        // Ignore any other exceptions
    }
    
    return 0;
}

} // namespace harness_002
