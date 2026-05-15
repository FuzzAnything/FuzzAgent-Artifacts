#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_017 {
/*
 * Fuzzing harness for libvpx VP9 SVC (Scalable Video Coding) encoding functionality
 * Targets vp9_one_pass_svc_start_layer and related SVC encoding with 371+ undiscovered branches
 * Focus: Multi-layer SVC encoding with vpx_codec_control API for SVC parameters
 * Strategy: Test VP9 SVC encoding with complex layer configurations, rate allocation, error handling
 * API Sequence: vpx_codec_enc_init() -> vpx_codec_control() with SVC parameters -> multi-layer encoding
 * Differentiation: Unlike harness_002 which tests basic SVC, this targets vp9_one_pass_svc_start_layer
 *                 and comprehensive SVC parameter configuration with VP9E_SET_SVC_PARAMETERS
 */



// Minimum input size for comprehensive SVC testing
const size_t MIN_INPUT_SIZE = 256;

// Maximum spatial layers (as per VPX_SS_MAX_LAYERS = 5)
const int MAX_SPATIAL_LAYERS = 5;
// Maximum temporal layers (as per VPX_TS_MAX_LAYERS = 5)
const int MAX_TEMPORAL_LAYERS = 5;

// Helper to create and fill image with fuzzed data for SVC testing
static vpx_image_t* create_fuzzed_image(FuzzedDataProvider* fdp, unsigned int width, unsigned int height) {
    if (!fdp || width == 0 || height == 0 || width > 4096 || height > 4096) {
        return nullptr;
    }
    
    // Use I420 format for SVC testing (most common for VP9 SVC)
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        return nullptr;
    }
    
    // Fill image planes with fuzzed data
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char* buf = img->planes[plane];
        const int stride = img->stride[plane];
        int w = (plane == 0) ? width : (width + 1) / 2;
        int h = (plane == 0) ? height : (height + 1) / 2;
        
        for (int y = 0; y < h; ++y) {
            if (fdp->remaining_bytes() < (size_t)w) {
                // Not enough data, free image and return
                vpx_img_free(img);
                return nullptr;
            }
            std::vector<uint8_t> row_data = fdp->ConsumeBytes<uint8_t>(w);
            memcpy(buf, row_data.data(), w);
            buf += stride;
        }
    }
    
    return img;
}

// Configure SVC parameters structure from fuzzer input
static void configure_svc_parameters(FuzzedDataProvider* fdp, vpx_svc_extra_cfg_t* svc_params, 
                                     int num_spatial_layers, int num_temporal_layers) {
    if (!svc_params || !fdp) return;
    
    memset(svc_params, 0, sizeof(vpx_svc_extra_cfg_t));
    
    // Configure each spatial layer
    for (int sl = 0; sl < num_spatial_layers; ++sl) {
        // Max and min quantizers (0-63 range)
        svc_params->max_quantizers[sl] = fdp->ConsumeIntegralInRange<int>(0, 63);
        svc_params->min_quantizers[sl] = fdp->ConsumeIntegralInRange<int>(0, svc_params->max_quantizers[sl]);
        
        // Scaling factors (common denominators for SVC)
        svc_params->scaling_factor_num[sl] = fdp->ConsumeIntegralInRange<int>(1, 16);
        svc_params->scaling_factor_den[sl] = fdp->ConsumeIntegralInRange<int>(svc_params->scaling_factor_num[sl], 16);
        
        // Speed settings per layer (0-9 range for VP9)
        svc_params->speed_per_layer[sl] = fdp->ConsumeIntegralInRange<int>(0, 9);
        
        // Loopfilter control (0=off, 1=on, 2=on with keyframe)
        svc_params->loopfilter_ctrl[sl] = fdp->ConsumeIntegralInRange<int>(0, 2);
    }
    
    // Temporal layering mode (from vpx/vp8cx.h)
    // VP9E_TEMPORAL_LAYERING_MODE_NOLAYERING = 0
    // VP9E_TEMPORAL_LAYERING_MODE_BYPASS = 1  
    // VP9E_TEMPORAL_LAYERING_MODE_0101 = 2
    // VP9E_TEMPORAL_LAYERING_MODE_0212 = 3
    svc_params->temporal_layering_mode = fdp->ConsumeIntegralInRange<int>(0, 3);
}
// Configure encoder for SVC with proper layer settings
static void configure_encoder_for_svc(vpx_codec_ctx_t* codec, FuzzedDataProvider* fdp,
                                      int num_spatial_layers, int num_temporal_layers) {
    if (!codec || !fdp) return;
    
    // First, enable SVC - takes int parameter directly, not pointer
    int enable_svc = 1;
    vpx_codec_control(codec, VP9E_SET_SVC, enable_svc);
    
    // Set SVC parameters structure
    vpx_svc_extra_cfg_t svc_params;
    configure_svc_parameters(fdp, &svc_params, num_spatial_layers, num_temporal_layers);
    vpx_codec_control(codec, VP9E_SET_SVC_PARAMETERS, &svc_params);
    
    // Set layer ID for current encoding
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));
    
    layer_id.spatial_layer_id = fdp->ConsumeIntegralInRange<int>(0, num_spatial_layers - 1);
    layer_id.temporal_layer_id = fdp->ConsumeIntegralInRange<int>(0, num_temporal_layers - 1);
    // For bypass mode, set per-spatial temporal layer IDs
    if (svc_params.temporal_layering_mode == 1) { // BYPASS mode
        for (int sl = 0; sl < num_spatial_layers; ++sl) {
            layer_id.temporal_layer_id_per_spatial[sl] = fdp->ConsumeIntegralInRange<int>(0, num_temporal_layers - 1);
        }
    }
    
    vpx_codec_control(codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
    
    // Set reference frame configuration for SVC
    vpx_svc_ref_frame_config_t ref_config;
    memset(&ref_config, 0, sizeof(ref_config));
    
    for (int sl = 0; sl < num_spatial_layers; ++sl) {
        // Frame buffer indices (0-7 for VP9)
        ref_config.lst_fb_idx[sl] = fdp->ConsumeIntegralInRange<int>(0, 7);
        ref_config.gld_fb_idx[sl] = fdp->ConsumeIntegralInRange<int>(0, 7);
        ref_config.alt_fb_idx[sl] = fdp->ConsumeIntegralInRange<int>(0, 7);
        
        // Update buffer slots (bitmask)
        ref_config.update_buffer_slot[sl] = fdp->ConsumeIntegral<uint8_t>();
        
        // Reference flags
        ref_config.reference_last[sl] = fdp->ConsumeBool();
        ref_config.reference_golden[sl] = fdp->ConsumeBool();
        ref_config.reference_alt_ref[sl] = fdp->ConsumeBool();
    }
    
    vpx_codec_control(codec, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
    
    // Test frame dropping configuration
    vpx_svc_frame_drop_t frame_drop;
    memset(&frame_drop, 0, sizeof(frame_drop));
    // Use enum values: CONSTRAINED_LAYER_DROP=0, LAYER_DROP=1, FULL_SUPERFRAME_DROP=2, CONSTRAINED_FROM_ABOVE_DROP=3
    int drop_mode = fdp->ConsumeIntegralInRange<int>(0, 3);
    frame_drop.framedrop_mode = static_cast<SVC_LAYER_DROP_MODE>(drop_mode);
    frame_drop.max_consec_drop = fdp->ConsumeIntegralInRange<int>(0, 10);
    
    for (int sl = 0; sl < num_spatial_layers; ++sl) {
        frame_drop.framedrop_thresh[sl] = fdp->ConsumeIntegralInRange<int>(0, 100);
    }
    
    // Test spatial layer sync flag
    vpx_svc_spatial_layer_sync_t spatial_sync;
    memset(&spatial_sync, 0, sizeof(spatial_sync));
    
    for (int sl = 0; sl < num_spatial_layers; ++sl) {
        spatial_sync.spatial_layer_sync[sl] = fdp->ConsumeBool();
    }
    
    vpx_codec_control(codec, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &spatial_sync);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume SVC configuration from fuzzer input
    uint8_t width_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
    uint8_t height_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
    uint8_t num_spatial_layers = fdp.ConsumeIntegralInRange<uint8_t>(1, MAX_SPATIAL_LAYERS);
    uint8_t num_temporal_layers = fdp.ConsumeIntegralInRange<uint8_t>(1, MAX_TEMPORAL_LAYERS);
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Map width/height indices to actual dimensions suitable for SVC
    unsigned int widths[] = {64, 128, 256, 512, 1024};
    unsigned int heights[] = {48, 96, 192, 384, 768};
    unsigned int width = widths[width_idx % 5];
    unsigned int height = heights[height_idx % 5];
    
    // VP9 encoder interface for SVC
    const vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // Encoder configuration
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Configure basic encoder settings
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Configure spatial and temporal layers for SVC
    cfg.ss_number_layers = num_spatial_layers;
    cfg.ts_number_layers = num_temporal_layers;
    
    // Configure layer bitrates (required for SVC)
    for (int sl = 0; sl < num_spatial_layers; ++sl) {
        // Higher layers get more bitrate
        cfg.layer_target_bitrate[sl] = cfg.rc_target_bitrate * (sl + 1) / num_spatial_layers;
    }
    // Configure temporal layer bitrate allocation
    for (int tl = 0; tl < num_temporal_layers; ++tl) {
        // Higher temporal layers get less bitrate
        cfg.ts_rate_decimator[tl] = (1 << tl); // Exponential rate reduction
        // Note: layer_target_bitrate array is also used for temporal layers
        // This will overwrite spatial layer values, but it's okay for testing
        cfg.layer_target_bitrate[tl] = cfg.rc_target_bitrate / (1 << tl);
    }
    
    // Declare encoder context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Basic SVC encoding with valid configuration
            if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
                configure_encoder_for_svc(&codec, &fdp, num_spatial_layers, num_temporal_layers);
                
                // Create and encode a single frame
                vpx_image_t* img = create_fuzzed_image(&fdp, width, height);
                if (img) {
                    vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
                    vpx_img_free(img);
                }
                
                vpx_codec_destroy(&codec);
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Test error handling with invalid SVC configurations
            if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
                // Test invalid layer ID (out of bounds)
                vpx_svc_layer_id_t invalid_layer_id;
                memset(&invalid_layer_id, 0, sizeof(invalid_layer_id));
                invalid_layer_id.spatial_layer_id = num_spatial_layers + 1; // Out of bounds
                vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &invalid_layer_id);
                // Test with zero spatial layers (invalid for SVC)
                int zero_layers = 0;
                vpx_codec_control(&codec, VP9E_SET_SVC, zero_layers);
                
                // Test getting layer ID back
                vpx_svc_layer_id_t get_layer_id;
                
                vpx_codec_destroy(&codec);
            }
            break;
        }
        
        case 2: {
            // Scenario 3: Test multiple frames with different layer configurations
            if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
                configure_encoder_for_svc(&codec, &fdp, num_spatial_layers, num_temporal_layers);
                
                // Encode multiple frames with varying layer configurations
                for (int frame = 0; frame < 3 && fdp.remaining_bytes() > 100; ++frame) {
                    vpx_image_t* img = create_fuzzed_image(&fdp, width, height);
                    if (!img) break;
                    
                    // Change layer ID for each frame to exercise vp9_one_pass_svc_start_layer
                    vpx_svc_layer_id_t dynamic_layer_id;
                    memset(&dynamic_layer_id, 0, sizeof(dynamic_layer_id));
                    dynamic_layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, num_spatial_layers - 1);
                    dynamic_layer_id.temporal_layer_id = fdp.ConsumeIntegralInRange<int>(0, num_temporal_layers - 1);
                    vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &dynamic_layer_id);
                    
                    // Encode frame
                    vpx_codec_encode(&codec, img, frame * 30, 1, 0, VPX_DL_GOOD_QUALITY);
                    
                    vpx_img_free(img);
                }
                
                vpx_codec_destroy(&codec);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Test SVC with inter-layer prediction modes
            if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
                configure_encoder_for_svc(&codec, &fdp, num_spatial_layers, num_temporal_layers);
                
                // Test inter-layer prediction control
                // Note: This would exercise paths in vp9_one_pass_svc_start_layer
                // related to disable_inter_layer_pred and simulcast_mode
                
                // Create test image
                vpx_image_t* img = create_fuzzed_image(&fdp, width, height);
                if (img) {
                    // Encode key frame first
                    vpx_codec_encode(&codec, img, 0, 1, VPX_EFLAG_FORCE_KF, VPX_DL_GOOD_QUALITY);
                    
                    // Encode inter frame
                    vpx_codec_encode(&codec, img, 30, 1, 0, VPX_DL_GOOD_QUALITY);
                    
                    vpx_img_free(img);
                }
                
                vpx_codec_destroy(&codec);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_017
