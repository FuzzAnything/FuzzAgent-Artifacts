#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/hal.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

namespace harness_006 {
// OpenCV core HAL mathematical functions, linear algebra, and highgui fuzzing harness
// Targets: HAL invSqrt, addRNGBias32f, exp, highgui waitKey, linear algebra LU and Cholesky
// harness_006.cpp - Focus on high-impact uncovered APIs across core, highgui, and linear algebra modules
// Different from harness_002 (HAL functions) and harness_005 (calib3d) by targeting linear algebra and highgui


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for mathematical operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Part 1: Configure parallel processing (setNumThreads)
        int num_threads = fdp.ConsumeIntegralInRange<int>(-1, 32);  // -1 means auto-detect
        cv::setNumThreads(num_threads);
        
        // Part 2: Create input matrices/arrays using random data for mathematical operations
        int array_len = fdp.ConsumeIntegralInRange<int>(1, 100);
        size_t float_array_bytes = array_len * sizeof(float);
        size_t double_array_bytes = array_len * sizeof(double);
        
        // Check if we have enough data for float operations
        if (fdp.remaining_bytes() < float_array_bytes * 3) {
            return 0;
        }
        
        // Create input arrays for HAL mathematical functions
        std::vector<float> src_data_float(array_len);
        std::vector<float> dst_data_float(array_len);
        
        // Fill src_data_float with fuzzer input
        std::vector<uint8_t> float_raw_data = fdp.ConsumeBytes<uint8_t>(float_array_bytes);
        if (float_raw_data.size() == float_array_bytes) {
            memcpy(src_data_float.data(), float_raw_data.data(), float_array_bytes);
            
            // Determine which HAL mathematical operation to test
            int hal_operation = fdp.ConsumeIntegralInRange<int>(0, 2);
            
            switch (hal_operation % 3) {
                case 0:
                    // Test invSqrt32f: Inverse square root function
                    cv::hal::invSqrt32f(src_data_float.data(), dst_data_float.data(), array_len);
                    break;
                    
                case 1:
                    // Test exp32f: Exponential function
                    cv::hal::exp32f(src_data_float.data(), dst_data_float.data(), array_len);
                    break;
                    
                case 2:
                    // Test addRNGBias32f: Add random bias to array
                    // Need scaleBiasPairs array (pairs of scale and bias values)
                    // For each element in src_data, we need a scale and bias pair
                    if (fdp.remaining_bytes() >= float_array_bytes * 2) {
                        std::vector<float> scale_bias_pairs(array_len * 2);
                        std::vector<uint8_t> pairs_raw = fdp.ConsumeBytes<uint8_t>(float_array_bytes * 2);
                        memcpy(scale_bias_pairs.data(), pairs_raw.data(), float_array_bytes * 2);
                        
                        // Make a copy of src_data to modify
                        std::vector<float> arr_copy = src_data_float;
                        cv::hal::addRNGBias32f(arr_copy.data(), scale_bias_pairs.data(), array_len);
                    }
                    break;
            }
        }
        
        // Part 3: Linear algebra operations (LU and Cholesky decomposition)
        if (fdp.remaining_bytes() >= 100) {
            int matrix_size = fdp.ConsumeIntegralInRange<int>(2, 10);  // Square matrix size
            bool use_double = fdp.ConsumeBool();
            
            size_t matrix_elements = matrix_size * matrix_size;
            size_t matrix_bytes = matrix_elements * (use_double ? sizeof(double) : sizeof(float));
            
            if (fdp.remaining_bytes() >= matrix_bytes + sizeof(int)) {
                int linear_alg_op = fdp.ConsumeIntegralInRange<int>(0, 1);
                
                if (use_double) {
                    // Double precision linear algebra
                    std::vector<double> matrix_data(matrix_elements);
                    std::vector<uint8_t> matrix_raw = fdp.ConsumeBytes<uint8_t>(matrix_bytes);
                    memcpy(matrix_data.data(), matrix_raw.data(), matrix_bytes);
                    
                    std::vector<double> b_data(matrix_size);
                    if (fdp.remaining_bytes() >= matrix_size * sizeof(double)) {
                        std::vector<uint8_t> b_raw = fdp.ConsumeBytes<uint8_t>(matrix_size * sizeof(double));
                        memcpy(b_data.data(), b_raw.data(), matrix_size * sizeof(double));
                    }
                    
                    switch (linear_alg_op % 2) {
                        case 0:
                            // LU decomposition (double)
                            {
                                int result = cv::hal::LU64f(
                                    matrix_data.data(), 
                                    matrix_size * sizeof(double),  // astep
                                    matrix_size, 
                                    b_data.data(), 
                                    sizeof(double),  // bstep
                                    1  // n (single RHS)
                                );
                            }
                            break;
                            
                        case 1:
                            // Cholesky decomposition (double)
                            {
                                bool result = cv::hal::Cholesky64f(
                                    matrix_data.data(), 
                                    matrix_size * sizeof(double),  // astep
                                    matrix_size, 
                                    b_data.data(), 
                                    sizeof(double),  // bstep
                                    1  // n (single RHS)
                                );
                            }
                            break;
                    }
                } else {
                    // Single precision linear algebra
                    std::vector<float> matrix_data(matrix_elements);
                    std::vector<uint8_t> matrix_raw = fdp.ConsumeBytes<uint8_t>(matrix_bytes);
                    memcpy(matrix_data.data(), matrix_raw.data(), matrix_bytes);
                    
                    std::vector<float> b_data(matrix_size);
                    if (fdp.remaining_bytes() >= matrix_size * sizeof(float)) {
                        std::vector<uint8_t> b_raw = fdp.ConsumeBytes<uint8_t>(matrix_size * sizeof(float));
                        memcpy(b_data.data(), b_raw.data(), matrix_size * sizeof(float));
                    }
                    
                    switch (linear_alg_op % 2) {
                        case 0:
                            // LU decomposition (float)
                            {
                                int result = cv::hal::LU32f(
                                    matrix_data.data(), 
                                    matrix_size * sizeof(float),  // astep
                                    matrix_size, 
                                    b_data.data(), 
                                    sizeof(float),  // bstep
                                    1  // n (single RHS)
                                );
                            }
                            break;
                            
                        case 1:
                            // Cholesky decomposition (float)
                            {
                                bool result = cv::hal::Cholesky32f(
                                    matrix_data.data(), 
                                    matrix_size * sizeof(float),  // astep
                                    matrix_size, 
                                    b_data.data(), 
                                    sizeof(float),  // bstep
                                    1  // n (single RHS)
                                );
                            }
                            break;
                    }
                }
            }
        }
        
        // Part 4: Highgui operations (waitKey)
        // Note: HighGUI functions may require GUI context which might not be available in fuzzing environment
        // We'll try to create a simple window if possible, but handle gracefully if not
        if (fdp.remaining_bytes() >= 4) {
            int wait_time = fdp.ConsumeIntegralInRange<int>(0, 100);  // ms to wait
            
            // Try to create a named window (might fail in headless environment)
            try {
                // Create a simple test image
                cv::Mat test_image(100, 100, CV_8UC3, cv::Scalar(0, 0, 0));
                
                // Draw something on it
                cv::rectangle(test_image, cv::Rect(10, 10, 80, 80), cv::Scalar(255, 0, 0), 2);
                cv::circle(test_image, cv::Point(50, 50), 30, cv::Scalar(0, 255, 0), -1);
                
                // Create window name from fuzzer input
                std::string window_name = "TestWindow_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                
                // Try to create window and wait for key
                cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
                cv::imshow(window_name, test_image);
                
                // Call waitKey with fuzzer-provided time
                int key_pressed = cv::waitKey(wait_time);
                
                // Destroy the window
                cv::destroyWindow(window_name);
                
            } catch (const cv::Exception& e) {
                // Ignore HighGUI errors - common in headless fuzzing environments
                // The function call itself exercises the code paths
            } catch (...) {
                // Catch any other exceptions
            }
        }
        
        // Part 5: Test with cv::Mat interface for the same operations
        if (fdp.remaining_bytes() >= 50) {
            int rows = fdp.ConsumeIntegralInRange<int>(2, 20);
            int cols = fdp.ConsumeIntegralInRange<int>(2, 20);
            bool use_float_mat = fdp.ConsumeBool();
            
            int mat_type = use_float_mat ? CV_32FC1 : CV_64FC1;
            size_t elem_size = use_float_mat ? sizeof(float) : sizeof(double);
            size_t mat_bytes = rows * cols * elem_size;
            
            if (fdp.remaining_bytes() >= mat_bytes) {
                std::vector<uint8_t> mat_data = fdp.ConsumeBytes<uint8_t>(mat_bytes);
                cv::Mat test_mat(rows, cols, mat_type, mat_data.data());
                
                // Test matrix operations that might use HAL functions internally
                if (!test_mat.empty()) {
                    cv::Mat result_mat;
                    
                    // Test sqrt (uses HAL sqrt internally)
                    cv::sqrt(test_mat, result_mat);
                    
                    // Test exp (uses HAL exp internally)
                    cv::exp(test_mat, result_mat);
                    
                    // Test log (uses HAL log internally)
                    cv::log(test_mat, result_mat);
                }
            }
        }
        
    } catch (const cv::Exception& e) {
        // Catch OpenCV exceptions and continue
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_006
