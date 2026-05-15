#include <opencv2/core.hpp>
#include <opencv2/core/hal/hal.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need enough for matrix dimensions and random seed
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Step 1: Create input arrays (cv::Mat) of float/double types with random data using cv::randu
        // Consume matrix dimensions (keep small for fuzzing efficiency)
        int rows = fdp.ConsumeIntegralInRange(1, 50);
        int cols = fdp.ConsumeIntegralInRange(1, 50);
        int total_elements = rows * cols;
        
        // Consume operation choice to test different mathematical functions
        int operation_choice = fdp.ConsumeIntegralInRange(0, 7);
        // 0: invSqrt float, 1: invSqrt double, 2: sqrt float, 3: sqrt double
        // 4: exp float, 5: exp double, 6: log float, 7: log double
        
        // Test float operations
        if (operation_choice == 0 || operation_choice == 2 || operation_choice == 4 || operation_choice == 6) {
            // Create input and output matrices for float operations
            cv::Mat src_float(rows, cols, CV_32F);
            cv::Mat dst_float(rows, cols, CV_32F);
            
            // Fill source matrix with values from fuzzer input
            // Check if we have enough input for all elements
            if (fdp.remaining_bytes() < total_elements * sizeof(float)) {
                return 0; // Not enough input
            }
            
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    src_float.at<float>(i, j) = fdp.ConsumeFloatingPoint<float>();
                }
            }
            
            // Prepare vectors for HAL functions
            std::vector<float> src_data(total_elements);
            std::vector<float> dst_data(total_elements);
            
            // Copy matrix data to vectors (contiguous memory)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    src_data[i * cols + j] = src_float.at<float>(i, j);
                }
            }
            
            // Test the chosen mathematical operation
            switch (operation_choice) {
                case 0: // invSqrt32f
                    cv::hal::invSqrt32f(src_data.data(), dst_data.data(), total_elements);
                    break;
                case 2: // sqrt32f
                    cv::hal::sqrt32f(src_data.data(), dst_data.data(), total_elements);
                    break;
                case 4: // exp32f
                    cv::hal::exp32f(src_data.data(), dst_data.data(), total_elements);
                    break;
                case 6: // log32f
                    // Ensure positive values for log
                    for (auto& val : src_data) {
                        val = std::abs(val) + 1e-6f; // Avoid log(0) or negative
                    }
                    cv::hal::log32f(src_data.data(), dst_data.data(), total_elements);
                    break;
            }
        }
        
        // Test double operations
        if (operation_choice == 1 || operation_choice == 3 || operation_choice == 5 || operation_choice == 7) {
            // Create input and output matrices for double operations
            cv::Mat src_double(rows, cols, CV_64F);
            cv::Mat dst_double(rows, cols, CV_64F);
            
            // Fill source matrix with values from fuzzer input
            // Check if we have enough input for all elements
            if (fdp.remaining_bytes() < total_elements * sizeof(double)) {
                return 0; // Not enough input
            }
            
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    src_double.at<double>(i, j) = fdp.ConsumeFloatingPoint<double>();
                }
            }
            // Prepare vectors for HAL functions
            std::vector<double> src_data(total_elements);
            std::vector<double> dst_data(total_elements);
            
            // Copy matrix data to vectors (contiguous memory)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    src_data[i * cols + j] = src_double.at<double>(i, j);
                }
            }
            
            // Test the chosen mathematical operation
            switch (operation_choice) {
                case 1: // invSqrt64f
                    cv::hal::invSqrt64f(src_data.data(), dst_data.data(), total_elements);
                    break;
                case 3: // sqrt64f
                    cv::hal::sqrt64f(src_data.data(), dst_data.data(), total_elements);
                    break;
                case 5: // exp64f
                    cv::hal::exp64f(src_data.data(), dst_data.data(), total_elements);
                    break;
                case 7: // log64f
                    // Ensure positive values for log
                    for (auto& val : src_data) {
                        val = std::abs(val) + 1e-12; // Avoid log(0) or negative
                    }
                    cv::hal::log64f(src_data.data(), dst_data.data(), total_elements);
                    break;
            }
        }
        
        // Step 4: Optionally test cartToPolar/polarToCart with paired arrays
        // Use remaining fuzzer input to decide whether to test these
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            // Create paired arrays for cartToPolar/polarToCart
            int polar_rows = fdp.ConsumeIntegralInRange(1, 30);
            int polar_cols = fdp.ConsumeIntegralInRange(1, 30);
            int polar_elements = polar_rows * polar_cols;
            
            bool test_float = fdp.ConsumeBool();
            bool angle_in_degrees = fdp.ConsumeBool();
            
            if (test_float) {
                // Test cartToPolar32f and polarToCart32f
                std::vector<float> x_data(polar_elements);
                std::vector<float> y_data(polar_elements);
                std::vector<float> mag_data(polar_elements);
                std::vector<float> angle_data(polar_elements);
                std::vector<float> x_recon_data(polar_elements);
                std::vector<float> y_recon_data(polar_elements);
                
                // Fill x and y with random values
                for (int i = 0; i < polar_elements; ++i) {
                    x_data[i] = fdp.ConsumeFloatingPointInRange<float>(-100.0f, 100.0f);
                    y_data[i] = fdp.ConsumeFloatingPointInRange<float>(-100.0f, 100.0f);
                }
                
                // Test cartToPolar32f
                cv::hal::cartToPolar32f(x_data.data(), y_data.data(), 
                                       mag_data.data(), angle_data.data(), 
                                       polar_elements, angle_in_degrees);
                
                // Test polarToCart32f (if we have enough remaining input)
                if (fdp.remaining_bytes() > 0) {
                    cv::hal::polarToCart32f(mag_data.data(), angle_data.data(),
                                           x_recon_data.data(), y_recon_data.data(),
                                           polar_elements, angle_in_degrees);
                }
            } else {
                // Test cartToPolar64f and polarToCart64f
                std::vector<double> x_data(polar_elements);
                std::vector<double> y_data(polar_elements);
                std::vector<double> mag_data(polar_elements);
                std::vector<double> angle_data(polar_elements);
                std::vector<double> x_recon_data(polar_elements);
                std::vector<double> y_recon_data(polar_elements);
                
                // Fill x and y with random values
                for (int i = 0; i < polar_elements; ++i) {
                    x_data[i] = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
                    y_data[i] = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
                }
                
                // Test cartToPolar64f
                cv::hal::cartToPolar64f(x_data.data(), y_data.data(), 
                                       mag_data.data(), angle_data.data(), 
                                       polar_elements, angle_in_degrees);
                
                // Test polarToCart64f (if we have enough remaining input)
                if (fdp.remaining_bytes() > 0) {
                    cv::hal::polarToCart64f(mag_data.data(), angle_data.data(),
                                           x_recon_data.data(), y_recon_data.data(),
                                           polar_elements, angle_in_degrees);
                }
            }
        }
        
        // Step 5: Verify results don't crash (implied by successful execution)
        // Step 6: Automatic cleanup via destructors (RAII handles this)
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
        return 0;
    }
    
    return 0;
}

} // namespace harness_008
