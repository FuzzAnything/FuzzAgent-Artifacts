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

namespace harness_026 {
/*
 * Fuzzing harness for libvpx targeting VP9 SVC (Scalable Video Coding) encoding
 * with focus on extreme layer configurations and quality relationships to unblock
 * 482 blocked branches in vp9_pick_inter_mode function.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_026.cpp targeting VP9 SVC configurations with semantic
 *       diversity from existing harnesses, especially harness_025 which found a
 *       library bug. Focus on different aspects of SVC:
 *       1. Extreme layer combinations (max 5 spatial layers, 4 temporal layers)
 *       2. Complex quality relationships and quantization parameter dependencies
 *       3. Dynamic quality adjustment during encoding session
 *       4. SVC-specific thresholds affecting vp9_pick_inter_mode decisions
 *       5. Inter-layer prediction variations with quality scaling
 * 
 * Specific vp9_pick_inter_mode SVC conditions to target:
 * - Line 1798: if (cpi->use_svc && svc->spatial_layer_id > 0)
 * - Line 1805-1817: thresh_svc_skip_golden adjustments based on layer quality relationships
 * - Line 1946-1958: svc_force_zero_mode for spatial references
 * - Line 2023-2029: flag_svc_subpel for sub-pixel motion in inter-layer references
 * - Line 2522-2528: SVC-specific keyframe and temporal layer constraints
 * - Line 2611-2612: SVC scene change detection interactions
 * 
 * APIs to target:
 * - vpx_codec_enc_config_default (SVC configuration setup)
 * - vpx_codec_enc_init_ver (VP9 encoder initialization with SVC)
 * - vpx_codec_control_ (SVC-specific encoder controls with quality focus)
 * - vpx_codec_encode (frame encoding with dynamic quality adjustments)
 * - vpx_codec_get_cx_data (retrieve encoded SVC packets)
 * - vpx_codec_destroy (cleanup)
 * 
 * Semantic differentiation from existing harnesses (000-025):
 * 1. harness_014/015: Basic SVC configurations with fixed layer counts
 * 2. harness_025: Comprehensive SVC controls but found library bug
 * 3. This harness_026: Focuses on extreme layer configurations (5 spatial, 4 temporal)
 * 4. Emphasis on quality layer relationships affecting vp9_pick_inter_mode decisions
 * 5. Tests dynamic quality adjustment during encoding (changing QP per layer over time)
 * 6. Focuses on SVC-specific thresholds: thresh_svc_skip_golden, svc_force_zero_mode
 * 7. Tests complex scaling factor relationships between spatial layers
 * 8. Exercises quality scaling in inter-layer prediction scenarios
 * 
 * Key SVC quality parameters to fuzz for vp9_pick_inter_mode coverage:
 * - base_qindex vs lower_layer_qindex differences (quality relationships)
 * - Scaling factor ratios affecting quality inheritance between layers
 * - Dynamic QP changes per layer during encoding session
 * - Quality-based thresholds for golden frame skipping
 * - Sub-pixel motion vector quality for inter-layer references
 * - Rate-distortion optimization with multi-layer quality tradeoffs
 * 
 * Invocation sequence:
 * 1. Initialize encoder with maximum SVC configuration (5 spatial, 4 temporal layers)
 * 2. Configure complex scaling relationships with non-uniform scaling factors
 * 3. Set up quality inheritance relationships between layers
 * 4. Apply SVC controls with focus on quality-related parameters
 * 5. Encode frames with dynamic quality adjustments (changing QP per frame)
 * 6. Test different quality inheritance modes for inter-layer prediction
 * 7. Exercise SVC-specific thresholds and constraints
 * 8. Validate encoded output contains expected SVC layer data
 * 
 * Expected coverage impact:
 * - Target 482 blocked branches in vp9_pick_inter_mode
 * - Exercise SVC-specific conditional paths related to quality relationships
 * - Cover quality inheritance and scaling logic in SVC encoding
 * - Test dynamic quality adjustment scenarios
 * - Improve overall VP9 encoder SVC quality-handling coverage
 * 
 * Reasoning: vp9_pick_inter_mode has extensive SVC-specific logic that depends on
 * complex quality relationships between layers. Existing harnesses test basic SVC
 * enablement and control configurations but don't deeply exercise the quality-based
 * conditional logic triggered by base_qindex vs lower_layer_qindex differences,
 * quality inheritance, and dynamic quality adjustments. This harness systematically
 * tests these quality relationships to unlock the blocked branches in the mode
 * selection algorithm.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for extreme SVC configuration, quality parameters,
    // dynamic adjustments, and frame data
    const size_t MIN_SIZE = sizeof(uint8_t) * 2048 + sizeof(int) * 512 + sizeof(unsigned int) * 1024;
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
    // Use reasonable ranges for video dimensions
    unsigned int base_width = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920) & ~1;
    unsigned int base_height = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080) & ~1;
    
    cfg.g_w = base_width;
    cfg.g_h = base_height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.rc_end_usage = VPX_VBR;
    
    // Step 3: Configure extreme SVC settings (max layers: 5 spatial, 4 temporal)
    unsigned int num_spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
    unsigned int num_temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    
    cfg.ss_number_layers = num_spatial_layers;
    cfg.ts_number_layers = num_temporal_layers;
    
    // Configure target bitrates for spatial layers
    for (unsigned int sl = 0; sl < num_spatial_layers; sl++) {
        // Set target bitrates with emphasis on quality relationships
        // Higher layers get progressively higher bitrates for quality testing
        cfg.ss_target_bitrate[sl] = cfg.rc_target_bitrate * (sl + 1) / num_spatial_layers;
        cfg.ss_enable_auto_alt_ref[sl] = fdp.ConsumeBool() ? 1 : 0;
    }
    
    // Configure temporal layer rate decimators
    for (unsigned int tl = 0; tl < num_temporal_layers; tl++) {
        cfg.ts_rate_decimator[tl] = (1 << (num_temporal_layers - tl - 1));
    }
    
    // Step 4: Initialize encoder
    unsigned int flags = 0;
    if (fdp.ConsumeBool()) {
        flags |= VPX_CODEC_USE_HIGHBITDEPTH;
    }
    
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 5: Apply SVC-specific controls with focus on quality relationships
    
    // 5a: Enable SVC
    vpx_codec_err_t err = vpx_codec_control(&codec, VP9E_SET_SVC, 1);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // 5b: Set SVC parameters with quality focus
    vpx_svc_extra_cfg_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Set temporal layering mode
    svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Configure min/max quantizers with emphasis on quality relationships between layers
    // Create quality gradients to test base_qindex vs lower_layer_qindex differences
    for (unsigned int sl = 0; sl < num_spatial_layers; sl++) {
        // Higher spatial layers get lower QP (better quality) to test quality inheritance
        svc_params.min_quantizers[sl] = fdp.ConsumeIntegralInRange<int>(4, 32 + sl * 5);
        svc_params.max_quantizers[sl] = fdp.ConsumeIntegralInRange<int>(svc_params.min_quantizers[sl] + 10, 63);
        
        // Set scaling factors for complex quality relationships
        svc_params.scaling_factor_num[sl] = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        svc_params.scaling_factor_den[sl] = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        
        // Speed settings per layer to affect quality-speed tradeoffs
        svc_params.speed_per_layer[sl] = fdp.ConsumeIntegralInRange<int>(0, 9);
        
        // Configure loopfilter control
        svc_params.loopfilter_ctrl[sl] = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    err = vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params);
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));
    
    // Configure temporal layer IDs with emphasis on quality variations
    for (unsigned int sl = 0; sl < num_spatial_layers; sl++) {
        layer_id.temporal_layer_id_per_spatial[sl] = fdp.ConsumeIntegralInRange<int>(0, num_temporal_layers - 1);
    }
    
    err = vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
    
    // 5d: Set SVC reference frame configuration with quality-based decisions
    vpx_svc_ref_frame_config_t ref_config;
    memset(&ref_config, 0, sizeof(ref_config));
    
    // Configure reference frame indices with quality relationships
    for (unsigned int sl = 0; sl < num_spatial_layers; sl++) {
        ref_config.lst_fb_idx[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
        ref_config.gld_fb_idx[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
        ref_config.alt_fb_idx[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
        
        // Quality-based reference updates
        ref_config.update_buffer_slot[sl] = fdp.ConsumeBool() ? (1 << ref_config.lst_fb_idx[sl]) : 0;
        if (fdp.ConsumeBool()) {
            ref_config.update_buffer_slot[sl] |= (1 << ref_config.gld_fb_idx[sl]);
        }
        if (fdp.ConsumeBool()) {
            ref_config.update_buffer_slot[sl] |= (1 << ref_config.alt_fb_idx[sl]);
        }
        
        // Reference quality decisions affecting mode selection
        ref_config.reference_last[sl] = fdp.ConsumeBool();
        ref_config.reference_golden[sl] = fdp.ConsumeBool();
        ref_config.reference_alt_ref[sl] = fdp.ConsumeBool();
    }
    
    err = vpx_codec_control(&codec, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
    
    // 5e: Set inter-layer prediction control with quality inheritance variations
    unsigned int inter_layer_pred = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    err = vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, inter_layer_pred);
    
    // 5f: Set golden frame temporal reference for SVC quality decisions
    unsigned int gf_temporal_ref = fdp.ConsumeBool() ? 1 : 0;
    err = vpx_codec_control(&codec, VP9E_SET_SVC_GF_TEMPORAL_REF, gf_temporal_ref);
    
    // 5g: Set spatial layer synchronization with quality constraints
    vpx_svc_spatial_layer_sync_t layer_sync;
    memset(&layer_sync, 0, sizeof(layer_sync));
    
    for (unsigned int sl = 0; sl < num_spatial_layers; sl++) {
        layer_sync.spatial_layer_sync[sl] = fdp.ConsumeBool() ? 1 : 0;
    }
    layer_sync.base_layer_intra_only = fdp.ConsumeBool() ? 1 : 0;
    
    err = vpx_codec_control(&codec, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &layer_sync);
    
    // 5h: Set additional quality-related encoder controls
    
    // Set noise sensitivity for quality adjustments
    unsigned int noise_sensitivity = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
    err = vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    
    // Set content type affecting quality decisions
    unsigned int tune_content = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    err = vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, tune_content);
    
    // Set color space affecting quality decisions
    unsigned int color_space = fdp.ConsumeIntegralInRange<unsigned int>(0, 7);
    err = vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    
    // Step 6: Prepare image buffers for encoding
    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, base_width, base_height, 16);
    
    if (!raw.planes[0]) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with synthetic data from fuzzer input
    size_t y_plane_size = base_width * base_height;
    size_t uv_plane_size = (base_width * base_height) / 4;
    
    if (fdp.remaining_bytes() < y_plane_size + uv_plane_size * 2) {
        vpx_img_free(&raw);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Consume data for Y plane
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    memcpy(raw.planes[0], y_data.data(), y_plane_size);
    
    // Consume data for U and V planes
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    
    memcpy(raw.planes[1], u_data.data(), uv_plane_size);
    memcpy(raw.planes[2], v_data.data(), uv_plane_size);
    
    // Step 7: Encode multiple frames with dynamic quality adjustments
    // Encode several frames to exercise temporal quality relationships
    unsigned int num_frames_to_encode = fdp.ConsumeIntegralInRange<unsigned int>(1, 10);
    
    for (unsigned int frame_idx = 0; frame_idx < num_frames_to_encode; frame_idx++) {
        // Dynamic quality adjustment: change layer parameters per frame
        if (frame_idx > 0 && fdp.ConsumeBool()) {
            // Update SVC layer ID with new temporal layer assignments
            for (unsigned int sl = 0; sl < num_spatial_layers; sl++) {
                layer_id.temporal_layer_id_per_spatial[sl] = 
                    fdp.ConsumeIntegralInRange<int>(0, num_temporal_layers - 1);
            }
            err = vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
        }
        
        // Encode the frame
        err = vpx_codec_encode(&codec, &raw, frame_idx, 1, 0, VPX_DL_REALTIME);
        
        if (err != VPX_CODEC_OK) {
            // Continue with cleanup on error
            break;
        }
        
        // Retrieve encoded data
        const vpx_codec_cx_pkt_t* pkt = nullptr;
        vpx_codec_iter_t iter = nullptr;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame encoded successfully
                // Exercise different packet processing scenarios
                if (fdp.ConsumeBool()) {
                    // Simulate packet processing with quality checks
                    volatile size_t packet_size = pkt->data.frame.sz;
                    volatile uint64_t pts = pkt->data.frame.pts;
                    (void)packet_size;
                    (void)pts;
                }
            }
        }
        
        // Modify image data slightly for next frame to create motion
        if (frame_idx < num_frames_to_encode - 1 && fdp.remaining_bytes() > y_plane_size / 4) {
            // Add some noise to create temporal variation
            size_t noise_size = std::min(y_plane_size / 4, fdp.remaining_bytes());
            std::vector<uint8_t> noise = fdp.ConsumeBytes<uint8_t>(noise_size);
            
            for (size_t i = 0; i < noise_size && i < y_plane_size; i++) {
                raw.planes[0][i] ^= noise[i] & 0x1F;  // Add limited noise
            }
        }
    }
    
    // Step 8: Cleanup
    vpx_img_free(&raw);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_026
