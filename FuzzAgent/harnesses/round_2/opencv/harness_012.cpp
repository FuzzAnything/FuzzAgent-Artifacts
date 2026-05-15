#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_012 {
// OpenCV fuzzing harness targeting mathematical operations in core module
// Specifically targets: cv::magnitude, cv::cartToPolar, cv::polarToCart, cv::sqrt, cv::exp, cv::log
// These functions have 30+ undiscovered branches each and are currently 0% covered
// Focus: vector mathematics and coordinate transformations with semantic diversity from existing harnesses

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for matrix dimensions and content
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // ===== 1. Determine matrix parameters from fuzzer input =====
        int rows = fdp.ConsumeIntegralInRange<int>(1, 100);
        int cols = fdp.ConsumeIntegralInRange<int>(1, 100);
        
        // Determine matrix type (single or double precision floating point)
        uint8_t type_selector = fdp.ConsumeIntegral<uint8_t>() % 2;
        int mat_type;
        switch (type_selector) {
            case 0: mat_type = CV_32F; break;  // 32-bit floating point
            case 1: mat_type = CV_64F; break;  // 64-bit floating point
            default: mat_type = CV_32F;
        }
        
        // ===== 2. Create input matrices x and y with random data =====
        cv::Mat x_mat(rows, cols, mat_type);
        cv::Mat y_mat(rows, cols, mat_type);
        
        // Calculate required data size for both matrices
        size_t elem_size = (mat_type == CV_32F) ? sizeof(float) : sizeof(double);
        size_t total_elements = rows * cols;
        size_t total_data_size = total_elements * elem_size * 2; // For both x and y
        if (fdp.remaining_bytes() >= total_data_size) {
            // Fill x_mat with random data - use uint8_t for all types
            std::vector<uint8_t> x_data = fdp.ConsumeBytes<uint8_t>(total_data_size / 2);
            if (x_data.size() == total_data_size / 2) {
                memcpy(x_mat.data, x_data.data(), total_data_size / 2);
            }
            
            // Fill y_mat with random data - use uint8_t for all types
            std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(total_data_size / 2);
            if (y_data.size() == total_data_size / 2) {
                memcpy(y_mat.data, y_data.data(), total_data_size / 2);
            }
        } else {
            // If not enough data, fill with small random values and continue
            cv::randu(x_mat, cv::Scalar::all(-10.0), cv::Scalar::all(10.0));
            cv::randu(y_mat, cv::Scalar::all(-10.0), cv::Scalar::all(10.0));
        }
        
        // ===== 3. Target API 1: cv::magnitude =====
        // Calculates magnitude of 2D vectors: dst = sqrt(x² + y²)
        cv::Mat magnitude_result;
        cv::magnitude(x_mat, y_mat, magnitude_result);
        
        // ===== 4. Target API 2: cv::cartToPolar =====
        // Converts Cartesian coordinates to polar (magnitude and angle)
        cv::Mat cart_magnitude, cart_angle;
        cv::cartToPolar(x_mat, y_mat, cart_magnitude, cart_angle, false); // angle in radians
        
        // Also test with angle in degrees
        cv::Mat cart_magnitude_deg, cart_angle_deg;
        cv::cartToPolar(x_mat, y_mat, cart_magnitude_deg, cart_angle_deg, true); // angle in degrees
        
        // ===== 5. Target API 3: cv::polarToCart =====
        // Convert polar coordinates back to Cartesian (round-trip test)
        cv::Mat polar_x, polar_y;
        cv::polarToCart(cart_magnitude, cart_angle, polar_x, polar_y, false); // angle in radians
        
        // Also test with angle in degrees
        cv::Mat polar_x_deg, polar_y_deg;
        cv::polarToCart(cart_magnitude_deg, cart_angle_deg, polar_x_deg, polar_y_deg, true);
        
        // ===== 6. Target API 4: cv::sqrt =====
        // Square root of magnitude array
        cv::Mat sqrt_result;
        cv::sqrt(magnitude_result, sqrt_result);
        
        // Also test sqrt on positive values from x_mat (abs value)
        cv::Mat x_abs = cv::abs(x_mat);
        cv::Mat sqrt_x;
        cv::sqrt(x_abs, sqrt_x);
        
        // ===== 7. Target API 5: cv::exp =====
        // Exponential function
        cv::Mat exp_result;
        
        // For exp, scale input to avoid overflow/underflow
        cv::Mat scaled_x = x_mat / 10.0;
        cv::exp(scaled_x, exp_result);
        
        // Also test exp on y_mat
        cv::Mat scaled_y = y_mat / 10.0;
        cv::Mat exp_y;
        cv::exp(scaled_y, exp_y);
        
        // ===== 8. Target API 6: cv::log =====
        // Natural logarithm
        cv::Mat log_result;
        
        // For log, ensure positive input values (use abs + epsilon)
        float epsilon = fdp.ConsumeFloatingPointInRange<float>(0.001f, 1.0f);
        cv::Mat pos_x = cv::abs(x_mat) + epsilon;
        cv::log(pos_x, log_result);
        
        // Also test log on positive y values
        cv::Mat pos_y = cv::abs(y_mat) + epsilon;
        cv::Mat log_y;
        cv::log(pos_y, log_y);
        
        // ===== 9. Test combinations and edge cases =====
        
        // Test with mixed precision (if input was float, convert to double and test)
        if (mat_type == CV_32F && fdp.ConsumeBool()) {
            cv::Mat x_double, y_double;
            x_mat.convertTo(x_double, CV_64F);
            y_mat.convertTo(y_double, CV_64F);
            
            cv::Mat magnitude_double;
            cv::magnitude(x_double, y_double, magnitude_double);
            
            cv::Mat sqrt_double;
            cv::sqrt(magnitude_double, sqrt_double);
        }
        
        // Test with small matrices (edge case)
        if (rows > 1 && cols > 1) {
            cv::Mat x_sub = x_mat(cv::Rect(0, 0, std::min(10, cols), std::min(10, rows)));
            cv::Mat y_sub = y_mat(cv::Rect(0, 0, std::min(10, cols), std::min(10, rows)));
            
            if (!x_sub.empty() && !y_sub.empty()) {
                cv::Mat magnitude_sub;
                cv::magnitude(x_sub, y_sub, magnitude_sub);
            }
        }
        
        // Test with zeros (edge case)
        if (fdp.ConsumeBool()) {
            cv::Mat x_zero = cv::Mat::zeros(5, 5, mat_type);
            cv::Mat y_zero = cv::Mat::zeros(5, 5, mat_type);
            
            cv::Mat magnitude_zero;
            cv::magnitude(x_zero, y_zero, magnitude_zero);
            
            cv::Mat cart_mag_zero, cart_ang_zero;
            cv::cartToPolar(x_zero, y_zero, cart_mag_zero, cart_ang_zero);
        }
        
        // Test with large values
        if (fdp.ConsumeBool()) {
            cv::Mat x_large = x_mat * 1000.0;
            cv::Mat y_large = y_mat * 1000.0;
            
            cv::Mat magnitude_large;
            cv::magnitude(x_large, y_large, magnitude_large);
        }
        
        // ===== 10. Cleanup (automatic via RAII) =====
        // All cv::Mat objects automatically clean up when they go out of scope
        
    } catch (const cv::Exception& e) {
        // Silently catch OpenCV exceptions during fuzzing
        // This allows the fuzzer to continue with other inputs
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_012
