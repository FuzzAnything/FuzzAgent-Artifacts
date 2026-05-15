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

namespace harness_014 {
/*
 * Fuzzing harness for libvpx targeting VP9 SVC (Scalable Video Coding) encoding
 * with deeper complexity to unblock ~1,700+ undiscovered branches including
 * vp9_pick_inter_mode SVC branch (line 1801) and thread_encoding_proc
 * 
 * Target: Unlock critical blocked branches identified in coverage analysis:
 * 1. vp9_pick_inter_mode - 359 blocked branches (47% blocked, 4,374 blocker hits)
 *    - Specifically target SVC condition: `if (cpi->use_svc && svc->spatial_layer_id > 0)`
 * 2. thread_encoding_proc - 1,357 undiscovered branches (0% coverage)
 * 3. vp9_svc_reset_temporal_layers - 104 undiscovered branches
 * 4. ctrl_set_delta_q_uv - 381 undiscovered branches (VP9E_SET_DELTA_Q_UV control)
 * 5. vp9_pick_inter_mode_sub8x8 - 411 undiscovered branches
 * 
 * Goal: Exercise VP9 encoder with comprehensive SVC configurations including
 *       multiple spatial/temporal layers, advanced encoder controls, and
 *       specific layer configurations to trigger SVC-specific code paths
 * 
 * APIs to target as specified in guidance:
 * - vpx_codec_enc_init_ver (initialize encoder with SVC configuration)
 * - vpx_codec_enc_config_default (get default configuration)
 * - vpx_codec_enc_config_set (apply configuration changes - CRITICAL DIFFERENTIATOR)
 * - vpx_codec_encode (encode frames with SVC)
 * - vpx_codec_get_cx_data (retrieve encoded packets)
 * - vpx_codec_destroy (cleanup)
 * 
 * Specific Control IDs to Test as specified in guidance:
 * - VP9E_SET_SVC_PARAMETERS - Configure SVC layers (using vpx_svc_extra_cfg_t)
 * - VP9E_SET_SVC_SPATIAL_LAYER_SYNC - Spatial layer synchronization
 * - VP9E_SET_DELTA_Q_UV - Delta QP for UV components (targets ctrl_set_delta_q_uv)
 * - VP9E_SET_ACTIVE_MAP - Active map control (targets ctrl_set_active_map)
 * - VP9E_SET_TILE_COLUMNS/VP9E_SET_TILE_ROWS - Tile-based parallel processing
 * 
 * Semantic differentiation from harness_013:
 * 1. Uses vpx_codec_enc_config_set() to apply configuration changes after initialization
 * 2. Tests additional encoder controls: VP9E_SET_DELTA_Q_UV, VP9E_SET_ACTIVE_MAP, tile controls
 * 3. Focuses on deeper SVC complexity with more varied layer scaling configurations
 * 4. Explicitly targets the blocked branch in vp9_pick_inter_mode line 1801
 * 5. Tests both spatial_layer_id > 0 and use_svc conditions systematically
 * 
 * Invocation sequence:
 * 1. Get default VP9 encoder configuration with vpx_codec_enc_config_default
 * 2. Configure SVC parameters: 2-4 spatial layers, 2-3 temporal layers
 * 3. Initialize encoder with SVC configuration using vpx_codec_enc_init_ver
 * 4. Apply configuration changes with vpx_codec_enc_config_set (differentiator)
 * 5. Enable SVC with vpx_codec_control(VP9E_SET_SVC, 1)
 * 6. Set SVC parameters with vpx_codec_control(VP9E_SET_SVC_PARAMETERS)
 * 7. Configure additional controls: delta QP, active map, tile settings
 * 8. Create input frames with varying resolutions per spatial layer
 * 9. Encode multiple frames cycling through different spatial_layer_id values (> 0)
 * 10. Retrieve encoded data with vpx_codec_get_cx_data
 * 11. Cleanup with vpx_codec_destroy
 * 
 * Expected coverage gains:
 * - Reach vp9_pick_inter_mode SVC branch (line 1801) by ensuring spatial_layer_id > 0
 * - Cover thread_encoding_proc (1,357 branches currently at 0%)
 * - Exercise ctrl_set_delta_q_uv (381 branches currently at 0%)
 * - Test validate_config SVC validation paths
 * - Improve vp9/ module coverage from current 53.48%
 * - Target ~1,700+ undiscovered branches across critical encoding functions
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for SVC configuration, control parameters, and frame data
    const size_t MIN_SIZE = sizeof(uint8_t) * 1024 + sizeof(int) * 256 + sizeof(unsigned int) * 512;
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
    
    // Step 2: Configure SVC parameters with deeper complexity
    
    // Use 2-4 spatial layers and 2-3 temporal layers for meaningful SVC testing
    unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 4);
    unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 3);
    
    cfg.ss_number_layers = spatial_layers;
    cfg.ts_number_layers = temporal_layers;
    
    // Set base layer dimensions (small to medium sizes for efficiency)
    unsigned int base_width = fdp.ConsumeIntegralInRange<unsigned int>(128, 640);
    unsigned int base_height = fdp.ConsumeIntegralInRange<unsigned int>(96, 480);
    
    // Ensure dimensions are even for 4:2:0 subsampling
    base_width = (base_width + 1) & ~1;
    base_height = (base_height + 1) & ~1;
    
    cfg.g_w = base_width;
    cfg.g_h = base_height;
    
    // Configure layer-specific bitrate targets with more variation
    for (unsigned int i = 0; i < spatial_layers; ++i) {
        cfg.ss_target_bitrate[i] = fdp.ConsumeIntegralInRange<unsigned int>(200, 4000);
        cfg.ss_enable_auto_alt_ref[i] = fdp.ConsumeBool() ? 1 : 0;
    }
    
    // Temporal layer configuration with varied periodicity
    if (temporal_layers > 1) {
        cfg.ts_periodicity = fdp.ConsumeIntegralInRange<unsigned int>(4, 12);
        for (unsigned int i = 0; i < cfg.ts_periodicity && i < VPX_TS_MAX_PERIODICITY; ++i) {
            cfg.ts_layer_id[i] = fdp.ConsumeIntegral<unsigned int>() % temporal_layers;
        }
        
        // Set varied rate decimators and target bitrates for temporal layers
        for (unsigned int i = 0; i < temporal_layers && i < VPX_TS_MAX_LAYERS; ++i) {
            cfg.ts_rate_decimator[i] = 1 << (fdp.ConsumeIntegralInRange<unsigned int>(0, 2));
            cfg.ts_target_bitrate[i] = fdp.ConsumeIntegralInRange<unsigned int>(200, 4000);
        }
    }
    
    // Enable error resilience for SVC (important for multi-layer)
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Set frame rate with more variation
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 120);
    // Lag in frames (lookahead buffer) - important for SVC temporal layers
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(4, 32);
    
    // Step 3: Initialize encoder
    vpx_codec_flags_t flags = 0;
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 4: Apply configuration changes with vpx_codec_enc_config_set (CRITICAL DIFFERENTIATOR)
    // Modify some configuration parameters after initialization
    cfg.g_pass = static_cast<vpx_enc_pass>(fdp.ConsumeIntegralInRange<unsigned int>(0, 2)); // VPX_RC_ONE_PASS, VPX_RC_FIRST_PASS, VPX_RC_LAST_PASS
    
    // Apply the modified configuration
    if (vpx_codec_enc_config_set(&codec, &cfg) != VPX_CODEC_OK) {
        // Continue even if config_set fails - this tests error paths
    }
    
    // Step 5: Enable and configure SVC
    
    // Enable SVC
    if (vpx_codec_control(&codec, VP9E_SET_SVC, 1) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Configure SVC parameters using vpx_svc_extra_cfg_t
    vpx_svc_extra_cfg_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Set temporal layering mode (0-3 as defined in VP9)
    svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Configure layer-specific parameters with more variation
    for (unsigned int i = 0; i < spatial_layers; ++i) {
        // Min/Max quantizers for each layer with wider ranges
        svc_params.min_quantizers[i] = fdp.ConsumeIntegralInRange<int>(0, 30);
        svc_params.max_quantizers[i] = fdp.ConsumeIntegralInRange<int>(30, 63);
        
        // More varied scaling factors (not just powers of 2)
        svc_params.scaling_factor_num[i] = fdp.ConsumeIntegralInRange<int>(1, 32);
        svc_params.scaling_factor_den[i] = fdp.ConsumeIntegralInRange<int>(16, 64);
        
        // Speed setting per layer (0-9, where 0 is best quality, 9 is fastest)
        svc_params.speed_per_layer[i] = fdp.ConsumeIntegralInRange<int>(0, 9);
        
        // Loopfilter control per layer (0=off, 1=on)
        svc_params.loopfilter_ctrl[i] = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    // Apply SVC parameters
    if (vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 6: Configure additional controls specified in guidance
    
    // Set delta QP for UV components (targets ctrl_set_delta_q_uv - 381 undiscovered branches)
    if (fdp.remaining_bytes() > sizeof(int)) {
        int delta_q_uv = fdp.ConsumeIntegralInRange<int>(-10, 10);
        vpx_codec_control(&codec, VP9E_SET_DELTA_Q_UV, delta_q_uv);
    }
    
    // Set active map control (targets ctrl_set_active_map - 16 undiscovered branches)
    if (fdp.remaining_bytes() > sizeof(vpx_active_map_t)) {
        vpx_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        
        // Create a simple active map (regions that can change vs static)
        active_map.rows = (cfg.g_h + 15) / 16;
        active_map.cols = (cfg.g_w + 15) / 16;
        
        if (fdp.remaining_bytes() >= (size_t)(active_map.rows * active_map.cols)) {
            size_t map_size = active_map.rows * active_map.cols;
            std::vector<uint8_t> map_data = fdp.ConsumeBytes<uint8_t>(map_size);
            active_map.active_map = map_data.data();
            
            // Note: Using VP8E_SET_ACTIVEMAP since VP9E_SET_ACTIVE_MAP not found
            vpx_codec_control(&codec, VP8E_SET_ACTIVEMAP, &active_map);
        }
    }
    
    // Set tile columns and rows for parallel processing
    if (fdp.remaining_bytes() > 2 * sizeof(int)) {
        int tile_columns = fdp.ConsumeIntegralInRange<int>(0, 6);
        int tile_rows = fdp.ConsumeIntegralInRange<int>(0, 6);
        
        vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
        vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, tile_rows);
    }
    
    // Set spatial layer sync (as specified in guidance)
    if (fdp.remaining_bytes() > sizeof(vpx_svc_spatial_layer_sync_t)) {
        vpx_svc_spatial_layer_sync_t spatial_sync;
        memset(&spatial_sync, 0, sizeof(spatial_sync));
        
        spatial_sync.base_layer_intra_only = fdp.ConsumeBool() ? 1 : 0;
        for (unsigned int i = 0; i < spatial_layers; ++i) {
            spatial_sync.spatial_layer_sync[i] = fdp.ConsumeBool() ? 1 : 0;
        }
        
        vpx_codec_control(&codec, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &spatial_sync);
    }
    
    // Step 7: Create test frame(s) with varying resolutions per spatial layer
    
    // Create images for each spatial layer (different resolutions)
    std::vector<vpx_image_t*> layer_images;
    for (unsigned int layer = 0; layer < spatial_layers; ++layer) {
        // Calculate resolution for this layer based on scaling factors
        unsigned int layer_width = (cfg.g_w * svc_params.scaling_factor_num[layer]) / 
                                   svc_params.scaling_factor_den[layer];
        unsigned int layer_height = (cfg.g_h * svc_params.scaling_factor_num[layer]) / 
                                    svc_params.scaling_factor_den[layer];
        
        // Ensure even dimensions
        layer_width = (layer_width + 1) & ~1;
        layer_height = (layer_height + 1) & ~1;
        
        vpx_image_t* raw = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, layer_width, layer_height, 16);
        if (!raw) {
            // Cleanup allocated images
            for (auto img : layer_images) {
                vpx_img_free(img);
            }
            vpx_codec_destroy(&codec);
            return 0;
        }
        
        // Fill image with test data from fuzzer input
        size_t y_plane_size = raw->w * raw->h;
        size_t uv_plane_size = (raw->w / 2) * (raw->h / 2);
        size_t total_size_needed = y_plane_size + uv_plane_size * 2;
        
        if (fdp.remaining_bytes() < total_size_needed) {
            vpx_img_free(raw);
            // Cleanup allocated images
            for (auto img : layer_images) {
                vpx_img_free(img);
            }
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
        
        layer_images.push_back(raw);
    }
    
    // Step 8: Encode multiple frames cycling through different spatial_layer_id values
    
    int frame_count = fdp.ConsumeIntegralInRange<int>(4, 16); // More frames for better coverage
    vpx_codec_pts_t pts = 0;
    int64_t duration = 1;
    unsigned long deadline = VPX_DL_REALTIME;
    
    // Use base layer image for encoding (encoder will handle scaling)
    vpx_image_t* base_image = layer_images[0];
    
    for (int frame_idx = 0; frame_idx < frame_count; ++frame_idx) {
        // Set layer ID for this frame - CRITICAL: Ensure spatial_layer_id > 0 for some frames
        // to trigger the blocked branch in vp9_pick_inter_mode line 1801
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        
        // Cycle through spatial layers, ensuring some frames have spatial_layer_id > 0
        layer_id.spatial_layer_id = frame_idx % spatial_layers;
        layer_id.temporal_layer_id = frame_idx % temporal_layers;
        
        // Set temporal layer ID for each spatial layer
        for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
            layer_id.temporal_layer_id_per_spatial[sl] = (frame_idx + sl) % temporal_layers;
        }
        
        if (vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id) != VPX_CODEC_OK) {
            // Continue even if layer ID setting fails
        }
        
        // Encode the frame
        if (vpx_codec_encode(&codec, base_image, pts, duration, 0, deadline) != VPX_CODEC_OK) {
            break;
        }
        
        // Retrieve encoded data - exercise vpx_codec_get_cx_data
        const vpx_codec_cx_pkt_t* pkt = NULL;
        vpx_codec_iter_t iter = NULL;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame packet received - exercises SVC packetization logic
                // In SVC mode, we might get multiple packets per frame (one per layer)
                // This tests thread_encoding_proc and related functions
            }
        }
        
        pts += duration;
        
        // For some frames, test with NULL input (flush) to exercise different code paths
        if (frame_idx == frame_count / 2 || frame_idx == frame_count - 1) {
            if (vpx_codec_encode(&codec, NULL, pts, 0, 0, deadline) != VPX_CODEC_OK) {
                break;
            }
            
            // Retrieve flush data
            iter = NULL;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process flushed packets
            }
        }
        
        // Every few frames, change some encoder settings to test reconfiguration paths
        if (frame_idx % 4 == 0 && fdp.remaining_bytes() > 100) {
            // Change delta QP value
            int new_delta_q_uv = fdp.ConsumeIntegralInRange<int>(-15, 15);
            vpx_codec_control(&codec, VP9E_SET_DELTA_Q_UV, new_delta_q_uv);
            
            // Change temporal layering mode
            int new_temporal_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
            svc_params.temporal_layering_mode = new_temporal_mode;
            vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params);
        }
    }
    
    // Step 9: Test frame dropping configuration for SVC
    if (fdp.remaining_bytes() > sizeof(vpx_svc_frame_drop_t)) {
        vpx_svc_frame_drop_t frame_drop;
        memset(&frame_drop, 0, sizeof(frame_drop));
        
        int drop_mode_val = fdp.ConsumeIntegralInRange<int>(0, 3);
        frame_drop.framedrop_mode = static_cast<SVC_LAYER_DROP_MODE>(drop_mode_val);
        frame_drop.max_consec_drop = fdp.ConsumeIntegralInRange<int>(0, 10);
        
        for (unsigned int i = 0; i < spatial_layers; ++i) {
            frame_drop.framedrop_thresh[i] = fdp.ConsumeIntegralInRange<int>(0, 100);
        }
        
        vpx_codec_control(&codec, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
    }
    
    // Step 10: Cleanup
    for (auto img : layer_images) {
        vpx_img_free(img);
    }
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_014
