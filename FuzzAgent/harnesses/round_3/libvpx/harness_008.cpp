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

namespace harness_008 {
/*
 * Fuzzing harness for libvpx comprehensive decoder setup lifecycle APIs
 * Targets: vpx_codec_dec_init, vpx_codec_dec_init_ver, vpx_codec_dec_cfg,
 *          vpx_codec_peek_stream_info, vpx_codec_get_stream_info
 * Focus: Comprehensive decoder initialization variations, configuration options,
 *        different decoder setup patterns, and advanced decoder configuration scenarios
 * Distinct from existing harnesses:
 * - harness_000: encode-decode pipeline
 * - harness_001: image manipulation  
 * - harness_002: multi-resolution encoding
 * - harness_003: decoder callback/buffer management
 * - harness_004: external rate control
 * - harness_005: error handling/encoder configuration
 * - harness_006: advanced encoder configuration
 * - harness_007: decoder callback registration
 * This harness focuses exclusively on decoder setup lifecycle and initialization paths
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 64;

// Maximum configuration parameters
const unsigned int MAX_THREADS = 16;
const unsigned int MAX_DIMENSION = 8192;

// Test structure for decoder configuration
typedef struct decoder_test_config {
    unsigned int threads;
    unsigned int width;
    unsigned int height;
    vpx_codec_flags_t flags;
    bool use_config;
    bool test_peek_stream_info;
    bool test_get_stream_info;
    int decoder_type; // 0: VP8, 1: VP9
} decoder_test_config_t;

// Helper function to get decoder interface based on type
static vpx_codec_iface_t* get_decoder_interface(int decoder_type) {
    if (decoder_type == 0) {
        return vpx_codec_vp8_dx();
    } else {
        return vpx_codec_vp9_dx();
    }
}

// Helper function to generate test frame data
static std::vector<uint8_t> generate_test_frame(FuzzedDataProvider& fdp, size_t max_size) {
    size_t frame_size = fdp.ConsumeIntegralInRange<size_t>(16, max_size);
    return fdp.ConsumeBytes<uint8_t>(frame_size);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    decoder_test_config_t config;
    config.threads = fdp.ConsumeIntegralInRange<unsigned int>(1, MAX_THREADS);
    config.width = fdp.ConsumeIntegralInRange<unsigned int>(1, MAX_DIMENSION);
    config.height = fdp.ConsumeIntegralInRange<unsigned int>(1, MAX_DIMENSION);
    
    // Consume flags - use bitmask approach to test different combinations
    uint32_t flags_bits = fdp.ConsumeIntegral<uint32_t>();
    config.flags = static_cast<vpx_codec_flags_t>(flags_bits);
    
    // Consume other test parameters
    config.use_config = fdp.ConsumeBool();
    config.test_peek_stream_info = fdp.ConsumeBool();
    config.test_get_stream_info = fdp.ConsumeBool();
    config.decoder_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Get decoder interface
    vpx_codec_iface_t* decoder_iface = get_decoder_interface(config.decoder_type);
    if (!decoder_iface) {
        return 0;
    }
    
    // Test 1: vpx_codec_peek_stream_info
    if (config.test_peek_stream_info && fdp.remaining_bytes() > 16) {
        vpx_codec_stream_info_t stream_info;
        stream_info.sz = sizeof(vpx_codec_stream_info_t);
        
        // Generate some test frame data for peek
        size_t peek_data_size = std::min(fdp.remaining_bytes(), (size_t)4096);
        std::vector<uint8_t> peek_data = fdp.ConsumeBytes<uint8_t>(peek_data_size);
        
        if (peek_data.size() > 0) {
            vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
                decoder_iface, 
                peek_data.data(), 
                static_cast<unsigned int>(peek_data.size()),
                &stream_info
            );
            
            // We don't check the result - just exercise the API
            (void)peek_result;
        }
    }
    
    // Test 2: Decoder initialization with various configurations
    vpx_codec_ctx_t codec_ctx;
    
    // Prepare configuration if requested
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_dec_cfg_t* cfg_ptr = nullptr;
    
    if (config.use_config) {
        decoder_cfg.threads = config.threads;
        decoder_cfg.w = config.width;
        decoder_cfg.h = config.height;
        cfg_ptr = &decoder_cfg;
    }
    
    // Try initialization with vpx_codec_dec_init (macro wrapper)
    vpx_codec_err_t init_result = vpx_codec_dec_init(&codec_ctx, decoder_iface, cfg_ptr, config.flags);
    
    if (init_result == VPX_CODEC_OK) {
        // Test 3: vpx_codec_get_stream_info if decoder initialized successfully
        if (config.test_get_stream_info) {
            vpx_codec_stream_info_t stream_info;
            stream_info.sz = sizeof(vpx_codec_stream_info_t);
            
            vpx_codec_err_t get_info_result = vpx_codec_get_stream_info(&codec_ctx, &stream_info);
            (void)get_info_result;
        }
        
        // Test 4: Try to decode some test data if we have remaining input
        if (fdp.remaining_bytes() > 16) {
            size_t decode_data_size = std::min(fdp.remaining_bytes(), (size_t)8192);
            std::vector<uint8_t> decode_data = fdp.ConsumeBytes<uint8_t>(decode_data_size);
            
            // Try decoding with various deadlines
            long deadline = fdp.ConsumeIntegral<long>();
            vpx_codec_err_t decode_result = vpx_codec_decode(
                &codec_ctx, 
                decode_data.data(), 
                static_cast<unsigned int>(decode_data.size()),
                nullptr, 
                deadline
            );
            (void)decode_result;
            
            // Try to get decoded frames if decode was successful
            if (decode_result == VPX_CODEC_OK || decode_result == VPX_CODEC_UNSUP_BITSTREAM) {
                vpx_codec_iter_t iter = nullptr;
                vpx_image_t* img;
                
                // Try to get a few frames
                int max_frames = fdp.ConsumeIntegralInRange<int>(0, 10);
                for (int i = 0; i < max_frames; i++) {
                    img = vpx_codec_get_frame(&codec_ctx, &iter);
                    if (!img) {
                        break;
                    }
                    // Just access some image properties to exercise more code
                    (void)img->fmt;
                    (void)img->d_w;
                    (void)img->d_h;
                }
            }
        }
        
        // Clean up
        vpx_codec_destroy(&codec_ctx);
    }
    
    // Test 5: Try initialization with vpx_codec_dec_init_ver directly
    // Reset the context
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // Try different ABI versions (some invalid, some valid)
    int abi_version = fdp.ConsumeIntegral<int>();
    
    vpx_codec_err_t init_ver_result = vpx_codec_dec_init_ver(
        &codec_ctx, 
        decoder_iface, 
        cfg_ptr, 
        config.flags, 
        abi_version
    );
    
    // If initialization succeeded, clean up
    if (init_ver_result == VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx);
    }
    
    // Test 6: Multiple initialization/destruction cycles with different configs
    int cycle_count = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int cycle = 0; cycle < cycle_count && fdp.remaining_bytes() > 16; cycle++) {
        // Generate new config for this cycle
        vpx_codec_dec_cfg_t cycle_cfg;
        cycle_cfg.threads = fdp.ConsumeIntegralInRange<unsigned int>(1, MAX_THREADS);
        cycle_cfg.w = fdp.ConsumeIntegralInRange<unsigned int>(1, MAX_DIMENSION);
        cycle_cfg.h = fdp.ConsumeIntegralInRange<unsigned int>(1, MAX_DIMENSION);
        
        vpx_codec_flags_t cycle_flags = static_cast<vpx_codec_flags_t>(fdp.ConsumeIntegral<uint32_t>());
        
        vpx_codec_err_t cycle_result = vpx_codec_dec_init(
            &codec_ctx, 
            decoder_iface, 
            &cycle_cfg, 
            cycle_flags
        );
        
        if (cycle_result == VPX_CODEC_OK) {
            // Try a quick decode if we have data
            if (fdp.remaining_bytes() > 32) {
                size_t cycle_data_size = std::min(fdp.remaining_bytes(), (size_t)1024);
                std::vector<uint8_t> cycle_data = fdp.ConsumeBytes<uint8_t>(cycle_data_size);
                
                vpx_codec_decode(
                    &codec_ctx, 
                    cycle_data.data(), 
                    static_cast<unsigned int>(cycle_data.size()),
                    nullptr, 
                    0
                );
            }
            
            vpx_codec_destroy(&codec_ctx);
        }
    }
    
    // Test 7: Edge case - NULL configuration pointer
    if (fdp.ConsumeBool()) {
        vpx_codec_err_t null_cfg_result = vpx_codec_dec_init(
            &codec_ctx, 
            decoder_iface, 
            nullptr,  // NULL config
            config.flags
        );
        
        if (null_cfg_result == VPX_CODEC_OK) {
            vpx_codec_destroy(&codec_ctx);
        }
    }
    
    // Test 8: Edge case - zero dimension configuration
    if (fdp.ConsumeBool()) {
        vpx_codec_dec_cfg_t zero_cfg;
        zero_cfg.threads = config.threads;
        zero_cfg.w = 0;  // Zero width
        zero_cfg.h = 0;  // Zero height
        
        vpx_codec_err_t zero_cfg_result = vpx_codec_dec_init(
            &codec_ctx, 
            decoder_iface, 
            &zero_cfg, 
            config.flags
        );
        
        if (zero_cfg_result == VPX_CODEC_OK) {
            vpx_codec_destroy(&codec_ctx);
        }
    }
    
    // Test 9: Test with different decoder interfaces
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        // Switch decoder type
        int alt_decoder_type = 1 - config.decoder_type; // Switch between 0 and 1
        vpx_codec_iface_t* alt_decoder_iface = get_decoder_interface(alt_decoder_type);
        
        if (alt_decoder_iface) {
            vpx_codec_err_t alt_result = vpx_codec_dec_init(
                &codec_ctx, 
                alt_decoder_iface, 
                cfg_ptr, 
                config.flags
            );
            
            if (alt_result == VPX_CODEC_OK) {
                vpx_codec_destroy(&codec_ctx);
            }
        }
    }
    
    return 0;
}

} // namespace harness_008
