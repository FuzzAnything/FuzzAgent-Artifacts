#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

namespace harness_012 {
/*
 * Fuzzing harness for libvpx targeting VP9 encoder configuration validation 
 * and inter-mode decision logic to unlock hundreds of blocked branches
 * 
 * Target: Unlock 281 blocked branches in validate_config function (vp9_cx_iface.c)
 *         Unlock 284 blocked branches in vp9_pick_inter_mode function
 * 
 * Critical blocked branches to unlock:
 * 1. validate_config: Configuration validation paths for various parameter combinations
 * 2. vp9_pick_inter_mode: Encoding mode decision logic with different frame types
 * 
 * Goal: Exercise VP9 encoder with diverse valid configuration combinations to
 *       trigger different validation paths and encoding decisions
 * 
 * APIs to target:
 * - vpx_codec_enc_config_default (get default configuration)
 * - vpx_codec_enc_init_ver (initialize encoder with varied configurations)
 * - vpx_codec_enc_config_set (trigger configuration validation)
 * - vpx_codec_encode (execute encoding with different modes)
 * - vpx_codec_get_cx_data (retrieve encoded packets)
 * - vpx_codec_destroy (cleanup)
 * 
 * Configuration parameters to vary:
 * - Profiles: 0, 1, 2, 3 (different VP9 profiles)
 * - Bit depths: 8, 10, 12 bits (with VPX_CODEC_USE_HIGHBITDEPTH flag)
 * - Color spaces: I420, I422, I444, NV12
 * - SVC configurations: spatial and temporal layers
 * - Rate control modes: VBR, CBR, CQ, Q (constant quality)
 * - Lag in frames: 0-25 (0 for real-time, >0 for lookahead)
 * - Tile configurations: columns and rows
 * - Alt-ref settings: enabled/disabled with different intervals
 * 
 * Invocation sequence:
 * 1. Get default VP9 encoder configuration with vpx_codec_enc_config_default
 * 2. Systematically vary configuration parameters to create diverse valid combinations
 * 3. Initialize encoder with each configuration variant using vpx_codec_enc_init_ver
 * 4. Call vpx_codec_enc_config_set to trigger validate_config with different states
 * 5. Create input frames with formats matching configuration
 * 6. Encode multiple frames with vpx_codec_encode to exercise vp9_pick_inter_mode
 * 7. Retrieve encoded data with vpx_codec_get_cx_data
 * 8. Cleanup with vpx_codec_destroy
 * 
 * Semantic differentiation:
 * Unlike harness_010 which tests advanced encoder features, this harness focuses
 * specifically on configuration validation logic and inter-mode decision paths.
 * Unlike harness_011 which targets high-bit-depth DSP functions, this harness
 * systematically tests configuration parameter combinations that trigger blocked
 * branches in validate_config and vp9_pick_inter_mode.
 * 
 * Expected coverage gain:
 * - Unlock 281 blocked branches in validate_config function
 * - Unlock 284 blocked branches in vp9_pick_inter_mode function
 * - Improve encoder module coverage by exercising validation and decision logic
 * - Cover configuration combinations currently untouched by existing fuzzers
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for configuration parameters and frame data
    const size_t MIN_SIZE = sizeof(uint8_t) * 256 + sizeof(int) * 64 + sizeof(unsigned int) * 128;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration selector to determine which parameter set to test
    uint8_t config_selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Initialize encoder interface for VP9
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Step 1: Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 2: Modify configuration parameters based on fuzzer input
    
    // Determine profile (0-3)
    uint8_t profile_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    cfg.g_profile = profile_selector;
    
    // Determine bit depth and set appropriate flags
    uint8_t bit_depth_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
    vpx_codec_flags_t flags = 0;
    
    switch (bit_depth_selector) {
        case 0:
            cfg.g_bit_depth = VPX_BITS_8;
            cfg.g_input_bit_depth = 8;
            flags = 0;
            break;
        case 1:
            cfg.g_bit_depth = VPX_BITS_10;
            cfg.g_input_bit_depth = 10;
            flags = VPX_CODEC_USE_HIGHBITDEPTH;
            break;
        case 2:
            cfg.g_bit_depth = VPX_BITS_12;
            cfg.g_input_bit_depth = 12;
            flags = VPX_CODEC_USE_HIGHBITDEPTH;
            break;
    }
    
    // Determine image format/color space
    uint8_t color_space_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (color_space_selector) {
        case 0:
            cfg.g_profile = 0;  // Profile 0 for 4:2:0
            break;
        case 1:
            if (cfg.g_profile < 1) cfg.g_profile = 1;  // Profile 1+ for 4:4:4
            break;
        case 2:
            if (cfg.g_profile < 2) cfg.g_profile = 2;  // Profile 2+ for 4:2:2
            break;
        case 3:
            // Keep whatever profile was selected
            break;
    }
    
    // Set frame dimensions (small to medium sizes for efficiency)
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(64, 640);
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(64, 480);
    
    // Ensure dimensions are even for 4:2:0 subsampling
    if (cfg.g_profile == 0) {
        cfg.g_w = (cfg.g_w + 1) & ~1;
        cfg.g_h = (cfg.g_h + 1) & ~1;
    }
    
    // Rate control configuration
    uint8_t rc_mode_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (rc_mode_selector) {
        case 0:
            cfg.rc_end_usage = VPX_VBR;  // Variable Bit Rate
            break;
        case 1:
            cfg.rc_end_usage = VPX_CBR;  // Constant Bit Rate
            break;
        case 2:
            cfg.rc_end_usage = VPX_CQ;   // Constrained Quality
            break;
        case 3:
            cfg.rc_end_usage = VPX_Q;    // Constant Quality
            break;
    }
    
    // Target bitrate (in kbps)
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 5000);
    
    // Lag in frames (lookahead buffer)
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    
    // Temporal and spatial scalability (SVC)
    cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    
    if (cfg.ts_number_layers > 1) {
        cfg.ts_periodicity = fdp.ConsumeIntegralInRange<unsigned int>(2, 8);
        for (unsigned int i = 0; i < cfg.ts_periodicity && i < VPX_TS_MAX_PERIODICITY; ++i) {
            cfg.ts_layer_id[i] = fdp.ConsumeIntegral<unsigned int>() % cfg.ts_number_layers;
        }
        
        // Set rate decimators and target bitrates for temporal layers
        for (unsigned int i = 0; i < cfg.ts_number_layers && i < VPX_TS_MAX_LAYERS; ++i) {
            cfg.ts_rate_decimator[i] = 1 << i;  // Common pattern: 1, 2, 4, ...
            cfg.ts_target_bitrate[i] = cfg.rc_target_bitrate / (1 << i);
        }
    }
    if (cfg.ss_number_layers > 1) {
        // Set target bitrates for spatial layers
        for (unsigned int i = 0; i < cfg.ss_number_layers && i < VPX_SS_MAX_LAYERS; ++i) {
            cfg.ss_target_bitrate[i] = cfg.rc_target_bitrate / (i + 1);
            cfg.ss_enable_auto_alt_ref[i] = fdp.ConsumeBool() ? 1 : 0;
        }
    }
    
    // Alt-ref settings
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Additional encoder settings
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
    
    cfg.g_pass = fdp.ConsumeBool() ? VPX_RC_FIRST_PASS : VPX_RC_ONE_PASS;
    
    // Step 3: Initialize encoder with configuration
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 4: Call vpx_codec_enc_config_set to trigger validate_config
    // This tests the configuration validation logic
    if (vpx_codec_enc_config_set(&codec, &cfg) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set tile configuration using vpx_codec_control (as shown in harness_010.cpp)
    unsigned int tile_columns = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
    unsigned int tile_rows = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
    vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, tile_rows);
    
    // Step 5: Create input frames
    const size_t num_frames = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    std::vector<vpx_image_t*> frames;
    for (size_t frame_idx = 0; frame_idx < num_frames; ++frame_idx) {
        // Determine frame format based on profile
        vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
        if (cfg.g_profile >= 1) {
            uint8_t fmt_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (fmt_selector) {
                case 0: fmt = VPX_IMG_FMT_I420; break;
                case 1: fmt = VPX_IMG_FMT_I422; break;
                case 2: fmt = VPX_IMG_FMT_I444; break;
            }
        }
        
        if (flags & VPX_CODEC_USE_HIGHBITDEPTH) {
            fmt = static_cast<vpx_img_fmt_t>(fmt | VPX_IMG_FMT_HIGHBITDEPTH);
        }
        
        vpx_image_t* img = vpx_img_alloc(NULL, fmt, cfg.g_w, cfg.g_h, 16);
        if (!img) {
            continue;
        }
        
        // Fill image with fuzzer data
        size_t bytes_needed = img->w * img->h;
        if (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) {
            bytes_needed *= 2;  // 10/12-bit samples
        }
        
        // Account for chroma subsampling
        if (fmt == VPX_IMG_FMT_I420) {
            bytes_needed = bytes_needed * 3 / 2;
        } else if (fmt == VPX_IMG_FMT_I422) {
            bytes_needed = bytes_needed * 2;
        } else if (fmt == VPX_IMG_FMT_I444) {
            bytes_needed = bytes_needed * 3;
        }
        
        if (fdp.remaining_bytes() < bytes_needed) {
            vpx_img_free(img);
            continue;
        }
        
        // Fill planes with fuzzer data
        std::vector<uint8_t> frame_data = fdp.ConsumeBytes<uint8_t>(bytes_needed);
        
        if (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) {
            // For high bit depth, we need to fill 16-bit samples
            // Simplified: just copy data as-is
            for (int plane = 0; plane < 3; ++plane) {
                if (img->planes[plane]) {
                    size_t plane_size = img->stride[plane] * (plane == 0 ? img->h : (img->h + 1) >> 1);
                    size_t copy_size = std::min(plane_size, frame_data.size() / 3);
                    memcpy(img->planes[plane], frame_data.data() + (plane * copy_size), copy_size);
                }
            }
        } else {
            // Standard 8-bit filling
            for (int plane = 0; plane < 3; ++plane) {
                if (img->planes[plane]) {
                    size_t plane_size = img->stride[plane] * (plane == 0 ? img->h : (img->h + 1) >> 1);
                    size_t copy_size = std::min(plane_size, frame_data.size() / 3);
                    memcpy(img->planes[plane], frame_data.data() + (plane * copy_size), copy_size);
                }
            }
        }
        
        frames.push_back(img);
    }
    
    // Step 6: Encode frames to exercise vp9_pick_inter_mode
    for (size_t i = 0; i < frames.size(); ++i) {
        vpx_enc_frame_flags_t frame_flags = 0;
        
        // Set keyframe flag for first frame or based on fuzzer input
        if (i == 0 || fdp.ConsumeBool()) {
            frame_flags = VPX_EFLAG_FORCE_KF;
        }
        
        // Determine frame duration
        unsigned long duration = 1;
        
        // Encode the frame
        if (vpx_codec_encode(&codec, frames[i], i * duration, duration, frame_flags, VPX_DL_REALTIME) != VPX_CODEC_OK) {
            // Continue with next frame even if this one fails
            continue;
        }
        
        // Step 7: Retrieve encoded data
        const vpx_codec_cx_pkt_t* pkt = NULL;
        vpx_codec_iter_t iter = NULL;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame packet received - success
                // We don't need to do anything with the data for fuzzing
            }
        }
    }
    
    // Step 8: Cleanup
    for (vpx_image_t* img : frames) {
        vpx_img_free(img);
    }
    
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_012
