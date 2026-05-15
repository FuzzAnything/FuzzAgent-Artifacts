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

namespace harness_013 {
/*
 * Fuzzing harness for libvpx targeting VP9 SVC (Scalable Video Coding) encoding
 * to unblock 330 blocked branches in vp9_pick_inter_mode and related SVC functions
 * 
 * Target: Unlock 330 blocked branches in vp9_pick_inter_mode function
 *         Cover SVC-specific functions currently at 0% coverage:
 *         - vp9_rc_get_svc_params (0% covered, 112 branches)
 *         - vp9_svc_reset_temporal_layers (0% covered)
 *         - set_flags_and_fb_idx_for_temporal_mode3 (0% covered)
 *         - vp9_init_second_pass_spatial_svc (0% covered)
 * 
 * Critical blocked branches to unlock:
 * 1. vp9_pick_inter_mode: SVC-specific conditions checking:
 *    - cpi->use_svc conditions
 *    - svc->spatial_layer_id > 0 checks
 *    - svc->temporal_layer_id conditions
 *    - Layer scaling factor comparisons
 *    - Temporal reference frame validation for SVC
 * 2. SVC rate control and layer configuration logic
 * 
 * Goal: Exercise VP9 encoder with comprehensive SVC configurations to
 *       trigger SVC-specific code paths in inter-mode decision logic
 * 
 * APIs to target:
 * - vpx_codec_enc_config_default (get default configuration)
 * - vpx_codec_enc_init_ver (initialize encoder with SVC configuration)
 * - vpx_codec_enc_config_set (apply SVC-specific configuration)
 * - vpx_codec_control with VP9E_SET_SVC (enable SVC)
 * - vpx_codec_control with VP9E_SET_SVC_PARAMETERS (configure SVC layers)
 * - vpx_codec_control with VP9E_SET_SVC_LAYER_ID (set layer IDs)
 * - vpx_codec_encode (encode frames with SVC)
 * - vpx_codec_get_cx_data (retrieve encoded packets)
 * - vpx_codec_destroy (cleanup)
 * 
 * SVC Configuration parameters to test:
 * - Spatial layers: 1-4 layers (VPX_SS_MAX_LAYERS = 5)
 * - Temporal layers: 1-4 layers
 * - Temporal layering modes: NOLAYERING, BYPASS, 0101, 0212
 * - Layer scaling factors: different resolution scaling per layer
 * - Layer-specific quantizers: min/max Q values per layer
 * - SVC rate control: layer-specific bitrate targets
 * - Inter-layer prediction: enabled/disabled
 * 
 * Invocation sequence:
 * 1. Get default VP9 encoder configuration with vpx_codec_enc_config_default
 * 2. Configure SVC parameters: spatial layers, temporal layers, scaling factors
 * 3. Initialize encoder with SVC configuration using vpx_codec_enc_init_ver
 * 4. Enable SVC with vpx_codec_control(VP9E_SET_SVC, 1)
 * 5. Set SVC parameters with vpx_codec_control(VP9E_SET_SVC_PARAMETERS)
 * 6. Create input frames matching highest layer resolution
 * 7. For multiple frames with different layer configurations:
 *    - Set layer ID with vpx_codec_control(VP9E_SET_SVC_LAYER_ID)
 *    - Encode frame with vpx_codec_encode
 *    - Retrieve encoded data with vpx_codec_get_cx_data
 * 8. Cleanup with vpx_codec_destroy
 * 
 * Semantic differentiation:
 * Unlike harness_012 which tests general configuration validation, this harness
 * specifically targets SVC (Scalable Video Coding) encoding features to unlock
 * blocked branches in vp9_pick_inter_mode that check for SVC conditions.
 * This harness focuses on multi-layer encoding with spatial/temporal scalability,
 * layer-specific configurations, and SVC rate control logic.
 * 
 * Expected coverage gain:
 * - Unlock 330 blocked branches in vp9_pick_inter_mode function
 * - Cover SVC-specific functions currently at 0% coverage
 * - Improve SVC encoder module coverage by exercising layer management logic
 * - Test SVC-specific inter-mode decision paths and reference frame handling
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for SVC configuration and frame data
    const size_t MIN_SIZE = sizeof(uint8_t) * 512 + sizeof(int) * 128 + sizeof(unsigned int) * 256;
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
    
    // Step 2: Configure SVC parameters
    
    // Determine spatial and temporal layers
    unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    
    cfg.ss_number_layers = spatial_layers;
    cfg.ts_number_layers = temporal_layers;
    
    // Set base layer dimensions (small to medium sizes for efficiency)
    unsigned int base_width = fdp.ConsumeIntegralInRange<unsigned int>(64, 320);
    unsigned int base_height = fdp.ConsumeIntegralInRange<unsigned int>(64, 240);
    
    // Ensure dimensions are even for 4:2:0 subsampling
    base_width = (base_width + 1) & ~1;
    base_height = (base_height + 1) & ~1;
    
    cfg.g_w = base_width;
    cfg.g_h = base_height;
    
    // Configure layer-specific bitrate targets
    for (unsigned int i = 0; i < spatial_layers; ++i) {
        cfg.ss_target_bitrate[i] = fdp.ConsumeIntegralInRange<unsigned int>(100, 2000);
        cfg.ss_enable_auto_alt_ref[i] = fdp.ConsumeBool() ? 1 : 0;
    }
    
    // Temporal layer configuration
    if (temporal_layers > 1) {
        cfg.ts_periodicity = fdp.ConsumeIntegralInRange<unsigned int>(2, 8);
        for (unsigned int i = 0; i < cfg.ts_periodicity && i < VPX_TS_MAX_PERIODICITY; ++i) {
            cfg.ts_layer_id[i] = fdp.ConsumeIntegral<unsigned int>() % temporal_layers;
        }
        
        // Set rate decimators and target bitrates for temporal layers
        for (unsigned int i = 0; i < temporal_layers && i < VPX_TS_MAX_LAYERS; ++i) {
            cfg.ts_rate_decimator[i] = 1 << i;  // Common pattern: 1, 2, 4, ...
            cfg.ts_target_bitrate[i] = fdp.ConsumeIntegralInRange<unsigned int>(100, 2000);
        }
    }
    
    // Enable error resilience for SVC
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Set frame rate
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    
    // Lag in frames (lookahead buffer) - important for SVC temporal layers
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 16);
    
    // Step 3: Initialize encoder
    vpx_codec_flags_t flags = 0;
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 4: Enable and configure SVC
    
    // Enable SVC
    if (vpx_codec_control(&codec, VP9E_SET_SVC, 1) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Configure SVC parameters
    vpx_svc_extra_cfg_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Set temporal layering mode
    svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Configure layer-specific parameters
    for (unsigned int i = 0; i < spatial_layers; ++i) {
        // Min/Max quantizers for each layer
        svc_params.min_quantizers[i] = fdp.ConsumeIntegralInRange<int>(0, 20);
        svc_params.max_quantizers[i] = fdp.ConsumeIntegralInRange<int>(40, 63);
        
        // Scaling factors for each layer (use powers of 2 for simplicity)
        unsigned int scale_num = 1 << (fdp.ConsumeIntegralInRange<unsigned int>(0, 4));
        svc_params.scaling_factor_num[i] = scale_num;
        svc_params.scaling_factor_den[i] = 16; // Denominator fixed at 16
        
        // Speed setting per layer
        svc_params.speed_per_layer[i] = fdp.ConsumeIntegralInRange<int>(0, 9);
        
        // Loopfilter control per layer
        svc_params.loopfilter_ctrl[i] = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    // Apply SVC parameters
    if (vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 5: Create test frame(s)
    // Create a simple test image at the highest spatial layer resolution
    vpx_image_t* raw = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!raw) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with test data from fuzzer input
    size_t y_plane_size = raw->w * raw->h;
    size_t uv_plane_size = (raw->w / 2) * (raw->h / 2);
    
    if (fdp.remaining_bytes() < y_plane_size + uv_plane_size * 2) {
        vpx_img_free(raw);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill Y plane
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    memcpy(raw->planes[0], y_data.data(), y_plane_size);
    
    // Fill U plane
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    memcpy(raw->planes[1], u_data.data(), uv_plane_size);
    
    // Fill V plane
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    memcpy(raw->planes[2], v_data.data(), uv_plane_size);
    
    // Step 6: Encode multiple frames with different layer configurations
    
    int frame_count = fdp.ConsumeIntegralInRange<int>(1, 8);
    vpx_codec_pts_t pts = 0;
    int64_t duration = 1;
    unsigned long deadline = VPX_DL_REALTIME;
    
    for (int frame_idx = 0; frame_idx < frame_count; ++frame_idx) {
        // Set layer ID for this frame
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        
        // For SVC, we cycle through spatial and temporal layers
        layer_id.spatial_layer_id = frame_idx % spatial_layers;
        layer_id.temporal_layer_id = frame_idx % temporal_layers;
        
        // Set temporal layer ID for each spatial layer
        for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
            layer_id.temporal_layer_id_per_spatial[sl] = (frame_idx + sl) % temporal_layers;
        }
        
        if (vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id) != VPX_CODEC_OK) {
            break;
        }
        
        // Encode the frame
        if (vpx_codec_encode(&codec, raw, pts, duration, 0, deadline) != VPX_CODEC_OK) {
            break;
        }
        
        // Retrieve encoded data
        const vpx_codec_cx_pkt_t* pkt = NULL;
        vpx_codec_iter_t iter = NULL;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame packet received - this exercises the SVC packetization logic
                // In SVC mode, we might get multiple packets per frame (one per layer)
            }
        }
        
        pts += duration;
        
        // For some frames, test with NULL input (flush)
        if (frame_idx == frame_count / 2) {
            if (vpx_codec_encode(&codec, NULL, pts, 0, 0, deadline) != VPX_CODEC_OK) {
                break;
            }
            
            // Retrieve flush data
            iter = NULL;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process flushed packets
            }
        }
    }
    
    // Step 7: Test additional SVC controls if we have enough input
    
    if (fdp.remaining_bytes() > 100) {
        // Test frame dropping configuration
        vpx_svc_frame_drop_t frame_drop;
        memset(&frame_drop, 0, sizeof(frame_drop));
        int drop_mode_val = fdp.ConsumeIntegralInRange<int>(0, 3);
        frame_drop.framedrop_mode = static_cast<SVC_LAYER_DROP_MODE>(drop_mode_val);
        frame_drop.max_consec_drop = fdp.ConsumeIntegralInRange<int>(0, 10);
        
        for (unsigned int i = 0; i < spatial_layers; ++i) {
            frame_drop.framedrop_thresh[i] = fdp.ConsumeIntegralInRange<int>(0, 100);
        }
        
        // Note: This control might fail if encoder is in wrong state, but that's OK for fuzzing
        vpx_codec_control(&codec, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
        
        // Test spatial layer sync
        vpx_svc_spatial_layer_sync_t spatial_sync;
        memset(&spatial_sync, 0, sizeof(spatial_sync));
        
        spatial_sync.base_layer_intra_only = fdp.ConsumeBool() ? 1 : 0;
        for (unsigned int i = 0; i < spatial_layers; ++i) {
            spatial_sync.spatial_layer_sync[i] = fdp.ConsumeBool() ? 1 : 0;
        }
        
        vpx_codec_control(&codec, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &spatial_sync);
    }
    
    // Step 8: Cleanup
    vpx_img_free(raw);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_013
