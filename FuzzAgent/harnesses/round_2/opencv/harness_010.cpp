#include <cstddef>
#include <cstdint>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv4/opencv2/core/hal/hal.hpp>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_010 {
// OpenCV fuzzing harness targeting HAL mathematical operations
// Focuses on core mathematical functions in HAL module which have 30+ undiscovered branches each
// APIs targeted: cv::hal::exp32f, cv::hal::exp64f, cv::hal::log32f, cv::hal::log64f,
//                cv::hal::sqrt32f, cv::hal::sqrt64f, cv::hal::invSqrt32f, cv::hal::invSqrt64f
// Strategy: Allocate input/output arrays of varying sizes, fill with random data including
//           edge cases (zeros, negatives, very small/large values), call mathematical functions,
//           and clean up arrays.
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for array sizes and operation selection
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ===== 1. Determine array size and operation parameters =====
    // Array length from fuzzer input (keep reasonable to avoid excessive memory)
    int array_len = fdp.ConsumeIntegralInRange<int>(1, 1024);
    
    // Determine data type: float or double
    bool use_float = fdp.ConsumeBool();
    
    // Determine which mathematical operations to test
    bool test_exp = fdp.ConsumeBool();
    bool test_log = fdp.ConsumeBool();
    bool test_sqrt = fdp.ConsumeBool();
    bool test_invsqrt = fdp.ConsumeBool();
    
    // If no operations selected, test all
    if (!test_exp && !test_log && !test_sqrt && !test_invsqrt) {
        test_exp = test_log = test_sqrt = test_invsqrt = true;
    }
    
    // ===== 2. Allocate and initialize input arrays =====
    // Create input arrays with random values including edge cases
    if (use_float) {
        // Float arrays
        std::vector<float> src_float(array_len);
        std::vector<float> dst_float(array_len);
        
        // Fill input array with various edge cases and random values
        for (int i = 0; i < array_len; ++i) {
            // Choose different edge cases based on fuzzer input
            uint8_t case_selector = fdp.ConsumeIntegral<uint8_t>() % 8;
            float value;
            
            switch (case_selector) {
                case 0:  // Zero
                    value = 0.0f;
                    break;
                case 1:  // Very small positive value
                    value = fdp.ConsumeFloatingPointInRange<float>(1e-38f, 1e-20f);
                    break;
                case 2:  // Small positive value
                    value = fdp.ConsumeFloatingPointInRange<float>(1e-20f, 1.0f);
                    break;
                case 3:  // Medium positive value
                    value = fdp.ConsumeFloatingPointInRange<float>(1.0f, 1000.0f);
                    break;
                case 4:  // Large positive value
                    value = fdp.ConsumeFloatingPointInRange<float>(1000.0f, 1e20f);
                    break;
                case 5:  // Very large positive value
                    value = fdp.ConsumeFloatingPointInRange<float>(1e20f, std::numeric_limits<float>::max());
                    break;
                case 6:  // Negative value (for functions that may handle them)
                    value = fdp.ConsumeFloatingPointInRange<float>(-1000.0f, -1e-20f);
                    break;
                case 7:  // Random value in full range
                    value = fdp.ConsumeFloatingPoint<float>();
                    break;
                default:
                    value = 0.0f;
            }
            
            src_float[i] = value;
        }
        
        // ===== 3. Call HAL mathematical functions =====
        
        // Test exp32f (exponential function)
        if (test_exp) {
            cv::hal::exp32f(src_float.data(), dst_float.data(), array_len);
        }
        
        // Test log32f (natural logarithm)
        if (test_log) {
            // Create a copy with only positive values for log (log of non-positive may cause issues)
            std::vector<float> src_log(array_len);
            for (int i = 0; i < array_len; ++i) {
                src_log[i] = std::abs(src_float[i]) + std::numeric_limits<float>::min();
            }
            cv::hal::log32f(src_log.data(), dst_float.data(), array_len);
        }
        
        // Test sqrt32f (square root)
        if (test_sqrt) {
            // Create a copy with only non-negative values for sqrt
            std::vector<float> src_sqrt(array_len);
            for (int i = 0; i < array_len; ++i) {
                src_sqrt[i] = std::abs(src_float[i]);
            }
            cv::hal::sqrt32f(src_sqrt.data(), dst_float.data(), array_len);
        }
        
        // Test invSqrt32f (inverse square root)
        if (test_invsqrt) {
            // Create a copy with only positive values for invSqrt
            std::vector<float> src_invsqrt(array_len);
            for (int i = 0; i < array_len; ++i) {
                src_invsqrt[i] = std::abs(src_float[i]) + std::numeric_limits<float>::min();
            }
            cv::hal::invSqrt32f(src_invsqrt.data(), dst_float.data(), array_len);
        }
        
    } else {
        // Double arrays
        std::vector<double> src_double(array_len);
        std::vector<double> dst_double(array_len);
        
        // Fill input array with various edge cases and random values
        for (int i = 0; i < array_len; ++i) {
            // Choose different edge cases based on fuzzer input
            uint8_t case_selector = fdp.ConsumeIntegral<uint8_t>() % 8;
            double value;
            
            switch (case_selector) {
                case 0:  // Zero
                    value = 0.0;
                    break;
                case 1:  // Very small positive value
                    value = fdp.ConsumeFloatingPointInRange<double>(1e-308, 1e-100);
                    break;
                case 2:  // Small positive value
                    value = fdp.ConsumeFloatingPointInRange<double>(1e-100, 1.0);
                    break;
                case 3:  // Medium positive value
                    value = fdp.ConsumeFloatingPointInRange<double>(1.0, 1000.0);
                    break;
                case 4:  // Large positive value
                    value = fdp.ConsumeFloatingPointInRange<double>(1000.0, 1e100);
                    break;
                case 5:  // Very large positive value
                    value = fdp.ConsumeFloatingPointInRange<double>(1e100, std::numeric_limits<double>::max());
                    break;
                case 6:  // Negative value (for functions that may handle them)
                    value = fdp.ConsumeFloatingPointInRange<double>(-1000.0, -1e-100);
                    break;
                case 7:  // Random value in full range
                    value = fdp.ConsumeFloatingPoint<double>();
                    break;
                default:
                    value = 0.0;
            }
            
            src_double[i] = value;
        }
        
        // ===== 3. Call HAL mathematical functions =====
        
        // Test exp64f (exponential function)
        if (test_exp) {
            cv::hal::exp64f(src_double.data(), dst_double.data(), array_len);
        }
        
        // Test log64f (natural logarithm)
        if (test_log) {
            // Create a copy with only positive values for log
            std::vector<double> src_log(array_len);
            for (int i = 0; i < array_len; ++i) {
                src_log[i] = std::abs(src_double[i]) + std::numeric_limits<double>::min();
            }
            cv::hal::log64f(src_log.data(), dst_double.data(), array_len);
        }
        
        // Test sqrt64f (square root)
        if (test_sqrt) {
            // Create a copy with only non-negative values for sqrt
            std::vector<double> src_sqrt(array_len);
            for (int i = 0; i < array_len; ++i) {
                src_sqrt[i] = std::abs(src_double[i]);
            }
            cv::hal::sqrt64f(src_sqrt.data(), dst_double.data(), array_len);
        }
        
        // Test invSqrt64f (inverse square root)
        if (test_invsqrt) {
            // Create a copy with only positive values for invSqrt
            std::vector<double> src_invsqrt(array_len);
            for (int i = 0; i < array_len; ++i) {
                src_invsqrt[i] = std::abs(src_double[i]) + std::numeric_limits<double>::min();
            }
            cv::hal::invSqrt64f(src_invsqrt.data(), dst_double.data(), array_len);
        }
    }
    
    // ===== 4. Arrays automatically cleaned up when vectors go out of scope =====
    
    return 0;
}

} // namespace harness_010
