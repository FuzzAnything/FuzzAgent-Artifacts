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

namespace harness_023 {
/*
 * Fuzzing harness for libvpx targeting VP8 encoder temporal filtering functions
 * Focus: VP8 temporal filtering functions with only 38.14% line coverage (vs overall 49.46%)
 * Specifically target vp8_temporal_filter_prepare_c, vp8_temporal_filter_apply_c, 
 * vp8_temporal_filter_iterate_c to reach uncovered temporal filtering code paths.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_023.cpp targeting coverage gaps in VP8 encoder temporal filtering
 *       functions by configuring encoder for alternative reference frames and golden
 *       frame updates that trigger temporal filtering operations.
 * 
 * APIs to target:
 * - vpx_codec_enc_config_default (VP8 encoder config)
 * - vpx_img_alloc (image allocation for frames)
 * - vpx_codec_enc_init_ver (VP8 encoder initialization)
 * - vpx_codec_control_ (set encoder controls for temporal filtering)
 * - vpx_codec_encode (frame encoding to trigger filtering)
 * - vpx_codec_destroy (cleanup)
 * - vpx_img_free (image cleanup)
 * 
 * Specific temporal filtering functions targeted:
 * - vp8_temporal_filter_prepare_c: Main temporal filtering preparation
 * - vp8_temporal_filter_apply_c: Core filtering application
 * - vp8_temporal_filter_iterate_c: Iterative filtering processing
 * - vp8_temporal_filter_predictors_mb_c: Motion prediction for filtering
 * 
 * Coverage gap: VP8 temporal filtering functions have only 38.14% line coverage
 * vs overall 49.46% coverage. These functions handle alternative reference frame
 * filtering and golden frame quality improvements.
 * 
 * Strategy to trigger temporal filtering:
 * 1. Enable multi-pass encoding (two-pass) which uses temporal filtering
 * 2. Configure alternative reference frames (alt-ref)
 * 3. Set appropriate keyframe and golden frame intervals
 * 4. Use low bitrate or quality settings that benefit from temporal filtering
 * 5. Exercise different strength and filter weight parameters
 * 
 * Semantic differentiation from existing harnesses:
 * 1. Specifically targets VP8 encoder (not VP9)
 * 2. Focuses on temporal filtering code paths (currently at 38.14% coverage)
 * 3. Configures encoder for alternative reference frame usage
 * 4. Tests golden frame update scenarios
 * 5. Uses multi-pass encoding to trigger temporal filtering
 * 
 * Invocation sequence:
 * 1. Initialize image buffers for source frames
 * 2. Get default VP8 encoder configuration
 * 3. Configure for temporal filtering: multi-pass, alt-ref, appropriate intervals
 * 4. Initialize VP8 encoder
 * 5. Set encoder controls for temporal filtering parameters
 * 6. Encode multiple frames to trigger temporal filtering operations
 * 7. Cleanup resources
 * 
 * Key parameters to fuzz for temporal filtering:
 * - g_pass: VPX_RC_FIRST_PASS, VPX_RC_LAST_PASS (multi-pass enables filtering)
 * - rc_end_usage: VPX_VBR, VPX_CBR (affects filtering decisions)
 * - kf_mode: VPX_KF_AUTO, VPX_KF_DISABLED
 * - kf_max_dist: Keyframe interval
 * - alt_ref_enabled: Enable alternative reference frames
 * - cpu_used: Speed setting affecting filtering quality
 * - filter strength and weight parameters
 * 
 * Reasoning: Temporal filtering is critical for VP8 quality in multi-pass encoding
 * and alternative reference frame scenarios. The low coverage (38.14%) indicates
 * these code paths are rarely exercised in existing fuzzing. By specifically
 * configuring the encoder for scenarios that use temporal filtering, we can
 * significantly increase coverage of these important quality-enhancing functions.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for encoder configuration and frame data
    const size_t MIN_SIZE = sizeof(uint8_t) * 2048 + sizeof(int) * 32 + sizeof(unsigned int) * 16;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize encoder interface for VP8 (targeting VP8 specifically)
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }
    
    // Create image buffer for source frames
    vpx_image_t* raw = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 320, 240, 1);
    if (!raw) {
        return 0;
    }
    
    // Fill image with fuzzed data for Y, U, V planes
    size_t y_size = 320 * 240;
    size_t uv_size = (320 / 2) * (240 / 2);
    
    if (fdp.remaining_bytes() < y_size + 2 * uv_size) {
        vpx_img_free(raw);
        return 0;
    }
    
    std::vector<uint8_t> y_plane = fdp.ConsumeBytes<uint8_t>(y_size);
    std::vector<uint8_t> u_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    std::vector<uint8_t> v_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    
    memcpy(raw->planes[0], y_plane.data(), y_size);
    memcpy(raw->planes[1], u_plane.data(), uv_size);
    memcpy(raw->planes[2], v_plane.data(), uv_size);
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(raw);
        return 0;
    }
    
    // Consume configuration parameters from fuzzer input
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Configure for temporal filtering scenarios
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Set lag in frames to enable lookahead (required for temporal filtering)
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    
    // Set pass mode to trigger temporal filtering (multi-pass encoding)
    cfg.g_pass = (config_byte & 0x01) ? VPX_RC_FIRST_PASS : VPX_RC_LAST_PASS;
    
    // Set rate control mode
    cfg.rc_end_usage = static_cast<vpx_rc_mode>(fdp.ConsumeIntegralInRange<int>(0, 3) % 4);
    
    // Set target bitrate (affects filtering decisions)
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
    
    // Set keyframe interval (affects temporal filtering opportunities)
    cfg.kf_mode = static_cast<vpx_kf_mode>(fdp.ConsumeIntegralInRange<int>(0, 2) % 3);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(30, 300);
    
    // Set minimum and maximum quantizer (affects filtering quality)
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
    
    // Initialize encoder
    vpx_codec_ctx_t codec;
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(raw);
        return 0;
    }
    
    // Apply encoder controls to influence temporal filtering
    // Set CPU used (speed/quality tradeoff)
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    
    // Enable auto alt-ref (critical for temporal filtering)
    int enable_auto_alt_ref = fdp.ConsumeBool() ? 1 : 0;
    vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    
    // Set noise sensitivity (affects filtering)
    int noise_sensitivity = fdp.ConsumeIntegralInRange<int>(0, 6);
    vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    
    // Set sharpness (affects loop filtering which interacts with temporal filtering)
    int sharpness = fdp.ConsumeIntegralInRange<int>(0, 7);
    vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
    
    // Set static threshold (for scene change detection)
    int static_thresh = fdp.ConsumeIntegralInRange<int>(0, 1000);
    vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_thresh);
    
    // Enable/disable token parts
    int token_partitions = fdp.ConsumeIntegralInRange<int>(0, 3);
    vpx_codec_control(&codec, VP8E_SET_TOKEN_PARTITIONS, token_partitions);
    
    // Set ARNR (AltRef Noise Reduction) parameters - directly related to temporal filtering
    int arnr_max_frames = fdp.ConsumeIntegralInRange<int>(0, 15);
    int arnr_strength = fdp.ConsumeIntegralInRange<int>(0, 6);
    int arnr_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_max_frames);
    vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    vpx_codec_control(&codec, VP8E_SET_ARNR_TYPE, arnr_type);
    
    // Set tuning for screen content (affects filtering decisions)
    int tuning = fdp.ConsumeBool() ? VP8_TUNE_PSNR : VP8_TUNE_SSIM;
    vpx_codec_control(&codec, VP8E_SET_TUNING, tuning);
    
    // Encode multiple frames to trigger temporal filtering
    // Temporal filtering often happens after multiple frames or on golden frames
    int num_frames = fdp.ConsumeIntegralInRange<int>(2, 10);
    for (int i = 0; i < num_frames && fdp.remaining_bytes() > 0; i++) {
        // Update image data slightly for each frame
        if (fdp.remaining_bytes() >= 100) {
            std::vector<uint8_t> frame_update = fdp.ConsumeBytes<uint8_t>(100);
            // Mix some noise into the image
            for (size_t j = 0; j < std::min(frame_update.size(), y_size); j++) {
                raw->planes[0][j] ^= frame_update[j % frame_update.size()];
            }
        }
        
        // Set frame flags to influence temporal filtering
        vpx_enc_frame_flags_t flags = 0;
        
        // Occasionally force keyframe
        if (fdp.ConsumeIntegralInRange<int>(0, 99) < 10) {
            flags |= VPX_EFLAG_FORCE_KF;
        }
        
        // Encode the frame
        err = vpx_codec_encode(&codec, raw, i, 1, flags, VPX_DL_REALTIME);
        
        if (err == VPX_CODEC_OK) {
            // Retrieve encoded data (this may trigger additional processing)
            const vpx_codec_cx_pkt_t* pkt = NULL;
            vpx_codec_iter_t iter = NULL;
            
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process packets if needed
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Frame encoded successfully
                }
            }
        }
        
        // Occasionally flush the encoder
        if (fdp.ConsumeIntegralInRange<int>(0, 99) < 20) {
            err = vpx_codec_encode(&codec, NULL, i, 1, 0, VPX_DL_REALTIME);
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&codec);
    vpx_img_free(raw);
    
    return 0;
}

} // namespace harness_023
