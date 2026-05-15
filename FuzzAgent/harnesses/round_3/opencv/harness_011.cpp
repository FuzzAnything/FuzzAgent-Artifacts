#include <cstddef>
#include <cstdint>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/hal.hpp>

namespace harness_011 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for basic parameters and at least some data
    // We need: array size (int) + operation type (uint8_t) + at least some float/double data
    if (size < sizeof(int) + sizeof(uint8_t) + sizeof(float) * 4) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume array size for mathematical operations (limit to reasonable size)
    int array_size = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Consume operation type to determine which mathematical function to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume boolean flag for angleInDegrees (used by some functions)
    bool angle_in_degrees = fdp.ConsumeBool();
    
    // Create input arrays for 32-bit and 64-bit operations
    std::vector<float> float_input(array_size);
    std::vector<double> double_input(array_size);
    std::vector<float> float_input2(array_size);  // For functions needing two inputs
    std::vector<double> double_input2(array_size);
    
    // Fill input arrays with fuzzed data
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(float); ++i) {
        float_input[i] = fdp.ConsumeFloatingPoint<float>();
    }
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); ++i) {
        double_input[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(float); ++i) {
        float_input2[i] = fdp.ConsumeFloatingPoint<float>();
    }
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() >= sizeof(double); ++i) {
        double_input2[i] = fdp.ConsumeFloatingPoint<double>();
    }
    
    // Create output arrays
    std::vector<float> float_output(array_size);
    std::vector<double> double_output(array_size);
    
    try {
        // Create Mat objects from the vectors (for raw pointer access)
        cv::Mat float_input_mat(array_size, 1, CV_32F, float_input.data());
        cv::Mat double_input_mat(array_size, 1, CV_64F, double_input.data());
        cv::Mat float_input_mat2(array_size, 1, CV_32F, float_input2.data());
        cv::Mat double_input_mat2(array_size, 1, CV_64F, double_input2.data());
        cv::Mat float_output_mat(array_size, 1, CV_32F, float_output.data());
        cv::Mat double_output_mat(array_size, 1, CV_64F, double_output.data());
        
        // Get raw pointers from Mat objects for HAL function calls
        const float* float_src_ptr = float_input_mat.ptr<float>();
        const double* double_src_ptr = double_input_mat.ptr<double>();
        const float* float_src_ptr2 = float_input_mat2.ptr<float>();
        const double* double_src_ptr2 = double_input_mat2.ptr<double>();
        float* float_dst_ptr = float_output_mat.ptr<float>();
        double* double_dst_ptr = double_output_mat.ptr<double>();
        
        // Test different mathematical operations based on operation type
        switch (operation) {
            case 0:  // 32-bit exponential
                if (float_src_ptr != nullptr && float_dst_ptr != nullptr) {
                    cv::hal::exp32f(float_src_ptr, float_dst_ptr, array_size);
                }
                break;
                
            case 1:  // 64-bit exponential
                if (double_src_ptr != nullptr && double_dst_ptr != nullptr) {
                    cv::hal::exp64f(double_src_ptr, double_dst_ptr, array_size);
                }
                break;
                
            case 2:  // 32-bit logarithm
                if (float_src_ptr != nullptr && float_dst_ptr != nullptr) {
                    cv::hal::log32f(float_src_ptr, float_dst_ptr, array_size);
                }
                break;
                
            case 3:  // 64-bit logarithm
                if (double_src_ptr != nullptr && double_dst_ptr != nullptr) {
                    cv::hal::log64f(double_src_ptr, double_dst_ptr, array_size);
                }
                break;
                
            case 4:  // 32-bit square root
                if (float_src_ptr != nullptr && float_dst_ptr != nullptr) {
                    cv::hal::sqrt32f(float_src_ptr, float_dst_ptr, array_size);
                }
                break;
                
            case 5:  // 64-bit square root
                if (double_src_ptr != nullptr && double_dst_ptr != nullptr) {
                    cv::hal::sqrt64f(double_src_ptr, double_dst_ptr, array_size);
                }
                break;
                
            case 6:  // 32-bit inverse square root
                if (float_src_ptr != nullptr && float_dst_ptr != nullptr) {
                    cv::hal::invSqrt32f(float_src_ptr, float_dst_ptr, array_size);
                }
                break;
                
            case 7:  // 64-bit magnitude (needs two input arrays)
                if (double_src_ptr != nullptr && double_src_ptr2 != nullptr && double_dst_ptr != nullptr) {
                    cv::hal::magnitude64f(double_src_ptr, double_src_ptr2, double_dst_ptr, array_size);
                }
                break;
        }
        
        // Optional: Access output values to ensure they're computed
        if (operation < 7) {  // Single-input operations
            if (array_size > 0) {
                // Access first element to exercise the code
                if (operation % 2 == 0) {  // 32-bit operations
                    volatile float val = float_output[0];
                    (void)val;  // Suppress unused variable warning
                } else {  // 64-bit operations
                    volatile double val = double_output[0];
                    (void)val;
                }
            }
        } else {  // magnitude64f operation
            if (array_size > 0) {
                volatile double val = double_output[0];
                (void)val;
            }
        }
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected during fuzzing
        // Just return 0 to indicate normal execution
        return 0;
    } catch (...) {
        // Catch any other exceptions
        return 0;
    }
    
    return 0;
}

} // namespace harness_011
