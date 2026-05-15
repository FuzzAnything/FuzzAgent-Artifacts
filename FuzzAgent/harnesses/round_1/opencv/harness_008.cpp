#include <cstddef>
#include <cstdint>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv4/opencv2/core/hal/hal.hpp>

namespace harness_008 {

// Maximum array size to prevent excessive memory usage
constexpr size_t MAX_ARRAY_SIZE = 1024;
// Helper function to sanitize double values
double sanitize_double(double value) {
    if (!std::isfinite(value)) {
        return 0.0;
    }
    return value;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Step 1: Determine array size with reasonable limits
        int array_size = fdp.ConsumeIntegralInRange<int>(1, MAX_ARRAY_SIZE);
        
        // Step 2: Check if we have enough data for the arrays
        size_t bytes_needed = array_size * sizeof(double) * 3; // x, y, mag arrays
        if (fdp.remaining_bytes() < bytes_needed + 16) {
            return 0; // Not enough data
        }
        
        // Step 3: Allocate input arrays
        std::vector<double> x_array(array_size);
        std::vector<double> y_array(array_size);
        std::vector<double> magnitude_result(array_size);
        std::vector<double> log_result(array_size);
        
        // Step 4: Fill arrays with fuzzed data
        for (int i = 0; i < array_size; ++i) {
            // Consume and sanitize x and y values
            double x_val = fdp.ConsumeFloatingPoint<double>();
            double y_val = fdp.ConsumeFloatingPoint<double>();
            
            x_array[i] = sanitize_double(x_val);
            y_array[i] = sanitize_double(y_val);
        }
        
        // Step 5: Test HAL magnitude64f function (double precision magnitude)
        cv::hal::magnitude64f(x_array.data(), y_array.data(), magnitude_result.data(), array_size);
        
        // Step 6: Test HAL generic magnitude interface (calls magnitude64f internally)
        cv::hal::magnitude(x_array.data(), y_array.data(), magnitude_result.data(), array_size);
        
        // Step 7: Prepare input for log function (need positive values for log)
        std::vector<double> log_input(array_size);
        for (int i = 0; i < array_size && fdp.remaining_bytes() > 8; ++i) {
            double val = fdp.ConsumeFloatingPoint<double>();
            // Ensure positive values for log (log of non-positive is undefined)
            val = std::abs(val);
            if (val == 0.0) val = 1.0; // Avoid log(0)
            log_input[i] = sanitize_double(val);
        }
        
        // Step 8: Test HAL log64f function (double precision logarithm)
        cv::hal::log64f(log_input.data(), log_result.data(), array_size);
        
        // Step 9: Test HAL generic log interface (calls log64f internally)
        cv::hal::log(log_input.data(), log_result.data(), array_size);
        
        // Step 10: Test error handling paths with edge cases using fuzzed data
        if (fdp.remaining_bytes() > 16) {
            uint8_t edge_case = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (edge_case) {
                case 0: {
                    // Test with array size = 0
                    if (array_size > 0) {
                        cv::hal::magnitude64f(x_array.data(), y_array.data(), magnitude_result.data(), 0);
                    }
                    break;
                }
                    
                case 1: {
                    // Test with small arrays (1-3 elements) using existing data
                    int small_size = std::min(array_size, 3);
                    cv::hal::magnitude64f(x_array.data(), y_array.data(), magnitude_result.data(), small_size);
                    break;
                }
                    
                case 2: {
                    // Test with potentially problematic values using fuzzed data
                    if (array_size > 0 && fdp.remaining_bytes() >= 24) {
                        double temp_x[3];
                        double temp_y[3];
                        double temp_result[3];
                        
                        // Use fuzzed data instead of hardcoded values
                        for (int i = 0; i < 3; i++) {
                            temp_x[i] = sanitize_double(fdp.ConsumeFloatingPoint<double>());
                            temp_y[i] = sanitize_double(fdp.ConsumeFloatingPoint<double>());
                        }
                        
                        cv::hal::magnitude64f(temp_x, temp_y, temp_result, 3);
                    }
                    break;
                }
                    
                case 3: {
                    // Test log with edge values using fuzzed data
                    if (array_size > 0 && fdp.remaining_bytes() >= 24) {
                        double temp_input[3];
                        double temp_log_result[3];
                        
                        // Use fuzzed data and ensure positive values
                        for (int i = 0; i < 3; i++) {
                            double val = fdp.ConsumeFloatingPoint<double>();
                            val = std::abs(val);
                            if (val == 0.0) val = 1.0;
                            temp_input[i] = sanitize_double(val);
                        }
                        
                        cv::hal::log64f(temp_input, temp_log_result, 3);
                    }
                    break;
                }
            }
        }
        
    } catch (...) {
        // Catch any unexpected exceptions
    }
    
    return 0;
}

} // namespace harness_008
