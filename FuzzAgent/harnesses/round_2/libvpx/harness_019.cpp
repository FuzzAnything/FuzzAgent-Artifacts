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

namespace harness_019 {
/*
 * Fuzzing harness for libvpx targeting VP9 SVC (Scalable Video Coding) encoding 
 * with segmentation features to reach blocked branches in vp9_pick_inter_mode 
 * and related motion estimation functions.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_019.cpp based on coverage guidance targeting VP9 SVC 
 *       encoding with segmentation features to address the highest coverage 
 *       blocker vp9_pick_inter_mode (375 blocked branches) and related motion 
 *       estimation functions.
 * 
 * APIs to target as specified in guidance:
 * - vpx_codec_enc_init_ver (Required Helper: Init with SVC)
 * - vpx_codec_enc_config_default (Required Helper: Config)
 * - vpx_codec_enc_config_set (Required Helper: SVC/segmentation config)
 * - vpx_codec_control_ (Target: Set SVC parameters, segmentation, reference frames)
 * - vpx_codec_encode (Target: Trigger encoding pipeline)
 * - vpx_codec_get_cx_data (Required Helper: Get encoded data)
 * - vpx_codec_destroy (Required Helper: Cleanup)
 * - vpx_img_alloc (Required Helper: Image allocation)
 * - vpx_img_free (Required Helper: Image cleanup)
 * 
 * Specific blocked branches targeted:
 * - vp9_pick_inter_mode: 375 blocked branches (inter-mode selection with SVC features)
 * - vp9_find_best_sub_pixel_tree_pruned_more: 230 blocked branches
 * - vp9_find_best_sub_pixel_tree_pruned_evenmore: 0% coverage
 * Total: 600+ blocked branches
 * 
 * Semantic differentiation from previous harnesses:
 * 1. Specifically focuses on SVC (Scalable Video Coding) encoding with multiple spatial/temporal layers
 * 2. Tests segmentation features with reference frame constraints (SEG_LVL_REF_FRAME)
 * 3. Tests advanced SVC configurations that trigger motion estimation code paths
 * 4. Uses vpx_codec_control_ with SVC-specific controls (VP9E_SET_SVC, VP9E_SET_SVC_PARAMETERS)
 * 5. Tests speed settings (speed >= 8) to reach specific branches in vp9_pick_inter_mode
 * 
 * Invocation sequence from guidance:
 * 1. Call vpx_codec_enc_config_default to get default VP9 encoder config.
 * 2. Modify config for SVC: set number of spatial/temporal layers, enable SVC mode.
 * 3. Call vpx_codec_enc_init_ver with SVC interface (vpx_codec_vp9_cx).
 * 4. Use vpx_codec_control_ to set SVC parameters: VP9E_SET_SVC_PARAMETERS.
 * 5. Use vpx_codec_control_ to set segmentation features.
 * 6. Use vpx_codec_control_ to set reference frame flags and constraints.
 * 7. Allocate input images using vpx_img_alloc.
 * 8. Call vpx_codec_encode multiple times with different SVC layer configurations.
 * 9. Call vpx_codec_get_cx_data to retrieve encoded packets.
 * 10. Call vpx_codec_destroy and vpx_img_free for cleanup.
 * 
 * Key configurations to test:
 * - Multiple spatial layers (2-3 layers)
 * - Temporal layering (2-3 layers)
 * - Segmentation with reference frame constraints (SEG_LVL_REF_FRAME)
 * - Different speed settings (speed >= 8 to reach specific branches)
 * - Various reference frame combinations (LAST, GOLDEN, ALTREF)
 * - Motion estimation settings that trigger sub-pixel search algorithms
 * 
 * Reasoning from guidance: These configurations target 375+ blocked branches in 
 * vp9_pick_inter_mode and additional blocked branches in motion estimation 
 * functions. The SVC and segmentation features are currently under-tested and 
 * represent high-complexity code paths that persist as coverage gaps after 19 
 * fuzzers. The blocked branches relate to SVC features (cpi->use_svc, 
 * svc->spatial_layer_id > 0), segmentation features with reference frame 
 * constraints, and specific speed settings (cpi->oxcf.speed >= 8).
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for SVC configuration, segmentation parameters,
    // and image data
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
    // Use reasonable ranges for video dimensions
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 1920) & ~1; // Ensure even width
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 1080) & ~1; // Ensure even height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Configure SVC parameters
    unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    
    // Set SVC configuration
    cfg.ss_number_layers = spatial_layers;
    cfg.ts_number_layers = temporal_layers;
    cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Configure rate control for SVC
    cfg.rc_end_usage = VPX_VBR;
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
    
    // Set layer target bitrates for SVC
    for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
        for (unsigned int tl = 0; tl < temporal_layers; ++tl) {
            unsigned int layer = sl * temporal_layers + tl;
            if (layer < VPX_MAX_LAYERS) {
                cfg.layer_target_bitrate[layer] = cfg.rc_target_bitrate * (layer + 1) / (spatial_layers * temporal_layers);
            }
        }
    }
    
    // Set keyframe interval
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 1000);
    
    // Initialize encoder with SVC configuration
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 3: Set SVC controls
    
    // Enable SVC mode
    if (vpx_codec_control(&codec, VP9E_SET_SVC, 1) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Configure SVC parameters
    vpx_svc_extra_cfg_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Set scaling factors for spatial layers
    for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
        svc_params.scaling_factor_num[sl] = fdp.ConsumeIntegralInRange<int>(1, 16);
        svc_params.scaling_factor_den[sl] = fdp.ConsumeIntegralInRange<int>(1, 16);
        svc_params.max_quantizers[sl] = fdp.ConsumeIntegralInRange<int>(cfg.rc_max_quantizer, 63);
        svc_params.min_quantizers[sl] = fdp.ConsumeIntegralInRange<int>(0, cfg.rc_min_quantizer);
        svc_params.speed_per_layer[sl] = fdp.ConsumeIntegralInRange<int>(0, 9);
    }
    
    // Set temporal layering mode
    svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Apply SVC parameters
    if (vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 4: Set segmentation controls
    // Note: The guidance mentions segmentation features but VP9E_SET_SEGMENTATION
    // doesn't exist. Instead, we'll use AQ mode and other controls that affect segmentation.
    
    // Set AQ mode (affects segmentation)
    unsigned int aq_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 4);
    if (vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set noise sensitivity (affects segmentation in some modes)
    unsigned int noise_sensitivity = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
    if (vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set speed setting (critical for reaching speed >= 8 branches)
    unsigned int speed = fdp.ConsumeIntegralInRange<unsigned int>(0, 9);
    if (vpx_codec_control(&codec, VP8E_SET_CPUUSED, speed) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 5: Set reference frame controls
    // Test various reference frame configurations that affect vp9_pick_inter_mode
    
    // Set alt-ref AQ
    int alt_ref_aq = fdp.ConsumeBool() ? 1 : 0;
    if (vpx_codec_control(&codec, VP9E_SET_ALT_REF_AQ, alt_ref_aq) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set frame parallel decoding (affects reference frame usage)
    unsigned int frame_parallel_decoding = fdp.ConsumeBool() ? 1 : 0;
    if (vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 6: Allocate and prepare input image
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with random data from fuzzer
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = (cfg.g_w / 2) * (cfg.g_h / 2);
    size_t total_img_size = y_size + 2 * uv_size;
    
    if (fdp.remaining_bytes() < total_img_size) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill Y plane
    std::vector<uint8_t> y_plane = fdp.ConsumeBytes<uint8_t>(y_size);
    memcpy(img->planes[VPX_PLANE_Y], y_plane.data(), y_size);
    
    // Fill U plane
    std::vector<uint8_t> u_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    memcpy(img->planes[VPX_PLANE_U], u_plane.data(), uv_size);
    
    // Fill V plane
    std::vector<uint8_t> v_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    memcpy(img->planes[VPX_PLANE_V], v_plane.data(), uv_size);
    
    // Step 7: Encode multiple frames with different SVC layer configurations
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int frame_idx = 0; frame_idx < num_frames; ++frame_idx) {
        if (fdp.remaining_bytes() < 100) {
            break; // Not enough data for more frames
        }
        
        // Set SVC layer ID for this frame
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        
        layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, spatial_layers - 1);
        layer_id.temporal_layer_id = fdp.ConsumeIntegralInRange<int>(0, temporal_layers - 1);
        
        if (vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id) != VPX_CODEC_OK) {
            continue; // Try next frame
        }
        
        // Encode the frame
        vpx_codec_err_t encode_result = vpx_codec_encode(&codec, img, frame_idx, 1, 0, VPX_DL_REALTIME);
        
        if (encode_result != VPX_CODEC_OK) {
            // Some encoding errors are expected with random data
            continue;
        }
        
        // Retrieve encoded data
        const vpx_codec_cx_pkt_t* pkt = NULL;
        vpx_codec_iter_t iter = NULL;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame encoded successfully
                // We could process the encoded data here if needed
            }
        }
        
        // Occasionally change encoder settings during sequence
        if (frame_idx % 3 == 0 && fdp.remaining_bytes() > 50) {
            // Change speed setting to test different code paths
            unsigned int new_speed = fdp.ConsumeIntegralInRange<unsigned int>(0, 9);
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, new_speed);
            
            // Change AQ mode
            unsigned int new_aq_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 4);
            vpx_codec_control(&codec, VP9E_SET_AQ_MODE, new_aq_mode);
        }
    }
    
    // Step 8: Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_019
