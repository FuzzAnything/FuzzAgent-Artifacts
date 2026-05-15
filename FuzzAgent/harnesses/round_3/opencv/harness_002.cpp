#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cfloat>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "opencv2/core/hal/hal.hpp"

namespace harness_002 {

// Include OpenCV HAL headers

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for at least basic parameters
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to test different mathematical functions
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume array size (n) - limit to reasonable size to avoid excessive memory
    int n = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Consume boolean flag for angleInDegrees (for cartToPolar functions)
    bool angleInDegrees = fdp.ConsumeBool();
    
    // Calculate minimum required bytes for this operation
    size_t required_floats = 0;
    size_t required_doubles = 0;
    
    if (operation == 0) { // cartToPolar32f
        required_floats = 4 * n; // x, y, mag, angle
    } else if (operation == 1) { // cartToPolar64f
        required_doubles = 4 * n; // x, y, mag, angle
    } else if (operation == 2) { // magnitude32f
        required_floats = 3 * n; // x, y, mag
    } else if (operation == 3) { // normL2Sqr_
        required_floats = 2 * n; // a, b
    }
    
    // Check if we have enough input data for the required arrays
    size_t required_bytes = required_floats * sizeof(float) + required_doubles * sizeof(double);
    if (fdp.remaining_bytes() < required_bytes) {
        return 0;
    }
    
    // Test the selected mathematical operation
    if (operation == 0) {
        // cartToPolar32f - float precision
        std::vector<float> x_f(n), y_f(n), mag_f(n), angle_f(n);
        
        // Fill input arrays with fuzzed data
        for (int i = 0; i < n; ++i) {
            x_f[i] = fdp.ConsumeFloatingPoint<float>();
            y_f[i] = fdp.ConsumeFloatingPoint<float>();
        }
        
        // Add edge cases based on fuzzer input
        if (n > 0 && fdp.ConsumeBool()) {
            int idx = fdp.ConsumeIntegralInRange<int>(0, n-1);
            // Choose edge case type from fuzzer
            uint8_t edge_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (edge_type) {
                case 0: x_f[idx] = 0.0f; y_f[idx] = 0.0f; break;
                case 1: 
                    x_f[idx] = fdp.ConsumeBool() ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    y_f[idx] = fdp.ConsumeBool() ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    break;
                case 2: 
                    x_f[idx] = std::numeric_limits<float>::quiet_NaN();
                    y_f[idx] = std::numeric_limits<float>::quiet_NaN();
                    break;
            }
        }
        
        // Call cartToPolar32f
        cv::hal::cartToPolar32f(x_f.data(), y_f.data(), mag_f.data(), angle_f.data(), n, angleInDegrees);
        
        // Basic sanity checks on outputs
        for (int i = 0; i < n; ++i) {
            if (std::isnan(mag_f[i]) || std::isinf(mag_f[i])) {
                // Acceptable for edge cases
            }
            if (std::isnan(angle_f[i]) || std::isinf(angle_f[i])) {
                // Acceptable for edge cases
            }
        }
    }
    else if (operation == 1) {
        // cartToPolar64f - double precision
        std::vector<double> x_d(n), y_d(n), mag_d(n), angle_d(n);
        
        // Fill input arrays with fuzzed data
        for (int i = 0; i < n; ++i) {
            x_d[i] = fdp.ConsumeFloatingPoint<double>();
            y_d[i] = fdp.ConsumeFloatingPoint<double>();
        }
        
        // Add edge cases based on fuzzer input
        if (n > 0 && fdp.ConsumeBool()) {
            int idx = fdp.ConsumeIntegralInRange<int>(0, n-1);
            // Choose edge case type from fuzzer
            uint8_t edge_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (edge_type) {
                case 0: x_d[idx] = 0.0; y_d[idx] = 0.0; break;
                case 1: 
                    x_d[idx] = fdp.ConsumeBool() ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
                    y_d[idx] = fdp.ConsumeBool() ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
                    break;
                case 2: 
                    x_d[idx] = std::numeric_limits<double>::quiet_NaN();
                    y_d[idx] = std::numeric_limits<double>::quiet_NaN();
                    break;
            }
        }
        
        // Call cartToPolar64f
        cv::hal::cartToPolar64f(x_d.data(), y_d.data(), mag_d.data(), angle_d.data(), n, angleInDegrees);
        
        // Basic sanity checks on outputs
        for (int i = 0; i < n; ++i) {
            if (std::isnan(mag_d[i]) || std::isinf(mag_d[i])) {
                // Acceptable for edge cases
            }
            if (std::isnan(angle_d[i]) || std::isinf(angle_d[i])) {
                // Acceptable for edge cases
            }
        }
    }
    else if (operation == 2) {
        // magnitude32f operation
        std::vector<float> x_f(n), y_f(n), mag_f(n);
        
        // Fill input arrays with fuzzed data
        for (int i = 0; i < n; ++i) {
            x_f[i] = fdp.ConsumeFloatingPoint<float>();
            y_f[i] = fdp.ConsumeFloatingPoint<float>();
        }
        
        // Add edge cases based on fuzzer input
        if (n > 0 && fdp.ConsumeBool()) {
            int idx = fdp.ConsumeIntegralInRange<int>(0, n-1);
            // Choose edge case type from fuzzer
            uint8_t edge_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (edge_type) {
                case 0: x_f[idx] = 0.0f; y_f[idx] = 0.0f; break;
                case 1: 
                    x_f[idx] = fdp.ConsumeBool() ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    y_f[idx] = fdp.ConsumeBool() ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    break;
                case 2: 
                    x_f[idx] = std::numeric_limits<float>::quiet_NaN();
                    y_f[idx] = std::numeric_limits<float>::quiet_NaN();
                    break;
            }
        }
        
        // Call magnitude32f
        cv::hal::magnitude32f(x_f.data(), y_f.data(), mag_f.data(), n);
        
        // Basic sanity checks on outputs
        for (int i = 0; i < n; ++i) {
            if (std::isnan(mag_f[i]) || std::isinf(mag_f[i])) {
                // Acceptable for edge cases
            }
        }
    }
    else if (operation == 3) {
        // normL2Sqr_ operation
        std::vector<float> a_f(n), b_f(n);
        
        // Fill input arrays with fuzzed data
        for (int i = 0; i < n; ++i) {
            a_f[i] = fdp.ConsumeFloatingPoint<float>();
            b_f[i] = fdp.ConsumeFloatingPoint<float>();
        }
        
        // Add edge cases based on fuzzer input
        if (n > 0 && fdp.ConsumeBool()) {
            int idx = fdp.ConsumeIntegralInRange<int>(0, n-1);
            // Choose edge case type from fuzzer
            uint8_t edge_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (edge_type) {
                case 0: a_f[idx] = 0.0f; b_f[idx] = 0.0f; break;
                case 1: 
                    a_f[idx] = fdp.ConsumeBool() ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    b_f[idx] = fdp.ConsumeBool() ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    break;
                case 2: 
                    a_f[idx] = std::numeric_limits<float>::quiet_NaN();
                    b_f[idx] = std::numeric_limits<float>::quiet_NaN();
                    break;
            }
        }
        
        // Call normL2Sqr_
        float result = cv::hal::normL2Sqr_(a_f.data(), b_f.data(), n);
        
        // Basic sanity check on result
        if (std::isnan(result) || std::isinf(result)) {
            // Acceptable for edge cases
        }
    }
    
    return 0;
}

} // namespace harness_002
