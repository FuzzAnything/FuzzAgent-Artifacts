#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

namespace harness_002 {
/*
 * Fuzzing harness for libvpx multi-encoder initialization APIs
 * Targets: vpx_codec_enc_init_multi_ver, vpx_codec_enc_init_multi,
 *          vpx_codec_enc_config_default, vpx_codec_enc_config_set
 * Focus: Multi-encoder initialization lifecycle and configuration APIs
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic configuration
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test mode from first byte
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>();
    
    // Test multi-encoder initialization
    if (test_mode & 0x01) {
        // Consume number of encoders (1-4 for fuzzing efficiency, actual limit is 16)
        int num_encoders = fdp.ConsumeIntegralInRange<int>(1, 4);
        
        // Need enough data for configurations and down-sampling factors
        size_t min_needed = num_encoders * (sizeof(vpx_codec_enc_cfg_t) + sizeof(vpx_rational_t)) / 8;
        if (fdp.remaining_bytes() < min_needed) {
            return 0;
        }
        
        // Allocate arrays for multi-encoder
        std::vector<vpx_codec_ctx_t> contexts(num_encoders);
        std::vector<vpx_codec_enc_cfg_t> configs(num_encoders);
        std::vector<vpx_rational_t> downsampling_factors(num_encoders);
        
        // Get default config for VP8 encoder interface
        vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
        
        // Initialize configurations with defaults
        for (int i = 0; i < num_encoders; i++) {
            vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &configs[i], 0);
            if (res != VPX_CODEC_OK) {
                return 0;
            }
            
            // Modify configuration with fuzzed data
            configs[i].g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
            configs[i].g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
            configs[i].rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
            configs[i].g_timebase.num = 1;
            configs[i].g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
            configs[i].g_error_resilient = fdp.ConsumeBool() ? 1 : 0;
            configs[i].g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
            
            // Set down-sampling factors (num: 1-4096, den: 1-num)
            downsampling_factors[i].num = fdp.ConsumeIntegralInRange<int>(1, 4096);
            downsampling_factors[i].den = fdp.ConsumeIntegralInRange<int>(1, downsampling_factors[i].num);
        }
        
        // Determine flags from fuzzed input
        vpx_codec_flags_t flags = 0;
        if (fdp.ConsumeBool()) {
            flags |= VPX_CODEC_USE_PSNR;
        }
        if (fdp.ConsumeBool()) {
            flags |= VPX_CODEC_USE_HIGHBITDEPTH;
        }
        
        // Try multi-encoder initialization
        vpx_codec_err_t err = vpx_codec_enc_init_multi_ver(
            &contexts[0], iface, &configs[0], num_encoders, flags, 
            &downsampling_factors[0], VPX_ENCODER_ABI_VERSION);
        
        // Also test the convenience macro
        if (err != VPX_CODEC_OK) {
            // Try with different ABI version to test error path
            err = vpx_codec_enc_init_multi_ver(
                &contexts[0], iface, &configs[0], num_encoders, flags,
                &downsampling_factors[0], VPX_ENCODER_ABI_VERSION - 1);
        }
        
        // If initialization succeeded, test configuration changes
        if (err == VPX_CODEC_OK) {
            // Test vpx_codec_enc_config_set on each encoder
            for (int i = 0; i < num_encoders && fdp.remaining_bytes() >= 4; i++) {
                // Create modified configuration
                vpx_codec_enc_cfg_t modified_config = configs[i];
                modified_config.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(50, 2000);
                modified_config.g_w = fdp.ConsumeIntegralInRange<unsigned int>(8, 2048);
                modified_config.g_h = fdp.ConsumeIntegralInRange<unsigned int>(8, 2048);
                
                vpx_codec_err_t set_err = vpx_codec_enc_config_set(&contexts[i], &modified_config);
                // Accept any result - we're testing the API call itself
                
                // Test with NULL config (should fail)
                if (fdp.ConsumeBool()) {
                    vpx_codec_enc_config_set(&contexts[i], nullptr);
                }
            }
            
            // Clean up all encoders
            for (int i = 0; i < num_encoders; i++) {
                vpx_codec_destroy(&contexts[i]);
            }
        }
        
    } else {
        // Test individual API functions
        
        // Test vpx_codec_enc_config_default error paths
        vpx_codec_enc_cfg_t cfg;
        
        // Test with NULL interface
        vpx_codec_enc_config_default(nullptr, &cfg, 0);
        
        // Test with NULL config
        vpx_codec_enc_config_default(vpx_codec_vp8_cx(), nullptr, 0);
        
        // Test with invalid usage (should fail)
        vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 1);
        
        // Test successful default config
        vpx_codec_err_t res = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
        if (res == VPX_CODEC_OK) {
            // Modify config with fuzzed data
            cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
            cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
            cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
            
            // Test vpx_codec_enc_config_set with single encoder
            vpx_codec_ctx_t codec;
            vpx_codec_err_t init_err = vpx_codec_enc_init(&codec, vpx_codec_vp8_cx(), &cfg, 0);
            
            if (init_err == VPX_CODEC_OK) {
                // Try to change configuration
                vpx_codec_enc_cfg_t new_cfg = cfg;
                new_cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(50, 2000);
                new_cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(8, 2048);
                new_cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(8, 2048);
                
                vpx_codec_enc_config_set(&codec, &new_cfg);
                
                // Test error paths
                vpx_codec_enc_config_set(nullptr, &new_cfg);  // NULL context
                vpx_codec_enc_config_set(&codec, nullptr);    // NULL config
                
                vpx_codec_destroy(&codec);
            }
        }
        
        // Test down-sampling factor structure
        vpx_rational_t dsf;
        dsf.num = fdp.ConsumeIntegralInRange<int>(1, 4096);
        dsf.den = fdp.ConsumeIntegralInRange<int>(1, dsf.num);
        
        // Test array of down-sampling factors
        int num_dsf = fdp.ConsumeIntegralInRange<int>(1, 4);
        std::vector<vpx_rational_t> dsf_array(num_dsf);
        for (int i = 0; i < num_dsf && fdp.remaining_bytes() >= 8; i++) {
            dsf_array[i].num = fdp.ConsumeIntegralInRange<int>(1, 4096);
            dsf_array[i].den = fdp.ConsumeIntegralInRange<int>(1, dsf_array[i].num);
        }
    }
    
    return 0;
}

} // namespace harness_002
