#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/core/hal/hal.hpp>
#include <opencv4/opencv2/core/utils/logger.hpp>

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - we need enough data for array dimensions and values
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize OpenCV logging system
    // Consume logging level from fuzzer input
    uint8_t log_level_val = fdp.ConsumeIntegral<uint8_t>() % 7; // 0-6 for LogLevel enum
    cv::utils::logging::LogLevel log_level = static_cast<cv::utils::logging::LogLevel>(log_level_val);
    
    // Set logging level
    cv::utils::logging::setLogLevel(log_level);
    
    // Step 2: Create test input arrays/matrices with various data types
    // Consume array size (limited to reasonable range)
    int array_size = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Create input arrays for double (64f) and float (32f) operations
    std::vector<double> src64(array_size);
    std::vector<double> dst64(array_size);
    std::vector<float> src32(array_size);
    std::vector<float> dst32(array_size);
    
    // Fill arrays with fuzzed data - use whatever data is available
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); i++) {
        src64[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    // Fill float array with remaining data or default values
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(float); i++) {
        src32[i] = fdp.ConsumeFloatingPoint<float>();
    }
    
    // If not enough data for all elements, remaining elements stay default-initialized (0.0)
    
    // Step 3: Call HAL mathematical functions
    
    // Target 1: cv::hal::sqrt64f (8 branches, 30 undiscovered accumulated branches)
    cv::hal::sqrt64f(src64.data(), dst64.data(), array_size);
    
    // Target 2: cv::hal::exp (both 32f and 64f versions)
    // Choose which exp function to test based on fuzzed input
    uint8_t exp_choice = fdp.ConsumeIntegral<uint8_t>() % 2;
    if (exp_choice == 0) {
        cv::hal::exp32f(src32.data(), dst32.data(), array_size);
    } else {
        cv::hal::exp64f(src64.data(), dst64.data(), array_size);
    }
    
    // Also test other related mathematical functions for broader coverage
    if (fdp.remaining_bytes() >= 4) {
        uint8_t extra_op = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (extra_op) {
            case 0:
                cv::hal::log32f(src32.data(), dst32.data(), array_size);
                break;
            case 1:
                cv::hal::log64f(src64.data(), dst64.data(), array_size);
                break;
            case 2:
                cv::hal::invSqrt32f(src32.data(), dst32.data(), array_size);
                break;
            case 3:
                cv::hal::invSqrt64f(src64.data(), dst64.data(), array_size);
                break;
        }
    }
    
    // Step 4: Verify results (basic sanity checks)
    // We'll do minimal verification to avoid crashes from invalid results
    for (int i = 0; i < array_size; i++) {
        // Check for NaN or infinity in results
        if (std::isnan(dst64[i]) || std::isinf(dst64[i])) {
            // This is acceptable in fuzzing - mathematical functions can produce these
        }
        if (i < array_size && (std::isnan(dst32[i]) || std::isinf(dst32[i]))) {
            // This is acceptable in fuzzing
        }
    }
    
    // Step 5: Call logging functions to target cv::utils::logging::internal::writeLogMessage
    // Generate a log message from fuzzer input
    if (fdp.remaining_bytes() > 0) {
        std::string log_message = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        
        // Call the internal writeLogMessage function
        // Note: This requires accessing the internal namespace
        // We'll use the public logging macros which internally call writeLogMessage
        // Generate log level for message
        uint8_t msg_log_level_val = fdp.ConsumeIntegral<uint8_t>() % 7;
        cv::utils::logging::LogLevel msg_log_level = static_cast<cv::utils::logging::LogLevel>(msg_log_level_val);
        
        // Use appropriate logging macro based on level
        switch (msg_log_level) {
            case cv::utils::logging::LOG_LEVEL_FATAL:
                CV_LOG_FATAL(nullptr, log_message.c_str());
                break;
            case cv::utils::logging::LOG_LEVEL_ERROR:
                CV_LOG_ERROR(nullptr, log_message.c_str());
                break;
            case cv::utils::logging::LOG_LEVEL_WARNING:
                CV_LOG_WARNING(nullptr, log_message.c_str());
                break;
            case cv::utils::logging::LOG_LEVEL_INFO:
                CV_LOG_INFO(nullptr, log_message.c_str());
                break;
            case cv::utils::logging::LOG_LEVEL_DEBUG:
                CV_LOG_DEBUG(nullptr, log_message.c_str());
                break;
            case cv::utils::logging::LOG_LEVEL_VERBOSE:
                CV_LOG_VERBOSE(nullptr, 0, log_message.c_str());
                break;
            default:
                // SILENT or unknown level - don't log
                break;
        }
    }
    
    // Step 6: Clean up resources
    // Arrays are automatically cleaned up when they go out of scope
    
    return 0;
}

} // namespace harness_001
