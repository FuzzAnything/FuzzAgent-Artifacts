#include <cstddef>
#include <cstdint>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core/hal/hal.hpp>

namespace harness_012 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for array size, flags, and some float data
    // We need: array size (int) + operation selector (uint8_t) + angle flag (bool) + at least some double data
    if (size < sizeof(int) + sizeof(uint8_t) + sizeof(bool) + sizeof(double) * 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume array size for coordinate operations (limit to reasonable size)
    int array_size = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Consume operation selector to test different function combinations
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume boolean flag for angleInDegrees (used by polar/cart and fastAtan functions)
    bool angle_in_degrees = fdp.ConsumeBool();
    
    // Create input arrays for 64-bit coordinate operations (double precision)
    std::vector<double> x_coords(array_size);
    std::vector<double> y_coords(array_size);
    std::vector<double> magnitudes(array_size);
    std::vector<double> angles(array_size);
    
    // Fill input arrays with fuzzed data
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); ++i) {
        x_coords[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); ++i) {
        y_coords[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); ++i) {
        magnitudes[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); ++i) {
        angles[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    // Create output arrays
    std::vector<double> out_magnitudes(array_size);
    std::vector<double> out_angles(array_size);
    std::vector<double> out_x_coords(array_size);
    std::vector<double> out_y_coords(array_size);
    std::vector<double> fast_atan_output(array_size);
    std::vector<double> magnitude_output(array_size);
    std::vector<double> inv_sqrt_output(array_size);
    
    // Get raw pointers for HAL function calls
    const double* x_ptr = x_coords.data();
    const double* y_ptr = y_coords.data();
    const double* mag_ptr = magnitudes.data();
    const double* angle_ptr = angles.data();
    double* out_mag_ptr = out_magnitudes.data();
    double* out_angle_ptr = out_angles.data();
    double* out_x_ptr = out_x_coords.data();
    double* out_y_ptr = out_y_coords.data();
    double* fast_atan_ptr = fast_atan_output.data();
    double* magnitude_ptr = magnitude_output.data();
    double* inv_sqrt_ptr = inv_sqrt_output.data();
    
    // Test different coordinate transformation operations
    switch (operation) {
        case 0:  // Cartesian to Polar and back (round-trip test)
            if (x_ptr != nullptr && y_ptr != nullptr && out_mag_ptr != nullptr && out_angle_ptr != nullptr) {
                // Convert Cartesian to Polar coordinates
                cv::hal::cartToPolar64f(x_ptr, y_ptr, out_mag_ptr, out_angle_ptr, array_size, angle_in_degrees);
                
                // Convert back from Polar to Cartesian
                cv::hal::polarToCart64f(out_mag_ptr, out_angle_ptr, out_x_ptr, out_y_ptr, array_size, angle_in_degrees);
            }
            break;
            
        case 1:  // Fast Atan2 computation
            if (y_ptr != nullptr && x_ptr != nullptr && fast_atan_ptr != nullptr) {
                // Compute fast atan2(y, x) - note: parameters are (y, x) not (x, y)
                cv::hal::fastAtan64f(y_ptr, x_ptr, fast_atan_ptr, array_size, angle_in_degrees);
            }
            break;
            
        case 2:  // Magnitude computation
            if (x_ptr != nullptr && y_ptr != nullptr && magnitude_ptr != nullptr) {
                // Compute magnitude of (x, y) vectors: sqrt(x^2 + y^2)
                cv::hal::magnitude64f(x_ptr, y_ptr, magnitude_ptr, array_size);
            }
            break;
            
        case 3:  // Inverse square root and combination
            if (mag_ptr != nullptr && inv_sqrt_ptr != nullptr) {
                // Compute inverse square root: 1.0 / sqrt(magnitude)
                cv::hal::invSqrt64f(mag_ptr, inv_sqrt_ptr, array_size);
                
                // Also test magnitude on the same input
                if (x_ptr != nullptr && y_ptr != nullptr && magnitude_ptr != nullptr) {
                    cv::hal::magnitude64f(x_ptr, y_ptr, magnitude_ptr, array_size);
                }
            }
            break;
    }
    
    // Optional: Validate outputs for consistency in case 0 (round-trip)
    if (operation == 0) {
        // Simple validation: check that round-trip conversion produces reasonable values
        // (not NaN or infinity)
        for (int i = 0; i < array_size; ++i) {
            if (std::isnan(out_x_coords[i]) || std::isinf(out_x_coords[i]) ||
                std::isnan(out_y_coords[i]) || std::isinf(out_y_coords[i])) {
                // Invalid output - but we don't crash, just continue
            }
        }
    }
    
    return 0;
}

} // namespace harness_012
