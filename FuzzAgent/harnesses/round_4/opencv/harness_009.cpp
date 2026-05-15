#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core/hal/hal.hpp>

namespace harness_009 {
// OpenCV image channel splitting/merging operations fuzzing harness
// Targets: HAL split functions (split8u, split16u, split32s, split64s) and merge functions (merge8u, merge16u, merge32s, merge64s)
// harness_009.cpp - Targets low-level HAL channel operations with over 28+ undiscovered branches
// Different from previous harnesses (image processing, mathematical functions, calibration, tracking) by focusing on channel splitting/merging


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic channel operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Determine test scenario from fuzzer input
        uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (scenario) {
            case 0: {
                // Scenario 0: Test split8u and merge8u (8-bit unsigned)
                int len = fdp.ConsumeIntegralInRange<int>(1, 100);
                int cn = fdp.ConsumeIntegralInRange<int>(1, 4); // Number of channels (1-4)
                
                // Calculate required input size
                size_t src_size = len * cn * sizeof(uint8_t);
                if (fdp.remaining_bytes() < src_size) {
                    return 0;
                }
                
                // Create source data
                std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(src_size);
                
                // Create destination arrays for split operation
                std::vector<std::vector<uint8_t>> dst_arrays(cn);
                std::vector<uint8_t*> dst_ptrs(cn);
                
                for (int i = 0; i < cn; ++i) {
                    dst_arrays[i].resize(len);
                    dst_ptrs[i] = dst_arrays[i].data();
                }
                
                // Test split8u
                cv::hal::split8u(src_data.data(), dst_ptrs.data(), len, cn);
                
                // Test merge8u (round trip)
                if (fdp.remaining_bytes() >= len * cn * sizeof(uint8_t)) {
                    std::vector<uint8_t> merged_data(len * cn);
                    
                    // Prepare source pointers for merge
                    std::vector<const uint8_t*> src_ptrs(cn);
                    for (int i = 0; i < cn; ++i) {
                        src_ptrs[i] = dst_arrays[i].data();
                    }
                    
                    // Test merge8u
                    cv::hal::merge8u(src_ptrs.data(), merged_data.data(), len, cn);
                }
                break;
            }
            
            case 1: {
                // Scenario 1: Test split16u and merge16u (16-bit unsigned)
                int len = fdp.ConsumeIntegralInRange<int>(1, 50);
                int cn = fdp.ConsumeIntegralInRange<int>(1, 4);
                
                size_t src_size = len * cn * sizeof(uint16_t);
                if (fdp.remaining_bytes() < src_size) {
                    return 0;
                }
                
                // Create source data (need to convert bytes to uint16_t)
                std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(src_size);
                std::vector<uint16_t> src_data(len * cn);
                memcpy(src_data.data(), raw_data.data(), src_size);
                
                // Create destination arrays
                std::vector<std::vector<uint16_t>> dst_arrays(cn);
                std::vector<uint16_t*> dst_ptrs(cn);
                
                for (int i = 0; i < cn; ++i) {
                    dst_arrays[i].resize(len);
                    dst_ptrs[i] = dst_arrays[i].data();
                }
                
                // Test split16u
                cv::hal::split16u(src_data.data(), dst_ptrs.data(), len, cn);
                
                // Test merge16u
                if (fdp.remaining_bytes() >= len * cn * sizeof(uint16_t)) {
                    std::vector<uint16_t> merged_data(len * cn);
                    std::vector<const uint16_t*> src_ptrs(cn);
                    
                    for (int i = 0; i < cn; ++i) {
                        src_ptrs[i] = dst_arrays[i].data();
                    }
                    
                    cv::hal::merge16u(src_ptrs.data(), merged_data.data(), len, cn);
                }
                break;
            }
            
            case 2: {
                // Scenario 2: Test split32s and merge32s (32-bit signed)
                int len = fdp.ConsumeIntegralInRange<int>(1, 25);
                int cn = fdp.ConsumeIntegralInRange<int>(1, 4);
                
                size_t src_size = len * cn * sizeof(int32_t);
                if (fdp.remaining_bytes() < src_size) {
                    return 0;
                }
                
                std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(src_size);
                std::vector<int32_t> src_data(len * cn);
                memcpy(src_data.data(), raw_data.data(), src_size);
                
                std::vector<std::vector<int32_t>> dst_arrays(cn);
                std::vector<int32_t*> dst_ptrs(cn);
                
                for (int i = 0; i < cn; ++i) {
                    dst_arrays[i].resize(len);
                    dst_ptrs[i] = dst_arrays[i].data();
                }
                
                // Test split32s
                cv::hal::split32s(src_data.data(), dst_ptrs.data(), len, cn);
                
                // Test merge32s
                if (fdp.remaining_bytes() >= len * cn * sizeof(int32_t)) {
                    std::vector<int32_t> merged_data(len * cn);
                    std::vector<const int32_t*> src_ptrs(cn);
                    
                    for (int i = 0; i < cn; ++i) {
                        src_ptrs[i] = dst_arrays[i].data();
                    }
                    
                    cv::hal::merge32s(src_ptrs.data(), merged_data.data(), len, cn);
                }
                break;
            }
            
            case 3: {
                // Scenario 3: Test split64s and merge64s (64-bit signed)
                int len = fdp.ConsumeIntegralInRange<int>(1, 12);
                int cn = fdp.ConsumeIntegralInRange<int>(1, 4);
                
                size_t src_size = len * cn * sizeof(int64_t);
                if (fdp.remaining_bytes() < src_size) {
                    return 0;
                }
                
                std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(src_size);
                std::vector<int64_t> src_data(len * cn);
                memcpy(src_data.data(), raw_data.data(), src_size);
                
                std::vector<std::vector<int64_t>> dst_arrays(cn);
                std::vector<int64_t*> dst_ptrs(cn);
                
                for (int i = 0; i < cn; ++i) {
                    dst_arrays[i].resize(len);
                    dst_ptrs[i] = dst_arrays[i].data();
                }
                
                // Test split64s
                cv::hal::split64s(src_data.data(), dst_ptrs.data(), len, cn);
                
                // Test merge64s
                if (fdp.remaining_bytes() >= len * cn * sizeof(int64_t)) {
                    std::vector<int64_t> merged_data(len * cn);
                    std::vector<const int64_t*> src_ptrs(cn);
                    
                    for (int i = 0; i < cn; ++i) {
                        src_ptrs[i] = dst_arrays[i].data();
                    }
                    
                    cv::hal::merge64s(src_ptrs.data(), merged_data.data(), len, cn);
                }
                break;
            }
            
            case 4: {
                // Scenario 4: Mixed operations and edge cases
                // Test different channel counts and array sizes
                uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                int len = fdp.ConsumeIntegralInRange<int>(1, 100);
                int cn = fdp.ConsumeIntegralInRange<int>(1, 4);
                
                switch (data_type) {
                    case 0: {
                        // 8-bit unsigned
                        size_t src_size = len * cn * sizeof(uint8_t);
                        if (fdp.remaining_bytes() < src_size) {
                            return 0;
                        }
                        
                        std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(src_size);
                        std::vector<std::vector<uint8_t>> dst_arrays(cn);
                        std::vector<uint8_t*> dst_ptrs(cn);
                        
                        for (int i = 0; i < cn; ++i) {
                            dst_arrays[i].resize(len);
                            dst_ptrs[i] = dst_arrays[i].data();
                        }
                        
                        // Test with different channel counts
                        cv::hal::split8u(src_data.data(), dst_ptrs.data(), len, cn);
                        
                        // Test split with single channel (cn=1)
                        if (cn > 1) {
                            std::vector<uint8_t> single_channel_data(len);
                            std::vector<uint8_t*> single_dst_ptr = {single_channel_data.data()};
                            cv::hal::split8u(src_data.data(), single_dst_ptr.data(), len, 1);
                        }
                        break;
                    }
                    
                    case 1: {
                        // 16-bit unsigned
                        size_t src_size = len * cn * sizeof(uint16_t);
                        if (fdp.remaining_bytes() < src_size) {
                            return 0;
                        }
                        
                        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(src_size);
                        std::vector<uint16_t> src_data(len * cn);
                        memcpy(src_data.data(), raw_data.data(), src_size);
                        
                        std::vector<std::vector<uint16_t>> dst_arrays(cn);
                        std::vector<uint16_t*> dst_ptrs(cn);
                        
                        for (int i = 0; i < cn; ++i) {
                            dst_arrays[i].resize(len);
                            dst_ptrs[i] = dst_arrays[i].data();
                        }
                        
                        cv::hal::split16u(src_data.data(), dst_ptrs.data(), len, cn);
                        
                        // Test with max channels (4)
                        if (cn < 4 && fdp.remaining_bytes() >= len * 4 * sizeof(uint16_t)) {
                            std::vector<uint8_t> max_raw_data = fdp.ConsumeBytes<uint8_t>(len * 4 * sizeof(uint16_t));
                            std::vector<uint16_t> max_src_data(len * 4);
                            memcpy(max_src_data.data(), max_raw_data.data(), len * 4 * sizeof(uint16_t));
                            
                            std::vector<std::vector<uint16_t>> max_dst_arrays(4);
                            std::vector<uint16_t*> max_dst_ptrs(4);
                            
                            for (int i = 0; i < 4; ++i) {
                                max_dst_arrays[i].resize(len);
                                max_dst_ptrs[i] = max_dst_arrays[i].data();
                            }
                            
                            cv::hal::split16u(max_src_data.data(), max_dst_ptrs.data(), len, 4);
                        }
                        break;
                    }
                    
                    case 2: {
                        // 32-bit signed
                        size_t src_size = len * cn * sizeof(int32_t);
                        if (fdp.remaining_bytes() < src_size) {
                            return 0;
                        }
                        
                        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(src_size);
                        std::vector<int32_t> src_data(len * cn);
                        memcpy(src_data.data(), raw_data.data(), src_size);
                        
                        std::vector<std::vector<int32_t>> dst_arrays(cn);
                        std::vector<int32_t*> dst_ptrs(cn);
                        
                        for (int i = 0; i < cn; ++i) {
                            dst_arrays[i].resize(len);
                            dst_ptrs[i] = dst_arrays[i].data();
                        }
                        
                        cv::hal::split32s(src_data.data(), dst_ptrs.data(), len, cn);
                        
                        // Test with small length (edge case)
                        if (len > 1) {
                            int small_len = 1;
                            size_t small_src_size = small_len * cn * sizeof(int32_t);
                            
                            if (fdp.remaining_bytes() >= small_src_size) {
                                std::vector<uint8_t> small_raw_data = fdp.ConsumeBytes<uint8_t>(small_src_size);
                                std::vector<int32_t> small_src_data(small_len * cn);
                                memcpy(small_src_data.data(), small_raw_data.data(), small_src_size);
                                
                                std::vector<std::vector<int32_t>> small_dst_arrays(cn);
                                std::vector<int32_t*> small_dst_ptrs(cn);
                                
                                for (int i = 0; i < cn; ++i) {
                                    small_dst_arrays[i].resize(small_len);
                                    small_dst_ptrs[i] = small_dst_arrays[i].data();
                                }
                                
                                cv::hal::split32s(small_src_data.data(), small_dst_ptrs.data(), small_len, cn);
                            }
                        }
                        break;
                    }
                    
                    case 3: {
                        // 64-bit signed
                        size_t src_size = len * cn * sizeof(int64_t);
                        if (fdp.remaining_bytes() < src_size) {
                            return 0;
                        }
                        
                        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(src_size);
                        std::vector<int64_t> src_data(len * cn);
                        memcpy(src_data.data(), raw_data.data(), src_size);
                        
                        std::vector<std::vector<int64_t>> dst_arrays(cn);
                        std::vector<int64_t*> dst_ptrs(cn);
                        
                        for (int i = 0; i < cn; ++i) {
                            dst_arrays[i].resize(len);
                            dst_ptrs[i] = dst_arrays[i].data();
                        }
                        
                        cv::hal::split64s(src_data.data(), dst_ptrs.data(), len, cn);
                        
                        // Test split-merge round trip
                        if (fdp.remaining_bytes() >= len * cn * sizeof(int64_t)) {
                            std::vector<int64_t> merged_data(len * cn);
                            std::vector<const int64_t*> src_ptrs(cn);
                            
                            for (int i = 0; i < cn; ++i) {
                                src_ptrs[i] = dst_arrays[i].data();
                            }
                            
                            cv::hal::merge64s(src_ptrs.data(), merged_data.data(), len, cn);
                        }
                        break;
                    }
                }
                break;
            }
        }
        
    } catch (...) {
        // Catch any exceptions - they're expected during fuzzing
    }
    
    return 0;
}

} // namespace harness_009
