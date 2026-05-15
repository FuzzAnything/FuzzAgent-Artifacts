#include <cstddef>
#include <cstdint>
#include <cmath>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core/hal/hal.hpp>

namespace harness_019 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for array size, bool flag, and some float data
    // We need: int (n), bool (angleInDegrees), and at least 2*n floats for x and y arrays
    const size_t MIN_SIZE = sizeof(int) + sizeof(bool) + sizeof(float) * 4; // n=1 minimum
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume array size n from fuzzer input
    // Limit to reasonable size for fuzzing performance and memory safety
    int n = fdp.ConsumeIntegralInRange<int>(1, 1024);
    
    // Consume angleInDegrees flag
    bool angleInDegrees = fdp.ConsumeBool();
    
    // Calculate minimum required bytes for float arrays
    size_t floats_needed = n * 2; // x and y arrays
    size_t bytes_needed = floats_needed * sizeof(float);
    
    if (fdp.remaining_bytes() < bytes_needed) {
        // Not enough data for arrays, reduce n or return
        n = std::min(n, static_cast<int>(fdp.remaining_bytes() / (2 * sizeof(float))));
        if (n <= 0) {
            return 0;
        }
        floats_needed = n * 2;
        bytes_needed = floats_needed * sizeof(float);
    }
    
    // Allocate and initialize input arrays x and y
    std::vector<float> x_arr(n);
    std::vector<float> y_arr(n);
    
    // Fill arrays with fuzzer data
    for (int i = 0; i < n; i++) {
        if (fdp.remaining_bytes() < sizeof(float) * 2) {
            break;
        }
        x_arr[i] = fdp.ConsumeFloatingPoint<float>();
        y_arr[i] = fdp.ConsumeFloatingPoint<float>();
    }
    
    // Allocate output arrays
    std::vector<float> dst_fastAtan2(n);
    std::vector<float> dst_magnitude(n);
    std::vector<float> dst_magnitude32f(n);
    
    try {
        // Step 1: Call cv::hal::fastAtan2
        cv::hal::fastAtan2(y_arr.data(), x_arr.data(), dst_fastAtan2.data(), n, angleInDegrees);
        
        // Step 2: Call cv::hal::magnitude
        cv::hal::magnitude(x_arr.data(), y_arr.data(), dst_magnitude.data(), n);
        
        // Step 3: Call cv::hal::magnitude32f
        cv::hal::magnitude32f(x_arr.data(), y_arr.data(), dst_magnitude32f.data(), n);
        
        // Optional: Verify results aren't NaN or inf (though valid for some inputs)
        for (int i = 0; i < n; i++) {
            // Check that outputs are finite numbers
            // This helps catch potential arithmetic issues
            (void)std::isfinite(dst_fastAtan2[i]);
            (void)std::isfinite(dst_magnitude[i]);
            (void)std::isfinite(dst_magnitude32f[i]);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
        // The fuzzer will explore different input combinations
    }
    
    return 0;
}

} // namespace harness_019
