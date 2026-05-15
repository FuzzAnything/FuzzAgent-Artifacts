#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

namespace harness_011 {
// OpenCV mathematical array operations fuzzing harness
// Targets: cv::exp, cv::sqrt, cv::magnitude functions with 52+ branches each (0% coverage)
// harness_011.cpp - Focuses on high-level mathematical array operations using cv::Mat API
// Differentiates from previous harnesses:
// - harness_001/003/004: Coordinate transformations (cartToPolar, polarToCart)
// - harness_002: Low-level HAL functions (sqrt32f, exp32f, etc.)
// - harness_007: HAL APIs with raw pointer arrays
// - harness_009: Channel operations
// This harness specifically targets cv::exp(), cv::sqrt(), cv::magnitude() with matrix operations


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic matrix operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Determine test scenario from fuzzer input
        uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        // Common matrix parameters
        int rows = fdp.ConsumeIntegralInRange<int>(1, 20);
        int cols = fdp.ConsumeIntegralInRange<int>(1, 20);
        int depth_choice = fdp.ConsumeIntegralInRange<int>(0, 1); // 0: CV_32F, 1: CV_64F
        int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
        
        int mat_type;
        if (depth_choice == 0) {
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
        
        size_t elem_size = CV_ELEM_SIZE(mat_type);
        size_t matrix_size = rows * cols * elem_size;
        
        switch (scenario) {
            case 0: {
                // Scenario 0: Test cv::exp() - exponential function
                if (fdp.remaining_bytes() < matrix_size) {
                    return 0;
                }
                
                std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                cv::Mat src(rows, cols, mat_type, src_data.data());
                cv::Mat dst;
                
                // Apply exponential function
                cv::exp(src, dst);
                
                // Test with range-limited values to avoid overflow
                if (fdp.remaining_bytes() >= matrix_size) {
                    std::vector<uint8_t> src2_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    cv::Mat src2(rows, cols, mat_type, src2_data.data());
                    
                    // Scale values to reasonable range for exponential
                    cv::Mat src_scaled;
                    if (depth_choice == 0) {
                        // For float, limit to [-10, 10] range to avoid huge values
                        src2.convertTo(src_scaled, CV_32F, 0.1f, 0.0f);
                    } else {
                        // For double, similar range limitation
                        src2.convertTo(src_scaled, CV_64F, 0.1, 0.0);
                    }
                    
                    cv::exp(src_scaled, dst);
                }
                break;
            }
            
            case 1: {
                // Scenario 1: Test cv::sqrt() - square root function
                if (fdp.remaining_bytes() < matrix_size) {
                    return 0;
                }
                
                std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                cv::Mat src(rows, cols, mat_type, src_data.data());
                cv::Mat dst;
                
                // Apply square root function
                cv::sqrt(src, dst);
                
                // Test with positive values only (sqrt of negative gives NaN)
                if (fdp.remaining_bytes() >= matrix_size) {
                    std::vector<uint8_t> src2_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    cv::Mat src2(rows, cols, mat_type, src2_data.data());
                    
                    // Make values positive for square root
                    cv::Mat src_positive = cv::abs(src2);
                    
                    // Ensure no zeros (or add small epsilon)
                    if (depth_choice == 0) {
                        src_positive += 1e-6f;
                    } else {
                        src_positive += 1e-12;
                    }
                    
                    cv::sqrt(src_positive, dst);
                }
                break;
            }
            
            case 2: {
                // Scenario 2: Test cv::magnitude() - magnitude of 2D vectors
                // Need two input matrices (X and Y components)
                if (fdp.remaining_bytes() < 2 * matrix_size) {
                    return 0;
                }
                
                std::vector<uint8_t> x_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                
                cv::Mat X(rows, cols, mat_type, x_data.data());
                cv::Mat Y(rows, cols, mat_type, y_data.data());
                cv::Mat mag;
                
                // Calculate magnitude of vectors (sqrt(x² + y²))
                cv::magnitude(X, Y, mag);
                
                // Test with different channel configurations
                if (fdp.remaining_bytes() >= 2 * matrix_size) {
                    std::vector<uint8_t> x2_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    std::vector<uint8_t> y2_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    
                    cv::Mat X2(rows, cols, mat_type, x2_data.data());
                    cv::Mat Y2(rows, cols, mat_type, y2_data.data());
                    
                    // Test with scaled values to avoid overflow
                    cv::Mat X_scaled, Y_scaled;
                    if (depth_choice == 0) {
                        X2.convertTo(X_scaled, CV_32F, 0.01f, 0.0f);
                        Y2.convertTo(Y_scaled, CV_32F, 0.01f, 0.0f);
                    } else {
                        X2.convertTo(X_scaled, CV_64F, 0.01, 0.0);
                        Y2.convertTo(Y_scaled, CV_64F, 0.01, 0.0);
                    }
                    
                    cv::magnitude(X_scaled, Y_scaled, mag);
                }
                break;
            }
            
            case 3: {
                // Scenario 3: Combined mathematical operations
                // Test sequences of exp -> sqrt -> magnitude to explore complex paths
                if (fdp.remaining_bytes() < matrix_size) {
                    return 0;
                }
                
                std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                cv::Mat src(rows, cols, mat_type, src_data.data());
                
                // Create positive version for sqrt
                cv::Mat src_pos = cv::abs(src);
                if (depth_choice == 0) {
                    src_pos += 1e-6f;
                } else {
                    src_pos += 1e-12;
                }
                
                cv::Mat exp_result, sqrt_result;
                
                // Test exp -> sqrt chain
                cv::exp(src, exp_result);
                if (!exp_result.empty()) {
                    cv::sqrt(exp_result, sqrt_result);
                }
                
                // Test sqrt -> exp chain (with positive input)
                if (!src_pos.empty()) {
                    cv::sqrt(src_pos, sqrt_result);
                    if (!sqrt_result.empty()) {
                        cv::exp(sqrt_result, exp_result);
                    }
                }
                
                // Test magnitude if we have enough data for two matrices
                if (fdp.remaining_bytes() >= matrix_size) {
                    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(matrix_size);
                    cv::Mat Y(rows, cols, mat_type, y_data.data());
                    cv::Mat mag_result;
                    
                    cv::magnitude(src, Y, mag_result);
                    
                    // Test mathematical transformation: magnitude -> exp
                    if (!mag_result.empty()) {
                        // Make magnitude positive for exp (already is by definition)
                        cv::exp(mag_result, exp_result);
                    }
                }
                break;
            }
        }
        
        // Additional test: Edge cases with special values
        if (fdp.remaining_bytes() >= 16) {
            int edge_case = fdp.ConsumeIntegralInRange<int>(0, 3);
            
            // Create small matrices for edge case testing
            int small_rows = fdp.ConsumeIntegralInRange<int>(1, 3);
            int small_cols = fdp.ConsumeIntegralInRange<int>(1, 3);
            size_t small_matrix_size = small_rows * small_cols * elem_size;
            
            if (fdp.remaining_bytes() >= small_matrix_size) {
                std::vector<uint8_t> edge_data = fdp.ConsumeBytes<uint8_t>(small_matrix_size);
                cv::Mat edge_src(small_rows, small_cols, mat_type, edge_data.data());
                cv::Mat edge_dst;
                
                switch (edge_case % 4) {
                    case 0:
                        // Test with near-zero values
                        cv::exp(edge_src * 0.001f, edge_dst);
                        break;
                    case 1:
                        // Test with moderate values
                        cv::sqrt(cv::abs(edge_src) + 1.0f, edge_dst);
                        break;
                    case 2:
                        // Test with two small matrices for magnitude
                        if (fdp.remaining_bytes() >= small_matrix_size) {
                            std::vector<uint8_t> edge_y_data = fdp.ConsumeBytes<uint8_t>(small_matrix_size);
                            cv::Mat edge_y(small_rows, small_cols, mat_type, edge_y_data.data());
                            cv::magnitude(edge_src, edge_y, edge_dst);
                        }
                        break;
                    case 3:
                        // Test mathematical identity: exp(log(x)) ≈ x (for positive x)
                        cv::Mat positive_src = cv::abs(edge_src) + 1.0f;
                        cv::Mat log_result;
                        cv::log(positive_src, log_result);
                        if (!log_result.empty()) {
                            cv::exp(log_result, edge_dst);
                        }
                        break;
                }
            }
        }
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected during fuzzing
        // Silently catch them to continue fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_011
