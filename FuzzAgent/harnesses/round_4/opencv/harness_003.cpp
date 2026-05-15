#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

namespace harness_003 {
// OpenCV mathematical coordinate transformation fuzzing harness - Alternative approach
// Targets: cartToPolar, polarToCart, magnitude functions with different parameter combinations
// harness_003.cpp


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Alternative approach: Test with different matrix sizes and data types
        // Focus on edge cases and specific parameter combinations
        
        // Determine test scenario
        int scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
        
        switch (scenario) {
            case 0: {
                // Scenario 0: Single element matrices (edge case)
                int type_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
                int mat_type;
                switch (type_choice % 4) {
                    case 0: mat_type = CV_32FC1; break;
                    case 1: mat_type = CV_32FC2; break;
                    case 2: mat_type = CV_64FC1; break;
                    case 3: mat_type = CV_64FC2; break;
                }
                
                size_t elem_size = CV_ELEM_SIZE(mat_type);
                if (fdp.remaining_bytes() >= 2 * elem_size) {
                    std::vector<uint8_t> x_data = fdp.ConsumeBytes<uint8_t>(elem_size);
                    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(elem_size);
                    
                    cv::Mat X(1, 1, mat_type, x_data.data());
                    cv::Mat Y(1, 1, mat_type, y_data.data());
                    
                    cv::Mat Mag, Angle;
                    bool use_degrees = fdp.ConsumeBool();
                    
                    // Test all three functions
                    cv::cartToPolar(X, Y, Mag, Angle, use_degrees);
                    cv::magnitude(X, Y, Mag);
                    
                    if (!Mag.empty() && !Angle.empty()) {
                        cv::Mat X_result, Y_result;
                        cv::polarToCart(Mag, Angle, X_result, Y_result, use_degrees);
                    }
                }
                break;
            }
            
            case 1: {
                // Scenario 1: 2D vectors (common use case)
                int rows = fdp.ConsumeIntegralInRange<int>(1, 10);
                int cols = fdp.ConsumeIntegralInRange<int>(1, 10);
                int channels = fdp.ConsumeIntegralInRange<int>(1, 2); // 1 or 2 channels
                bool use_double = fdp.ConsumeBool();
                
                int mat_type = (use_double ? CV_64F : CV_32F);
                if (channels == 2) mat_type += 8; // CV_32FC2 or CV_64FC2
                
                size_t elem_size = CV_ELEM_SIZE(mat_type);
                size_t matrix_size = rows * cols * elem_size;
                
                if (fdp.remaining_bytes() >= 2 * matrix_size) {
                    std::vector<uint8_t> x_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    
                    cv::Mat X(rows, cols, mat_type, x_data.data());
                    cv::Mat Y(rows, cols, mat_type, y_data.data());
                    
                    // Test different parameter combinations
                    bool use_degrees = fdp.ConsumeBool();
                    cv::Mat Mag, Angle;
                    
                    // Test cartToPolar with different angleInDegrees values
                    cv::cartToPolar(X, Y, Mag, Angle, use_degrees);
                    
                    // Test magnitude
                    cv::magnitude(X, Y, Mag);
                    
                    // Test polarToCart if we have enough data
                    if (fdp.remaining_bytes() >= 2 * matrix_size) {
                        std::vector<uint8_t> mag_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                        std::vector<uint8_t> angle_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                        
                        cv::Mat Mag_input(rows, cols, mat_type, mag_data.data());
                        cv::Mat Angle_input(rows, cols, mat_type, angle_data.data());
                        
                        cv::Mat X_result, Y_result;
                        cv::polarToCart(Mag_input, Angle_input, X_result, Y_result, use_degrees);
                    }
                }
                break;
            }
            
            case 2: {
                // Scenario 2: Multi-channel matrices (3-4 channels)
                int rows = fdp.ConsumeIntegralInRange<int>(1, 5);
                int cols = fdp.ConsumeIntegralInRange<int>(1, 5);
                int channels = fdp.ConsumeIntegralInRange<int>(3, 4);
                bool use_double = fdp.ConsumeBool();
                
                int mat_type;
                if (use_double) {
                    mat_type = (channels == 3) ? CV_64FC3 : CV_64FC4;
                } else {
                    mat_type = (channels == 3) ? CV_32FC3 : CV_32FC4;
                }
                
                size_t elem_size = CV_ELEM_SIZE(mat_type);
                size_t matrix_size = rows * cols * elem_size;
                
                if (fdp.remaining_bytes() >= 2 * matrix_size) {
                    std::vector<uint8_t> x_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    
                    cv::Mat X(rows, cols, mat_type, x_data.data());
                    cv::Mat Y(rows, cols, mat_type, y_data.data());
                    
                    cv::Mat Mag, Angle;
                    bool use_degrees = fdp.ConsumeBool();
                    
                    // Test with multi-channel data
                    cv::cartToPolar(X, Y, Mag, Angle, use_degrees);
                    cv::magnitude(X, Y, Mag);
                }
                break;
            }
            
            case 3: {
                // Scenario 3: Mixed operations and edge cases
                // Test with zeros, ones, and random data
                int rows = fdp.ConsumeIntegralInRange<int>(1, 8);
                int cols = fdp.ConsumeIntegralInRange<int>(1, 8);
                
                // Create matrices with zeros
                cv::Mat X_zeros = cv::Mat::zeros(rows, cols, CV_32FC1);
                cv::Mat Y_zeros = cv::Mat::zeros(rows, cols, CV_32FC1);
                
                cv::Mat Mag_z, Angle_z;
                cv::cartToPolar(X_zeros, Y_zeros, Mag_z, Angle_z, false);
                cv::magnitude(X_zeros, Y_zeros, Mag_z);
                
                // Create matrices with ones
                cv::Mat X_ones = cv::Mat::ones(rows, cols, CV_32FC1);
                cv::Mat Y_ones = cv::Mat::ones(rows, cols, CV_32FC1);
                
                cv::Mat Mag_o, Angle_o;
                bool use_degrees = fdp.ConsumeBool();
                cv::cartToPolar(X_ones, Y_ones, Mag_o, Angle_o, use_degrees);
                cv::magnitude(X_ones, Y_ones, Mag_o);
                
                // Test polarToCart with synthetic data
                if (!Mag_o.empty() && !Angle_o.empty()) {
                    cv::Mat X_result, Y_result;
                    cv::polarToCart(Mag_o, Angle_o, X_result, Y_result, use_degrees);
                }
                break;
            }
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_003
