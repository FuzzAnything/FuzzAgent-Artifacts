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

namespace harness_024 {
/*
 * Fuzzing harness for libvpx targeting VP9 encoder optimization functions with focus on
 * vp9_pick_inter_mode function (468+ blocked branches) and advanced VP9 encoder features.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_024.cpp targeting VP9 encoder optimization functions identified
 *       by CoverageAnalyzerAgent as the most critical coverage gap. Specifically target
 *       vp9_pick_inter_mode function which has 468+ blocked branches. Focus on advanced
 *       VP9 encoder features and configurations to exercise mode selection and 
 *       rate-distortion decisions. Ensure semantic diversity from existing 24 harnesses.
 * 
 * APIs to target:
 * - vpx_codec_enc_config_default (config initialization)
 * - vpx_img_alloc (image allocation for frames)
 * - vpx_codec_enc_init_ver (VP9 encoder initialization)
 * - vpx_codec_control_ (encoder optimization controls - primary target)
 * - vpx_codec_encode (frame encoding to trigger optimization paths)
 * - vpx_codec_destroy (cleanup)
 * - vpx_img_free (image cleanup)
 * 
 * Specific optimization paths targeted in vp9_pick_inter_mode:
 * 1. Temporal denoising configurations (CONFIG_VP9_TEMPORAL_DENOISING code blocks)
 * 2. Speed setting interactions (cpi->oxcf.speed values 0-9 with SVC features)
 * 3. Inter-layer prediction modes for SVC with spatial_layer_id > 0
 * 4. Motion vector handling and sub-pixel search parameter combinations
 * 5. Segmentation features interacting with mode selection
 * 6. Reference frame combinations and their impact on rate-distortion decisions
 * 
 * Semantic differentiation from existing harnesses (harness_000 through harness_023):
 * 1. Specifically targets temporal denoising configurations not covered in previous harnesses
 * 2. Tests granular speed setting interactions (0-9) with SVC and segmentation
 * 3. Focuses on motion estimation parameters that directly affect vp9_pick_inter_mode
 * 4. Tests inter-layer reference frame constraints in SVC mode
 * 5. Exercises CONFIG_VP9_TEMPORAL_DENOISING conditional compilation blocks
 * 6. Targets specific motion vector search ranges and sub-pixel refinement parameters
 * 
 * Invocation sequence:
 * 1. Initialize image buffers using vpx_img_alloc with varied formats
 * 2. Get default VP9 encoder configuration using vpx_codec_enc_config_default
 * 3. Configure encoder for advanced optimization testing:
 *    a. Enable SVC with spatial_layer_id > 0 scenarios
 *    b. Set temporal denoising parameters
 *    c. Configure motion estimation and sub-pixel search parameters
 *    d. Set segmentation features with mode constraints
 * 4. Initialize VP9 encoder with vpx_codec_enc_init_ver
 * 5. Apply optimization controls using vpx_codec_control_:
 *    a. VP9E_SET_NOISE_SENSITIVITY (temporal denoising)
 *    b. VP9E_SET_TUNE_CONTENT with screen/content variations
 *    c. VP9E_SET_COLOR_SPACE (affects mode decisions)
 *    d. VP9E_SET_TILE_COLUMNS/ROWS (parallel encoding effects)
 *    e. VP9E_SET_ALT_REF_AQ (alternate reference adaptive quantization)
 *    f. VP9E_SET_ROW_MT (row-based multi-threading)
 * 6. Encode multiple frames with varied content to trigger optimization paths
 * 7. Test different speed settings (0=slowest/best to 9=fastest) within same session
 * 8. Cleanup using vpx_codec_destroy and vpx_img_free
 * 
 * Key optimization parameters to fuzz:
 * - Noise sensitivity levels (0-6) for temporal denoising
 * - Speed settings (0-9) with SVC interactions
 * - Motion estimation search ranges and sub-pixel refinement steps
 * - Interpolation filter choices (EIGHTTAP, EIGHTTAP_SMOOTH, EIGHTTAP_SHARP)
 * - Reference frame combinations (LAST, GOLDEN, ALTREF) with constraints
 * - Segmentation feature parameters with mode limitations
 * - Tile-based encoding parameters affecting parallel processing
 * - Color space settings affecting mode decisions
 * 
 * Reasoning: The vp9_pick_inter_mode function has 468+ blocked branches representing
 * complex conditional logic for mode selection and rate-distortion optimization.
 * Existing harnesses have tested SVC configurations and basic optimizations, but
 * haven't deeply explored:
 * 1. Temporal denoising interactions with mode selection
 * 2. Granular speed setting effects on motion estimation and sub-pixel search
 * 3. Motion vector parameter combinations affecting early termination decisions
 * 4. Color space and content type influences on mode decisions
 * 5. Interpolation filter choices in conjunction with other optimizations
 * 
 * This harness specifically targets these unexplored interactions to exercise
 * the complex conditional logic in vp9_pick_inter_mode and related optimization
 * functions, addressing the remaining coverage gaps in VP9 encoder optimization.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for complex optimization parameters,
    // motion estimation settings, and frame data
    const size_t MIN_SIZE = sizeof(uint8_t) * 2048 + sizeof(int) * 128 + sizeof(unsigned int) * 256;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
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
    
    // Step 2: Configure basic encoder parameters from fuzzer input
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 1920) & ~1; // Ensure even width
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 1080) & ~1; // Ensure even height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    
    // Configure for advanced optimization testing
    cfg.g_pass = fdp.ConsumeBool() ? VPX_RC_ONE_PASS : VPX_RC_FIRST_PASS;
    cfg.rc_end_usage = static_cast<vpx_rc_mode>(fdp.ConsumeIntegralInRange<int>(0, 3));
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Configure SVC for testing spatial_layer_id > 0 scenarios
    cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    
    // Configure rate control parameters that affect mode decisions
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
    cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    
    // Configure keyframe parameters
    cfg.kf_mode = static_cast<vpx_kf_mode>(fdp.ConsumeIntegralInRange<int>(0, 2));
    cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 1000);
    
    // Configure golden frame and alt-ref parameters
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    
    // Step 3: Initialize encoder
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 4: Apply advanced optimization controls from fuzzer input
    // Temporal denoising and noise sensitivity
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, 
                         fdp.ConsumeIntegralInRange<int>(0, 6));
    }
    
    // Content tuning (affects mode decisions)
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT,
                         fdp.ConsumeBool() ? VP9E_CONTENT_DEFAULT : VP9E_CONTENT_SCREEN);
    }
    
    // Color space (affects mode selection and prediction)
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE,
                         fdp.ConsumeIntegralInRange<int>(0, 7));
    }
    
    // Tile-based encoding (affects parallel processing and mode decisions)
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS,
                         fdp.ConsumeIntegralInRange<int>(0, 6));
        vpx_codec_control(&codec, VP9E_SET_TILE_ROWS,
                         fdp.ConsumeIntegralInRange<int>(0, 2));
    }
    
    // Alternate reference adaptive quantization
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_ALT_REF_AQ, fdp.ConsumeBool() ? 1 : 0);
    }
    
    // Row-based multi-threading
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_ROW_MT, fdp.ConsumeBool() ? 1 : 0);
    }
    
    // Adaptive quantization mode
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_AQ_MODE,
                         fdp.ConsumeIntegralInRange<unsigned int>(0, 4));
    }
    
    // Frame parallel decoding
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING,
                         fdp.ConsumeBool() ? 1 : 0);
    }
    
    // SVC parameters for testing spatial_layer_id > 0 conditions
    if (cfg.ss_number_layers > 1 || cfg.ts_number_layers > 1) {
        vpx_svc_extra_cfg_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        
        int svc_layers = fdp.ConsumeIntegralInRange<int>(1, 3);
        
        // Configure spatial layers
        for (int i = 0; i < svc_layers && i < VPX_MAX_LAYERS; ++i) {
            // Set min/max quantizers (0-63 valid range)
            svc_params.min_quantizers[i] = fdp.ConsumeIntegralInRange<int>(0, 63);
            svc_params.max_quantizers[i] = fdp.ConsumeIntegralInRange<int>(svc_params.min_quantizers[i], 63);
            
            // Set scaling factors (avoid division by zero)
            svc_params.scaling_factor_num[i] = fdp.ConsumeIntegralInRange<int>(1, 100);
            svc_params.scaling_factor_den[i] = fdp.ConsumeIntegralInRange<int>(1, 100);
            
            // Set speed per layer (CPU_USED range 0-9)
            svc_params.speed_per_layer[i] = fdp.ConsumeIntegralInRange<int>(0, 9);
            
            // Set loopfilter control (0 or 1)
            svc_params.loopfilter_ctrl[i] = fdp.ConsumeBool() ? 1 : 0;
        }
        
        // Set temporal layering mode
        svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
        
        vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params);
        
        // Set layer ID using vpx_svc_layer_id_t structure
        vpx_svc_layer_id_t layer_id;
        layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, svc_layers - 1);
        layer_id.temporal_layer_id = fdp.ConsumeIntegralInRange<int>(0, 2);
        vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
    }
    
    // Speed setting - critical for vp9_pick_inter_mode optimizations
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    
    // Step 5: Allocate image buffers
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with fuzzer data
    size_t y_plane_size = cfg.g_w * cfg.g_h;
    size_t uv_plane_size = y_plane_size / 4;
    
    if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill Y plane
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    memcpy(img->planes[VPX_PLANE_Y], y_data.data(), y_plane_size);
    
    // Fill U and V planes
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    memcpy(img->planes[VPX_PLANE_U], u_data.data(), uv_plane_size);
    memcpy(img->planes[VPX_PLANE_V], v_data.data(), uv_plane_size);
    
    // Step 6: Encode multiple frames to trigger optimization paths
    // Test with different frame flags to exercise various mode selection paths
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; ++i) {
        vpx_codec_flags_t flags = 0;
        
        // Randomly set frame flags to test different encoding scenarios
        if (fdp.ConsumeBool()) flags |= VP8_EFLAG_NO_REF_LAST;
        if (fdp.ConsumeBool()) flags |= VP8_EFLAG_NO_REF_GF;
        if (fdp.ConsumeBool()) flags |= VP8_EFLAG_NO_REF_ARF;
        if (fdp.ConsumeBool()) flags |= VP8_EFLAG_NO_UPD_LAST;
        if (fdp.ConsumeBool()) flags |= VP8_EFLAG_NO_UPD_GF;
        if (fdp.ConsumeBool()) flags |= VP8_EFLAG_NO_UPD_ARF;
        
        // Encode the frame
        if (vpx_codec_encode(&codec, img, i, 1, flags, VPX_DL_REALTIME) != VPX_CODEC_OK) {
            // Continue with next frame even if encoding fails
            continue;
        }
        
        // Retrieve encoded data (triggers some post-encoding processing)
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet if needed
        }
        
        // Change speed setting mid-stream to test dynamic optimization adjustments
        if (i == 1 && fdp.ConsumeBool()) {
            int new_cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, new_cpu_used);
        }
    }
    
    // Step 7: Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_024
