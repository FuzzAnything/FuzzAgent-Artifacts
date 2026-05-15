#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

namespace harness_001 {
// OpenCV mathematical coordinate transformation fuzzing harness
// Targets: cartToPolar, polarToCart, magnitude functions (0% coverage)
// harness_001.cpp


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for matrix operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Part 1: Determine matrix parameters from fuzzer input
        int rows = fdp.ConsumeIntegralInRange<int>(1, 50);
        int cols = fdp.ConsumeIntegralInRange<int>(1, 50);
        int depth_type = fdp.ConsumeIntegralInRange<int>(0, 1); // 0: CV_32F, 1: CV_64F
        int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
        
        int mat_type;
        if (depth_type == 0) {
            // CV_32F (float)
            switch (channels % 4) {
                case 0: mat_type = CV_32FC1; break;
                case 1: mat_type = CV_32FC2; break;
                case 2: mat_type = CV_32FC3; break;
                case 3: mat_type = CV_32FC4; break;
            }
        } else {
            // CV_64F (double)
            switch (channels % 4) {
                case 0: mat_type = CV_64FC1; break;
                case 1: mat_type = CV_64FC2; break;
                case 2: mat_type = CV_64FC3; break;
                case 3: mat_type = CV_64FC4; break;
            }
        }
        
        // Calculate required data size for two matrices (X and Y)
        size_t elem_size = CV_ELEM_SIZE(mat_type);
        size_t matrix_size = rows * cols * elem_size;
        size_t total_required_size = 2 * matrix_size + 4; // Two matrices + operation type
        
        if (fdp.remaining_bytes() < total_required_size) {
            return 0;
        }
        
        // Create X matrix (cartesian coordinates)
        std::vector<uint8_t> x_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
        cv::Mat X(rows, cols, mat_type, x_data.data());
        
        // Create Y matrix (cartesian coordinates)
        std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
        cv::Mat Y(rows, cols, mat_type, y_data.data());
        
        // Determine which mathematical operation to test
        int operation = fdp.ConsumeIntegralInRange<int>(0, 4);
        bool angle_in_degrees = fdp.ConsumeBool();
        
        cv::Mat Mag, Angle, X_result, Y_result;
        
        switch (operation % 5) {
            case 0:
                // Test cartToPolar: Convert cartesian to polar coordinates
                cv::cartToPolar(X, Y, Mag, Angle, angle_in_degrees);
                break;
                
            case 1:
                // Test polarToCart: Convert polar to cartesian coordinates
                // First create magnitude and angle matrices from remaining data
                if (fdp.remaining_bytes() >= 2 * matrix_size) {
                    std::vector<uint8_t> mag_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    std::vector<uint8_t> angle_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    
                    cv::Mat Mag_input(rows, cols, mat_type, mag_data.data());
                    cv::Mat Angle_input(rows, cols, mat_type, angle_data.data());
                    
                    cv::polarToCart(Mag_input, Angle_input, X_result, Y_result, angle_in_degrees);
                }
                break;
                
            case 2:
                // Test magnitude: Calculate magnitude of 2D vectors
                cv::magnitude(X, Y, Mag);
                break;
                
            case 3:
                // Test combined operations: cartToPolar -> polarToCart round trip
                cv::cartToPolar(X, Y, Mag, Angle, angle_in_degrees);
                
                if (!Mag.empty() && !Angle.empty()) {
                    cv::polarToCart(Mag, Angle, X_result, Y_result, angle_in_degrees);
                }
                break;
                
            case 4:
                // Test with different data types mixing
                if (depth_type == 0) {
                    // Test with float input
                    cv::cartToPolar(X, Y, Mag, Angle, angle_in_degrees);
                    cv::magnitude(X, Y, Mag);
                } else {
                    // Test with double input
                    cv::cartToPolar(X, Y, Mag, Angle, angle_in_degrees);
                    cv::magnitude(X, Y, Mag);
                }
                break;
        }
        
        // Part 2: Test with single-channel matrices (common use case)
        if (fdp.remaining_bytes() >= 2 * rows * cols * sizeof(float) + 4) {
            int single_channel_rows = fdp.ConsumeIntegralInRange<int>(1, 30);
            int single_channel_cols = fdp.ConsumeIntegralInRange<int>(1, 30);
            
            size_t single_channel_size = single_channel_rows * single_channel_cols * sizeof(float);
            if (fdp.remaining_bytes() >= 2 * single_channel_size) {
                std::vector<uint8_t> x_single_data = fdp.ConsumeBytes<uint8_t>(single_channel_size);
                std::vector<uint8_t> y_single_data = fdp.ConsumeBytes<uint8_t>(single_channel_size);
                
                cv::Mat X_single(single_channel_rows, single_channel_cols, CV_32FC1, x_single_data.data());
                cv::Mat Y_single(single_channel_rows, single_channel_cols, CV_32FC1, y_single_data.data());
                
                cv::Mat Mag_single, Angle_single;
                bool test_degrees = fdp.ConsumeBool();
                
                // Test all three functions with single channel data
                cv::cartToPolar(X_single, Y_single, Mag_single, Angle_single, test_degrees);
                cv::magnitude(X_single, Y_single, Mag_single);
                
                if (!Mag_single.empty() && !Angle_single.empty()) {
                    cv::Mat X_roundtrip, Y_roundtrip;
                    cv::polarToCart(Mag_single, Angle_single, X_roundtrip, Y_roundtrip, test_degrees);
                }
            }
        }
        
        // Part 3: Test with edge cases using remaining input
        if (fdp.remaining_bytes() > 0) {
            // Create small matrices for edge case testing
            int edge_rows = 1;
            int edge_cols = 1;
            size_t edge_size = edge_rows * edge_cols * sizeof(float);
            
            if (fdp.remaining_bytes() >= 2 * edge_size) {
                std::vector<uint8_t> edge_x_data = fdp.ConsumeBytes<uint8_t>(edge_size);
                std::vector<uint8_t> edge_y_data = fdp.ConsumeBytes<uint8_t>(edge_size);
                
                cv::Mat X_edge(edge_rows, edge_cols, CV_32FC1, edge_x_data.data());
                cv::Mat Y_edge(edge_rows, edge_cols, CV_32FC1, edge_y_data.data());
                
                // Test with zero vectors
                cv::Mat Mag_edge, Angle_edge;
                cv::cartToPolar(X_edge, Y_edge, Mag_edge, Angle_edge, false);
                cv::magnitude(X_edge, Y_edge, Mag_edge);
            }
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they're expected during fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_001
