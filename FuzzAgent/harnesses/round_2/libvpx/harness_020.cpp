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

namespace harness_020 {
/*
 * Fuzzing harness for libvpx targeting VP9 SVC (Scalable Video Coding) multi-layer encoding
 * to cover blocked branches in vp9_pick_inter_mode (375/764 branches blocked, 49% blocked)
 * and related SVC code paths with spatial_layer_id > 0 conditions.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_020.cpp based on coverage guidance targeting VP9 SVC 
 *       multi-layer encoding features to address the coverage gap in vp9_pick_inter_mode
 *       and related SVC code paths where conditions cpi->use_svc && svc->spatial_layer_id > 0 
 *       have 0 hits.
 * 
 * APIs to target as specified in guidance:
 * - vpx_codec_enc_init_ver (Required: SVC initialization)
 * - vpx_codec_enc_config_default (Required: Default config)
 * - vpx_codec_enc_config_set (Required: Configure SVC parameters)
 * - vpx_codec_control_ (Critical: Set SVC-specific controls)
 * - vpx_codec_encode (Target: Encode with SVC)
 * - vpx_codec_get_cx_data (Target: Get encoded data)
 * - vpx_codec_destroy (Required: Cleanup)
 * - vpx_img_alloc (Helper: Image allocation)
 * - vpx_img_free (Helper: Image cleanup)
 * 
 * Specific blocked branches targeted:
 * - vp9_pick_inter_mode: 375 blocked branches (49% coverage gap)
 * - Conditions with cpi->use_svc && svc->spatial_layer_id > 0: 0 hits
 * - validate_config: 302/720 branches blocked
 * - vp9_rd_pick_inter_mode_sb: 303/462 branches blocked
 * 
 * Semantic differentiation from previous harnesses:
 * 1. Specifically focuses on SVC multi-layer encoding with spatial_layer_id > 0
 * 2. Uses vpx_codec_control_ with advanced SVC controls: VP9E_SET_SVC, 
 *    VP9E_SET_SVC_PARAMETERS, VP9E_SET_SVC_LAYER_ID, VP9E_SET_SVC_REF_FRAME_CONFIG
 * 3. Tests multiple spatial layers (2-3) with spatial_layer_id > 0 conditions
 * 4. Tests temporal layering with different layer configurations
 * 5. Tests SVC reference frame configurations to trigger motion estimation paths
 * 6. Focuses on the exact coverage gap identified: spatial_layer_id > 0 conditions
 * 
 * Invocation sequence from guidance:
 * 1. Call vpx_codec_enc_config_default to get default VP9 encoder configuration.
 * 2. Configure SVC parameters using vpx_codec_control_ with control IDs for spatial layers, 
 *    temporal layers, and scalability.
 * 3. Call vpx_codec_enc_init_ver with SVC configuration for multi-layer encoding.
 * 4. Allocate source frames using vpx_img_alloc with appropriate formats.
 * 5. Call vpx_codec_encode multiple times with different spatial/temporal layer configurations.
 * 6. Call vpx_codec_get_cx_data to retrieve encoded layer data.
 * 7. Clean up with vpx_codec_destroy and vpx_img_free.
 * 
 * Key configurations to test:
 * - Multiple spatial layers (2-3 layers) with spatial_layer_id > 0
 * - Temporal layering (2-3 layers)
 * - SVC reference frame configurations
 * - Different scaling factors for spatial layers
 * - Layer-specific quantization parameters
 * - Frame dropping configurations for SVC
 * - Inter-layer prediction modes
 * 
 * Reasoning from guidance: These configurations target 375+ blocked branches in 
 * vp9_pick_inter_mode and additional blocked branches in motion estimation 
 * functions. The SVC conditions cpi->use_svc && svc->spatial_layer_id > 0 are 
 * currently untested (0 hits). This represents the largest remaining coverage 
 * gap in the VP9 encoder module and addresses complex conditional logic that 
 * current fuzzers have not exercised.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for SVC configuration, layer parameters,
    // and image data for multiple frames
    const size_t MIN_SIZE = sizeof(uint8_t) * 4096 + sizeof(int) * 256 + sizeof(unsigned int) * 512;
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
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920) & ~1; // Ensure even width
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080) & ~1; // Ensure even height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Configure SVC parameters - specifically test with multiple spatial layers
    // to trigger spatial_layer_id > 0 conditions
    unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 3); // Must be >1 for spatial_layer_id > 0
    unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    
    // Set SVC configuration
    cfg.ss_number_layers = spatial_layers;
    cfg.ts_number_layers = temporal_layers;
    cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Configure rate control for SVC
    cfg.rc_end_usage = fdp.ConsumeBool() ? VPX_VBR : VPX_CBR;
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 50);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
    
    // Set layer target bitrates for SVC
    for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
        for (unsigned int tl = 0; tl < temporal_layers; ++tl) {
            unsigned int layer = sl * temporal_layers + tl;
            if (layer < VPX_MAX_LAYERS) {
                // Distribute bitrate across layers
                cfg.layer_target_bitrate[layer] = cfg.rc_target_bitrate * (sl + 1) / (spatial_layers * (tl + 1));
            }
        }
    }
    
    // Set keyframe interval
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 30);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 100);
    
    // Set lag in frames (for lookahead)
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    
    // Initialize encoder with SVC configuration
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 3: Set SVC controls
    
    // Enable SVC mode (critical for cpi->use_svc condition)
    if (vpx_codec_control(&codec, VP9E_SET_SVC, 1) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Configure SVC parameters
    vpx_svc_extra_cfg_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Set scaling factors for spatial layers
    for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
        // Different scaling factors to trigger different code paths
        svc_params.scaling_factor_num[sl] = fdp.ConsumeIntegralInRange<int>(1, 16);
        svc_params.scaling_factor_den[sl] = fdp.ConsumeIntegralInRange<int>(1, 16);
        // Layer-specific quantization
        svc_params.max_quantizers[sl] = fdp.ConsumeIntegralInRange<int>(cfg.rc_max_quantizer, 63);
        svc_params.min_quantizers[sl] = fdp.ConsumeIntegralInRange<int>(0, cfg.rc_min_quantizer);
        // Speed per layer (affects motion estimation)
        svc_params.speed_per_layer[sl] = fdp.ConsumeIntegralInRange<int>(0, 9);
        // Set loopfilter control per layer
        svc_params.loopfilter_ctrl[sl] = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    // Set temporal layering mode
    svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    // Apply SVC parameters
    if (vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set inter-layer prediction mode
    unsigned int inter_layer_pred = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
    if (vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, inter_layer_pred) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set speed setting (affects motion estimation and vp9_pick_inter_mode)
    unsigned int speed = fdp.ConsumeIntegralInRange<unsigned int>(0, 9);
    if (vpx_codec_control(&codec, VP8E_SET_CPUUSED, speed) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set AQ mode (affects segmentation)
    unsigned int aq_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 4);
    if (vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set noise sensitivity
    unsigned int noise_sensitivity = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    if (vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set content type
    unsigned int tune_content = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    if (vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, tune_content) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 4: Allocate input image
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with fuzzer data
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = (cfg.g_w / 2) * (cfg.g_h / 2);
    
    if (fdp.remaining_bytes() < y_size + 2 * uv_size) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Consume image data
    std::vector<uint8_t> y_plane = fdp.ConsumeBytes<uint8_t>(y_size);
    std::vector<uint8_t> u_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    std::vector<uint8_t> v_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    
    if (y_plane.size() != y_size || u_plane.size() != uv_size || v_plane.size() != uv_size) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    memcpy(img->planes[VPX_PLANE_Y], y_plane.data(), y_size);
    memcpy(img->planes[VPX_PLANE_U], u_plane.data(), uv_size);
    memcpy(img->planes[VPX_PLANE_V], v_plane.data(), uv_size);
    
    img->stride[VPX_PLANE_Y] = cfg.g_w;
    img->stride[VPX_PLANE_U] = cfg.g_w / 2;
    img->stride[VPX_PLANE_V] = cfg.g_w / 2;
    
    // Step 5: Encode multiple frames with different SVC layer configurations
    // This is critical for testing spatial_layer_id > 0 conditions
    
    unsigned int num_frames_to_encode = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
    
    for (unsigned int frame_idx = 0; frame_idx < num_frames_to_encode; ++frame_idx) {
        if (fdp.remaining_bytes() < sizeof(uint8_t) * 1024) {
            break; // Not enough data for more frames
        }
        
        // Set layer ID for this frame - CRITICAL: Test spatial_layer_id > 0
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        
        // Vary spatial layer ID to test spatial_layer_id > 0 conditions
        // Start with layer 0 for first frame (keyframe), then use >0 for subsequent frames
        if (frame_idx == 0) {
            layer_id.spatial_layer_id = 0; // Keyframe at base layer
            layer_id.temporal_layer_id = 0;
        } else {
            // Test spatial_layer_id > 0 conditions
            layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<unsigned int>(1, spatial_layers - 1);
            layer_id.temporal_layer_id = fdp.ConsumeIntegralInRange<unsigned int>(0, temporal_layers - 1);
        }
        
        // Set temporal layer ID per spatial layer
        for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
            layer_id.temporal_layer_id_per_spatial[sl] = 
                fdp.ConsumeIntegralInRange<unsigned int>(0, temporal_layers - 1);
        }
        
        // Apply layer ID
        if (vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id) != VPX_CODEC_OK) {
            // Continue with next frame even if this fails
            continue;
        }
        
        // Set reference frame configuration for SVC
        if (spatial_layers > 1 && frame_idx > 0) {
            vpx_svc_ref_frame_config_t ref_config;
            memset(&ref_config, 0, sizeof(ref_config));
            
            // Configure reference frames for different spatial layers
            for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
                ref_config.lst_fb_idx[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
                ref_config.gld_fb_idx[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
                ref_config.alt_fb_idx[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
                // Set update flags (deprecated but still in struct)
                ref_config.update_buffer_slot[sl] = fdp.ConsumeIntegralInRange<int>(0, 7);
                ref_config.update_last[sl] = fdp.ConsumeBool() ? 1 : 0;
                ref_config.update_golden[sl] = fdp.ConsumeBool() ? 1 : 0;
                ref_config.update_alt_ref[sl] = fdp.ConsumeBool() ? 1 : 0;
                // Set reference flags
                ref_config.reference_last[sl] = fdp.ConsumeBool() ? 1 : 0;
                ref_config.reference_golden[sl] = fdp.ConsumeBool() ? 1 : 0;
                ref_config.reference_alt_ref[sl] = fdp.ConsumeBool() ? 1 : 0;
            }
            
            // Set duration
            ref_config.duration[0] = 1000000 / cfg.g_timebase.den;
            ref_config.duration[1] = 1000000 / cfg.g_timebase.den;
            
            // Apply reference frame configuration
            vpx_codec_control(&codec, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
        }
        
        // Set frame flags
        vpx_enc_frame_flags_t flags = 0;
        if (frame_idx == 0) {
            flags |= VPX_EFLAG_FORCE_KF; // Force keyframe for first frame
        }
        
        // Encode the frame
        vpx_codec_pts_t pts = frame_idx * 1000000 / cfg.g_timebase.den;
        int64_t duration = 1000000 / cfg.g_timebase.den;
        int deadline = VPX_DL_REALTIME;
        
        if (vpx_codec_encode(&codec, img, pts, duration, flags, deadline) != VPX_CODEC_OK) {
            // Continue with next frame even if encode fails
            continue;
        }
        
        // Step 6: Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet if needed
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame packet received
                // Check spatial layer encoding status
                if (spatial_layers > 1) {
                    for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
                        if (pkt->data.frame.spatial_layer_encoded[sl]) {
                            // This spatial layer was encoded
                            // This helps test spatial_layer_id > 0 conditions
                        }
                    }
                }
            }
        }
        
        // Update image content slightly for next frame
        if (fdp.remaining_bytes() > y_size / 4) {
            std::vector<uint8_t> noise = fdp.ConsumeBytes<uint8_t>(y_size / 4);
            for (size_t i = 0; i < noise.size() && i < y_size; ++i) {
                img->planes[VPX_PLANE_Y][i] ^= noise[i];
            }
        }
    }
    
    // Step 7: Flush encoder
    if (vpx_codec_encode(&codec, NULL, 0, 0, 0, 0) == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process any remaining packets
        }
    }
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_020
