#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"

namespace harness_014 {
/*
 * Fuzzing harness for libvpx VP9 decoder functionality
 * 
 * Targets VP9 decoder-specific APIs with focus on proper error handling and 
 * API contract compliance based on crash analysis findings:
 * 1) VP9 decoder initialization (vpx_codec_vp9_dx() -> vpx_codec_dec_init())
 * 2) VP9 frame decoding (vpx_codec_decode() with various input data patterns)
 * 3) Frame retrieval (vpx_codec_get_frame() with proper iteration)
 * 4) Error handling and NULL checks for all API parameters
 * 5) Correct buffer lifetime management and size validation
 * 6) Avoiding API misuse patterns identified in crash analysis
 *
 * Crash Analysis Lessons Incorporated:
 * - Always check nullable pointers before dereferencing
 * - Validate buffer sizes match between allocation and API calls  
 * - Respect memory ownership contracts in API documentation
 * - Separate encoder/decoder contexts for API testing
 * - Verify ConsumeBytes return sizes before use
 * - Don't assume API fields are always initialized
 * - Don't mix encoder and decoder API calls
 * - Don't free buffers while library holds references
 *
 * Differentiation from existing harnesses:
 * - harness_000: Tests both VP8/VP9 encode-decode pipeline
 * - harness_008: Covers comprehensive decoder setup lifecycle
 * - harness_007: Tests decoder callback execution patterns
 * - harness_010: Focuses on decoder callback registration
 * - harness_013: Targets VP8 decoder specifically
 * 
 * This harness specifically targets VP9 decoder paths with emphasis on
 * API contract compliance and error handling based on crash analysis.
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 32;

// Maximum test frame size
const size_t MAX_FRAME_SIZE = 65536;

// VP9 decoder test configuration with safety parameters
typedef struct vp9_decoder_test_config {
    unsigned int threads;
    unsigned int width;
    unsigned int height;
    vpx_codec_flags_t flags;
    bool test_error_handling;
    bool test_corrupted_data;
    bool test_edge_cases;
    bool test_memory_pressure;
    uint8_t test_scenario;
    bool validate_buffer_sizes;
    bool check_null_pointers;
} vp9_decoder_test_config_t;

// Helper function to generate VP9-like test data with safety checks
static std::vector<uint8_t> generate_vp9_test_data(FuzzedDataProvider* fdp, 
                                                   bool corrupted, 
                                                   bool edge_case,
                                                   bool validate_sizes) {
    std::vector<uint8_t> frame_data;
    
    // Always check fdp pointer (crash analysis lesson: check nullable pointers)
    if (!fdp) {
        return frame_data;
    }
    
    if (edge_case && fdp->remaining_bytes() > 0) {
        // Generate edge case frame data
        uint8_t edge_type = fdp->ConsumeIntegral<uint8_t>() % 4;
        
        switch (edge_type) {
            case 0: // Very small frame
                frame_data.push_back(0x85); // VP9 frame marker
                frame_data.push_back(0x01); // profile
                frame_data.push_back(0x00); // reserved
                break;
                
            case 1: // Minimal valid VP9 frame
                frame_data.push_back(0x85); // VP9 frame marker
                frame_data.push_back(0x01); // profile
                frame_data.push_back(0x00); // reserved
                // Add minimal frame data
                frame_data.push_back(0x00);
                frame_data.push_back(0x00);
                break;
                
            case 2: // Frame with unusual parameters
                frame_data.push_back(0x85); // VP9 frame marker
                frame_data.push_back(0x03); // profile 3
                frame_data.push_back(0x00); // reserved
                break;
                
            case 3: // Empty frame (edge case for error handling)
                // Intentionally empty to test error paths
                break;
        }
    } else if (fdp->remaining_bytes() > 0) {
        // Generate regular VP9-like frame data
        // Consume size but validate it (crash analysis lesson: validate buffer sizes)
        size_t requested_frame_size = fdp->ConsumeIntegralInRange<size_t>(4, MAX_FRAME_SIZE);
        
        // Actually consume bytes and check returned size (crash analysis lesson: verify ConsumeBytes return sizes)
        std::vector<uint8_t> raw_data = fdp->ConsumeBytes<uint8_t>(requested_frame_size);
        
        // Use actual size, not requested size (FIX for crash pattern A)
        size_t actual_size = raw_data.size();
        
        if (actual_size > 0) {
            // Add VP9 frame header
            frame_data.push_back(0x85); // VP9 frame marker
            frame_data.push_back(0x01); // profile
            frame_data.push_back(0x00); // reserved
            
            // Append the actual consumed data
            frame_data.insert(frame_data.end(), raw_data.begin(), raw_data.end());
            
            if (corrupted && actual_size > 10) {
                // Corrupt some bytes to test error handling
                for (size_t i = 3; i < std::min(actual_size, (size_t)10); i++) {
                    frame_data[i] ^= 0xFF; // Flip bits
                }
            }
        }
    }
    
    return frame_data;
}

// Helper function to safely initialize decoder with configuration
static vpx_codec_err_t safe_decoder_init(vpx_codec_ctx_t* ctx, 
                                        const vpx_codec_iface_t* iface,
                                        vpx_codec_dec_cfg_t* cfg,
                                        vpx_codec_flags_t flags) {
    // Check all pointers before use (crash analysis lesson: check nullable pointers)
    if (!ctx || !iface) {
        return VPX_CODEC_INVALID_PARAM;
    }
    
    // Initialize decoder context
    vpx_codec_err_t res;
    
    if (cfg) {
        res = vpx_codec_dec_init(ctx, iface, cfg, flags);
    } else {
        // Use default configuration
        res = vpx_codec_dec_init(ctx, iface, NULL, flags);
    }
    
    return res;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Early return for insufficient input
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    vp9_decoder_test_config_t config;
    
    // Use safe consumption with bounds checking
    config.threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
    config.width = fdp.ConsumeIntegralInRange<unsigned int>(1, 8192);
    config.height = fdp.ConsumeIntegralInRange<unsigned int>(1, 8192);
    
    // Consume flags with masking to ensure valid values
    uint32_t flags_bits = fdp.ConsumeIntegral<uint32_t>();
    config.flags = static_cast<vpx_codec_flags_t>(flags_bits & 0xFFFF); // Mask to reasonable range
    
    // Consume test parameters
    config.test_error_handling = fdp.ConsumeBool();
    config.test_corrupted_data = fdp.ConsumeBool();
    config.test_edge_cases = fdp.ConsumeBool();
    config.test_memory_pressure = fdp.ConsumeBool();
    config.test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    config.validate_buffer_sizes = true; // Always validate based on crash analysis
    config.check_null_pointers = true;   // Always check based on crash analysis
    
    // Get VP9 decoder interface
    const vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    // Check pointer before use (crash analysis lesson: check nullable pointers)
    if (!decoder_iface) {
        return 0;
    }
    
    // Test different scenarios based on fuzzer input
    switch (config.test_scenario) {
        case 0: {
            // Scenario 0: Normal decoding with configuration
            vpx_codec_ctx_t decoder;
            vpx_codec_dec_cfg_t cfg;
            
            // Initialize configuration safely
            memset(&cfg, 0, sizeof(cfg));
            cfg.threads = config.threads;
            
            // Initialize decoder with safety checks
            vpx_codec_err_t init_res = safe_decoder_init(&decoder, decoder_iface, &cfg, config.flags);
            
            if (init_res == VPX_CODEC_OK) {
                // Generate test frame data
                std::vector<uint8_t> frame_data = generate_vp9_test_data(&fdp, 
                                                                        config.test_corrupted_data,
                                                                        config.test_edge_cases,
                                                                        config.validate_buffer_sizes);
                
                // Decode if we have data (check size before use)
                if (!frame_data.empty()) {
                    // Use actual size, not requested size (FIX for crash pattern A)
                    unsigned int data_sz = static_cast<unsigned int>(frame_data.size());
                    
                    // Decode the frame
                    vpx_codec_err_t decode_res = vpx_codec_decode(&decoder, 
                                                                  frame_data.data(), 
                                                                  data_sz, 
                                                                  NULL, 0);
                    
                    // Check decode result and handle errors appropriately
                    if (decode_res == VPX_CODEC_OK || config.test_error_handling) {
                        // Try to get decoded frames
                        vpx_codec_iter_t iter = NULL;
                        const vpx_image_t* img;
                        
                        while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
                            // Process the image if valid
                            if (img && config.check_null_pointers) {
                                // Safely access image fields
                                (void)img->d_w;
                                (void)img->d_h;
                                (void)img->fmt;
                                
                                // Check plane pointers before accessing (crash analysis lesson)
                                if (img->planes[0]) {
                                    volatile uint8_t sample = img->planes[0][0];
                                    (void)sample; // Prevent unused warning
                                }
                            }
                        }
                    }
                }
                
                // Destroy decoder
                vpx_codec_destroy(&decoder);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Error handling tests with invalid parameters
            if (config.test_error_handling) {
                // Test 1: NULL context pointer
                vpx_codec_err_t res1 = vpx_codec_decode(NULL, data, (unsigned int)size, NULL, 0);
                (void)res1;
                
                // Test 2: NULL data pointer with non-zero size
                vpx_codec_err_t res2 = vpx_codec_decode(NULL, NULL, 1, NULL, 0);
                (void)res2;
                
                // Test 3: Valid context but corrupted data
                vpx_codec_ctx_t decoder;
                vpx_codec_dec_cfg_t cfg;
                
                memset(&cfg, 0, sizeof(cfg));
                cfg.threads = 1;
                
                vpx_codec_err_t init_res = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, 0);
                
                if (init_res == VPX_CODEC_OK) {
                    // Test with obviously corrupted/invalid data
                    uint8_t invalid_data[] = {0xFF, 0xFF, 0xFF, 0xFF};
                    vpx_codec_err_t decode_res = vpx_codec_decode(&decoder, invalid_data, 
                                                                  sizeof(invalid_data), NULL, 0);
                    (void)decode_res;
                    
                    // Test get_frame with NULL iterator
                    vpx_image_t* img = vpx_codec_get_frame(&decoder, NULL);
                    (void)img;
                    
                    vpx_codec_destroy(&decoder);
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Multiple decode cycles with different data
            vpx_codec_ctx_t decoder;
            
            // Initialize with default configuration
            vpx_codec_err_t init_res = vpx_codec_dec_init(&decoder, decoder_iface, NULL, 0);
            
            if (init_res == VPX_CODEC_OK) {
                // Perform multiple decode cycles
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
                    std::vector<uint8_t> frame_data = generate_vp9_test_data(&fdp, 
                                                                            config.test_corrupted_data,
                                                                            config.test_edge_cases,
                                                                            config.validate_buffer_sizes);
                    
                    if (!frame_data.empty()) {
                        unsigned int data_sz = static_cast<unsigned int>(frame_data.size());
                        vpx_codec_decode(&decoder, frame_data.data(), data_sz, NULL, 0);
                        
                        // Get and discard frames
                        vpx_codec_iter_t iter = NULL;
                        while (vpx_codec_get_frame(&decoder, &iter) != NULL) {
                            // Just consume frames
                        }
                    }
                }
                
                vpx_codec_destroy(&decoder);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Memory pressure tests with large configurations
            if (config.test_memory_pressure && fdp.remaining_bytes() > 100) {
                vpx_codec_ctx_t decoder;
                vpx_codec_dec_cfg_t cfg;
                
                // Use larger thread count for memory pressure
                memset(&cfg, 0, sizeof(cfg));
                cfg.threads = fdp.ConsumeIntegralInRange<unsigned int>(4, 16);
                
                vpx_codec_err_t init_res = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, 
                                                             VPX_CODEC_USE_FRAME_THREADING);
                
                if (init_res == VPX_CODEC_OK) {
                    // Generate and decode larger frame
                    std::vector<uint8_t> frame_data = generate_vp9_test_data(&fdp, 
                                                                            false, // Don't corrupt for memory tests
                                                                            false,
                                                                            config.validate_buffer_sizes);
                    
                    if (!frame_data.empty()) {
                        unsigned int data_sz = static_cast<unsigned int>(frame_data.size());
                        vpx_codec_decode(&decoder, frame_data.data(), data_sz, NULL, 0);
                        
                        // Exercise get_frame to ensure memory is properly managed
                        vpx_codec_iter_t iter = NULL;
                        int frame_count = 0;
                        while (vpx_codec_get_frame(&decoder, &iter) != NULL && frame_count < 10) {
                            frame_count++;
                        }
                    }
                    
                    vpx_codec_destroy(&decoder);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_014
