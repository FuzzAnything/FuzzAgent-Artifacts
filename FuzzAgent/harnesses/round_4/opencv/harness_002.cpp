#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/hal.hpp>

namespace harness_002 {
// OpenCV HAL mathematical functions fuzzing harness
// Targets: sqrt, exp, log, invSqrt, fastAtan2 HAL functions (low coverage)
// Different from harness_000 (image processing) and harness_001 (coordinate transformations)
// harness_002.cpp


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for mathematical operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Part 1: Determine operation parameters from fuzzer input
        int operation = fdp.ConsumeIntegralInRange<int>(0, 7);  // 8 different operations
        int data_type = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0: float (32f), 1: double (64f)
        int array_len = fdp.ConsumeIntegralInRange<int>(1, 100);
        bool use_degrees = fdp.ConsumeBool();  // For fastAtan2 operations
        
        // Calculate required data size based on array length and data type
        size_t elem_size = (data_type == 0) ? sizeof(float) : sizeof(double);
        size_t array_bytes = array_len * elem_size;
        
        // For operations that need two input arrays (like fastAtan2)
        size_t total_required_bytes = 0;
        if (operation >= 5 && operation <= 7) {
            // fastAtan2 needs X and Y arrays
            total_required_bytes = 2 * array_bytes;
        } else {
            // Single input array operations
            total_required_bytes = array_bytes;
        }
        
        if (fdp.remaining_bytes() < total_required_bytes) {
            return 0;
        }
        
        // Create input arrays from fuzzer data
        if (data_type == 0) {  // Float operations
            std::vector<float> src_data(array_len);
            std::vector<float> dst_data(array_len);
            
            // Fill src_data with fuzzer input
            std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(array_bytes);
            if (raw_data.size() == array_bytes) {
                memcpy(src_data.data(), raw_data.data(), array_bytes);
                
                // Apply the selected mathematical operation
                switch (operation % 8) {
                    case 0:  // sqrt32f
                        cv::hal::sqrt32f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 1:  // exp32f
                        cv::hal::exp32f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 2:  // log32f
                        cv::hal::log32f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 3:  // invSqrt32f
                        cv::hal::invSqrt32f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 4:  // sqrt -> log chain (test multiple operations)
                        cv::hal::sqrt32f(src_data.data(), dst_data.data(), array_len);
                        if (!dst_data.empty()) {
                            std::vector<float> temp_data(array_len);
                            cv::hal::log32f(dst_data.data(), temp_data.data(), array_len);
                        }
                        break;
                        
                    case 5:  // fastAtan32f (needs X and Y arrays)
                        if (fdp.remaining_bytes() >= array_bytes) {
                            std::vector<float> y_data(array_len);
                            std::vector<uint8_t> y_raw = fdp.ConsumeBytes<uint8_t>(array_bytes);
                            memcpy(y_data.data(), y_raw.data(), array_bytes);
                            
                            cv::hal::fastAtan32f(y_data.data(), src_data.data(), 
                                                dst_data.data(), array_len, use_degrees);
                        }
                        break;
                        
                    case 6:  // magnitude32f (needs X and Y arrays)
                        if (fdp.remaining_bytes() >= array_bytes) {
                            std::vector<float> y_data(array_len);
                            std::vector<uint8_t> y_raw = fdp.ConsumeBytes<uint8_t>(array_bytes);
                            memcpy(y_data.data(), y_raw.data(), array_bytes);
                            
                            cv::hal::magnitude32f(src_data.data(), y_data.data(), 
                                                 dst_data.data(), array_len);
                        }
                        break;
                        
                    case 7:  // exp -> log round trip
                        cv::hal::exp32f(src_data.data(), dst_data.data(), array_len);
                        if (!dst_data.empty()) {
                            std::vector<float> temp_data(array_len);
                            cv::hal::log32f(dst_data.data(), temp_data.data(), array_len);
                        }
                        break;
                }
            }
        } else {  // Double operations (64f)
            std::vector<double> src_data(array_len);
            std::vector<double> dst_data(array_len);
            
            // Fill src_data with fuzzer input
            std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(array_bytes);
            if (raw_data.size() == array_bytes) {
                memcpy(src_data.data(), raw_data.data(), array_bytes);
                
                // Apply the selected mathematical operation
                switch (operation % 8) {
                    case 0:  // sqrt64f
                        cv::hal::sqrt64f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 1:  // exp64f
                        cv::hal::exp64f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 2:  // log64f
                        cv::hal::log64f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 3:  // invSqrt64f
                        cv::hal::invSqrt64f(src_data.data(), dst_data.data(), array_len);
                        break;
                        
                    case 4:  // sqrt -> log chain (test multiple operations)
                        cv::hal::sqrt64f(src_data.data(), dst_data.data(), array_len);
                        if (!dst_data.empty()) {
                            std::vector<double> temp_data(array_len);
                            cv::hal::log64f(dst_data.data(), temp_data.data(), array_len);
                        }
                        break;
                        
                    case 5:  // fastAtan64f (needs X and Y arrays)
                        if (fdp.remaining_bytes() >= array_bytes) {
                            std::vector<double> y_data(array_len);
                            std::vector<uint8_t> y_raw = fdp.ConsumeBytes<uint8_t>(array_bytes);
                            memcpy(y_data.data(), y_raw.data(), array_bytes);
                            
                            cv::hal::fastAtan64f(y_data.data(), src_data.data(), 
                                                dst_data.data(), array_len, use_degrees);
                        }
                        break;
                        
                    case 6:  // magnitude64f (needs X and Y arrays)
                        if (fdp.remaining_bytes() >= array_bytes) {
                            std::vector<double> y_data(array_len);
                            std::vector<uint8_t> y_raw = fdp.ConsumeBytes<uint8_t>(array_bytes);
                            memcpy(y_data.data(), y_raw.data(), array_bytes);
                            
                            cv::hal::magnitude64f(src_data.data(), y_data.data(), 
                                                 dst_data.data(), array_len);
                        }
                        break;
                        
                    case 7:  // exp -> log round trip
                        cv::hal::exp64f(src_data.data(), dst_data.data(), array_len);
                        if (!dst_data.empty()) {
                            std::vector<double> temp_data(array_len);
                            cv::hal::log64f(dst_data.data(), temp_data.data(), array_len);
                        }
                        break;
                }
            }
        }
        
        // Part 2: Test with mixed operations using remaining input
        if (fdp.remaining_bytes() >= 32) {
            int mixed_len = fdp.ConsumeIntegralInRange<int>(1, 20);
            size_t mixed_elem_size = sizeof(float);  // Use float for mixed tests
            size_t mixed_bytes = mixed_len * mixed_elem_size;
            
            if (fdp.remaining_bytes() >= mixed_bytes) {
                std::vector<float> mixed_src(mixed_len);
                std::vector<float> mixed_dst(mixed_len);
                
                std::vector<uint8_t> mixed_raw = fdp.ConsumeBytes<uint8_t>(mixed_bytes);
                memcpy(mixed_src.data(), mixed_raw.data(), mixed_bytes);
                
                // Test a sequence of different operations on the same data
                int mixed_op = fdp.ConsumeIntegralInRange<int>(0, 3);
                
                switch (mixed_op % 4) {
                    case 0:  // sqrt -> exp
                        cv::hal::sqrt32f(mixed_src.data(), mixed_dst.data(), mixed_len);
                        if (!mixed_dst.empty()) {
                            std::vector<float> temp(mixed_len);
                            cv::hal::exp32f(mixed_dst.data(), temp.data(), mixed_len);
                        }
                        break;
                        
                    case 1:  // exp -> sqrt
                        cv::hal::exp32f(mixed_src.data(), mixed_dst.data(), mixed_len);
                        if (!mixed_dst.empty()) {
                            std::vector<float> temp(mixed_len);
                            cv::hal::sqrt32f(mixed_dst.data(), temp.data(), mixed_len);
                        }
                        break;
                        
                    case 2:  // log -> invSqrt
                        cv::hal::log32f(mixed_src.data(), mixed_dst.data(), mixed_len);
                        if (!mixed_dst.empty()) {
                            std::vector<float> temp(mixed_len);
                            cv::hal::invSqrt32f(mixed_dst.data(), temp.data(), mixed_len);
                        }
                        break;
                        
                    case 3:  // invSqrt -> exp
                        cv::hal::invSqrt32f(mixed_src.data(), mixed_dst.data(), mixed_len);
                        if (!mixed_dst.empty()) {
                            std::vector<float> temp(mixed_len);
                            cv::hal::exp32f(mixed_dst.data(), temp.data(), mixed_len);
                        }
                        break;
                }
            }
        }
        
        // Part 3: Test edge cases with small arrays
        if (fdp.remaining_bytes() >= 16) {
            int edge_len = 1;  // Minimum array length
            size_t edge_elem_size = sizeof(double);  // Use double for edge cases
            size_t edge_bytes = edge_len * edge_elem_size;
            
            if (fdp.remaining_bytes() >= 2 * edge_bytes) {
                std::vector<double> edge_src(edge_len);
                std::vector<double> edge_dst(edge_len);
                
                std::vector<uint8_t> edge_raw1 = fdp.ConsumeBytes<uint8_t>(edge_bytes);
                std::vector<uint8_t> edge_raw2 = fdp.ConsumeBytes<uint8_t>(edge_bytes);
                
                memcpy(edge_src.data(), edge_raw1.data(), edge_bytes);
                
                // Test with potentially problematic values
                int edge_op = fdp.ConsumeIntegralInRange<int>(0, 2);
                
                switch (edge_op % 3) {
                    case 0:  // sqrt with edge values
                        cv::hal::sqrt64f(edge_src.data(), edge_dst.data(), edge_len);
                        break;
                        
                    case 1:  // log with edge values
                        cv::hal::log64f(edge_src.data(), edge_dst.data(), edge_len);
                        break;
                        
                    case 2:  // exp with edge values
                        cv::hal::exp64f(edge_src.data(), edge_dst.data(), edge_len);
                        break;
                }
            }
        }
        
        // Part 4: Test cartToPolar and polarToCart for completeness (different from harness_001)
        // Test with single precision to complement harness_001's coverage
        if (fdp.remaining_bytes() >= 32) {
            int polar_len = fdp.ConsumeIntegralInRange<int>(1, 10);
            size_t polar_bytes = polar_len * sizeof(float);
            
            if (fdp.remaining_bytes() >= 2 * polar_bytes) {
                std::vector<float> x_data(polar_len);
                std::vector<float> y_data(polar_len);
                std::vector<float> mag_data(polar_len);
                std::vector<float> angle_data(polar_len);
                
                std::vector<uint8_t> x_raw = fdp.ConsumeBytes<uint8_t>(polar_bytes);
                std::vector<uint8_t> y_raw = fdp.ConsumeBytes<uint8_t>(polar_bytes);
                
                memcpy(x_data.data(), x_raw.data(), polar_bytes);
                memcpy(y_data.data(), y_raw.data(), polar_bytes);
                
                bool polar_degrees = fdp.ConsumeBool();
                
                // Test cartToPolar32f
                cv::hal::cartToPolar32f(x_data.data(), y_data.data(), 
                                       mag_data.data(), angle_data.data(), 
                                       polar_len, polar_degrees);
                
                // If we have more data, test polarToCart round trip
                if (!mag_data.empty() && !angle_data.empty() && fdp.remaining_bytes() >= 8) {
                    std::vector<float> x_roundtrip(polar_len);
                    std::vector<float> y_roundtrip(polar_len);
                    
                    cv::hal::polarToCart32f(mag_data.data(), angle_data.data(),
                                           x_roundtrip.data(), y_roundtrip.data(),
                                           polar_len, polar_degrees);
                }
            }
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they're expected during fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_002
