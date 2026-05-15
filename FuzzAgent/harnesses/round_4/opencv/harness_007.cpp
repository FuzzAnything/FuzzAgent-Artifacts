#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core/hal/hal.hpp>

namespace harness_007 {
// OpenCV HAL mathematical functions fuzzing harness
// Targets: cv::hal mathematical functions (sqrt32f, invSqrt32f, polarToCart, cartToPolar, fastAtan32f)
// harness_007.cpp - Targets low-level HAL functions with raw pointer arrays
// Differentiates from harness_001, harness_003, harness_004 by using direct HAL API calls
// instead of high-level cv::Mat-based functions


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic array operations
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Determine test scenario from fuzzer input
        uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (scenario) {
            case 0: {
                // Scenario 0: Test sqrt32f and invSqrt32f with float arrays
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 100);
                if (fdp.remaining_bytes() < static_cast<size_t>(array_size * sizeof(float))) {
                    return 0;
                }
                
                std::vector<float> src_array(array_size);
                std::vector<float> dst_array(array_size);
                
                // Fill source array with fuzzer data
                for (int i = 0; i < array_size; ++i) {
                    src_array[i] = fdp.ConsumeFloatingPoint<float>();
                }
                
                // Test sqrt32f
                cv::hal::sqrt32f(src_array.data(), dst_array.data(), array_size);
                
                // Test invSqrt32f (avoid division by zero)
                std::vector<float> src_positive(array_size);
                for (int i = 0; i < array_size; ++i) {
                    float val = std::abs(src_array[i]);
                    if (val == 0.0f) val = 1.0f; // Avoid zero
                    src_positive[i] = val;
                }
                cv::hal::invSqrt32f(src_positive.data(), dst_array.data(), array_size);
                break;
            }
            
            case 1: {
                // Scenario 1: Test polar/cartesian transformations with float arrays
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 50);
                size_t required_bytes = array_size * sizeof(float) * 4; // x, y, mag, angle
                if (fdp.remaining_bytes() < required_bytes) {
                    return 0;
                }
                
                std::vector<float> x_array(array_size);
                std::vector<float> y_array(array_size);
                std::vector<float> mag_array(array_size);
                std::vector<float> angle_array(array_size);
                std::vector<float> x_result(array_size);
                std::vector<float> y_result(array_size);
                
                // Fill arrays with fuzzer data
                for (int i = 0; i < array_size; ++i) {
                    x_array[i] = fdp.ConsumeFloatingPoint<float>();
                    y_array[i] = fdp.ConsumeFloatingPoint<float>();
                }
                
                bool angle_in_degrees = fdp.ConsumeBool();
                
                // Test cartToPolar32f
                cv::hal::cartToPolar32f(x_array.data(), y_array.data(), 
                                       mag_array.data(), angle_array.data(), 
                                       array_size, angle_in_degrees);
                
                // Test polarToCart32f (round trip)
                cv::hal::polarToCart32f(mag_array.data(), angle_array.data(),
                                       x_result.data(), y_result.data(),
                                       array_size, angle_in_degrees);
                
                // Test fastAtan32f
                cv::hal::fastAtan32f(y_array.data(), x_array.data(),
                                   angle_array.data(), array_size, angle_in_degrees);
                break;
            }
            
            case 2: {
                // Scenario 2: Test sqrt64f and invSqrt64f with double arrays
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 50);
                if (fdp.remaining_bytes() < static_cast<size_t>(array_size * sizeof(double))) {
                    return 0;
                }
                
                std::vector<double> src_array(array_size);
                std::vector<double> dst_array(array_size);
                
                // Fill source array with fuzzer data
                for (int i = 0; i < array_size; ++i) {
                    src_array[i] = fdp.ConsumeFloatingPoint<double>();
                }
                
                // Test sqrt64f
                cv::hal::sqrt64f(src_array.data(), dst_array.data(), array_size);
                
                // Test invSqrt64f (avoid division by zero)
                std::vector<double> src_positive(array_size);
                for (int i = 0; i < array_size; ++i) {
                    double val = std::abs(src_array[i]);
                    if (val == 0.0) val = 1.0; // Avoid zero
                    src_positive[i] = val;
                }
                cv::hal::invSqrt64f(src_positive.data(), dst_array.data(), array_size);
                break;
            }
            
            case 3: {
                // Scenario 3: Test polar/cartesian transformations with double arrays
                int array_size = fdp.ConsumeIntegralInRange<int>(1, 25);
                size_t required_bytes = array_size * sizeof(double) * 4; // x, y, mag, angle
                if (fdp.remaining_bytes() < required_bytes) {
                    return 0;
                }
                
                std::vector<double> x_array(array_size);
                std::vector<double> y_array(array_size);
                std::vector<double> mag_array(array_size);
                std::vector<double> angle_array(array_size);
                std::vector<double> x_result(array_size);
                std::vector<double> y_result(array_size);
                
                // Fill arrays with fuzzer data
                for (int i = 0; i < array_size; ++i) {
                    x_array[i] = fdp.ConsumeFloatingPoint<double>();
                    y_array[i] = fdp.ConsumeFloatingPoint<double>();
                }
                
                bool angle_in_degrees = fdp.ConsumeBool();
                
                // Test cartToPolar64f
                cv::hal::cartToPolar64f(x_array.data(), y_array.data(), 
                                       mag_array.data(), angle_array.data(), 
                                       array_size, angle_in_degrees);
                
                // Test polarToCart64f (round trip)
                cv::hal::polarToCart64f(mag_array.data(), angle_array.data(),
                                       x_result.data(), y_result.data(),
                                       array_size, angle_in_degrees);
                
                // Test fastAtan64f
                cv::hal::fastAtan64f(y_array.data(), x_array.data(),
                                   angle_array.data(), array_size, angle_in_degrees);
                break;
            }
            
            case 4: {
                // Scenario 4: Edge cases and special values
                int array_size = fdp.ConsumeIntegralInRange<int>(0, 10); // Allow zero size
                
                if (array_size > 0) {
                    if (fdp.remaining_bytes() < static_cast<size_t>(array_size * sizeof(float))) {
                        return 0;
                    }
                    
                    std::vector<float> src_array(array_size);
                    std::vector<float> dst_array(array_size);
                    
                    // Fill with potentially special values
                    for (int i = 0; i < array_size; ++i) {
                        uint8_t special_case = fdp.ConsumeIntegral<uint8_t>() % 4;
                        switch (special_case) {
                            case 0: src_array[i] = 0.0f; break;
                            case 1: src_array[i] = 1.0f; break;
                            case 2: src_array[i] = -1.0f; break;
                            case 3: src_array[i] = fdp.ConsumeFloatingPoint<float>(); break;
                        }
                    }
                    
                    // Test with edge cases
                    cv::hal::sqrt32f(src_array.data(), dst_array.data(), array_size);
                    
                    // For invSqrt, avoid zero
                    std::vector<float> src_safe(array_size);
                    for (int i = 0; i < array_size; ++i) {
                        float val = src_array[i];
                        if (val == 0.0f) val = 1.0f;
                        src_safe[i] = val;
                    }
                    cv::hal::invSqrt32f(src_safe.data(), dst_array.data(), array_size);
                }
                break;
            }
        }
        
    } catch (...) {
        // Catch any exceptions and continue
    }
    
    return 0;
}

} // namespace harness_007
