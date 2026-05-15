#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <memory>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_013 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom AV1 encoder with focus on deep FAST-9
 * corner detection paths (aom_fast9_detect) via global motion estimation.
 * 
 * Primary Target: aom_fast9_detect (1898 blocked branches - 96% blocked)
 * 
 * APIs targeted as per coverage guidance:
 * - aom_codec_enc_init_ver (Required Setup)
 * - aom_codec_enc_config_default (Configuration)
 * - aom_codec_enc_config_set (Enable motion estimation features)
 * - aom_codec_encode (Primary entry point triggering global motion estimation)
 * - aom_codec_destroy (Cleanup)
 * 
 * Semantic differentiation from previous 13 harnesses:
 * - harness_012: Global motion estimation with algorithmic corner patterns
 * - harness_013: Direct fuzzer-driven pixel generation with varying brightness
 *                thresholds to exercise deep FAST-9 conditional logic paths
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum input size: need enough for configuration, threshold params, and image data
    // We need: config params (ints) + threshold params + multiple frames of pixel data
    const size_t MIN_INPUT_SIZE = sizeof(int) * 20 + 1024;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Insufficient input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Get encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }

    // Step 2: Get default encoder configuration
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_err_t cfg_err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (cfg_err != AOM_CODEC_OK) {
        return 0;
    }

    // Consume configuration parameters from fuzzed input
    // Fixed-size parameters first
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 512);
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    
    // FAST-9 corner detection threshold parameter 'b' variations
    // This is critical for triggering different code paths in aom_fast9_detect
    uint8_t brightness_threshold_low = fdp.ConsumeIntegral<uint8_t>();
    uint8_t brightness_threshold_high = fdp.ConsumeIntegral<uint8_t>();
    bool use_high_threshold = fdp.ConsumeBool();
    uint8_t active_brightness_threshold = use_high_threshold ? brightness_threshold_high : brightness_threshold_low;
    
    // Motion estimation configuration
    uint8_t enable_global_motion = fdp.ConsumeIntegral<uint8_t>() % 2;
    uint8_t enable_obmc = fdp.ConsumeIntegral<uint8_t>() % 2;
    uint8_t enable_tx64 = fdp.ConsumeIntegral<uint8_t>() % 2;
    uint8_t cpu_used = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    
    // Frame sequence parameters
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(2, 8);
    uint8_t brightness_variation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Update encoder configuration for motion estimation
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_threads = threads;
    enc_cfg.rc_target_bitrate = bitrate;
    enc_cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;  // No lookahead to force simpler motion estimation
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    // Set profile to support motion estimation features
    enc_cfg.g_profile = 0;  // Main profile
    
    // Step 3: Initialize encoder with aom_codec_enc_init_ver
    aom_codec_ctx_t encoder;
    aom_codec_flags_t flags = 0;
    
    aom_codec_err_t init_err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                                     flags, AOM_ENCODER_ABI_VERSION);
    if (init_err != AOM_CODEC_OK) {
        return 0;
    }
    
    // Step 4: Use aom_codec_enc_config_set to reconfigure for motion estimation
    // This tests the API mentioned in guidance
    aom_codec_enc_cfg_t updated_cfg = enc_cfg;
    
    // Modify configuration to potentially trigger different code paths
    // Changing parameters that affect motion estimation
    if (fdp.ConsumeBool()) {
        updated_cfg.g_error_resilient = fdp.ConsumeBool() ? AOM_ERROR_RESILIENT_DEFAULT : 0;
        aom_codec_err_t config_set_err = aom_codec_enc_config_set(&encoder, &updated_cfg);
        // Don't fail if config_set fails - it's part of testing error paths
    }
    
    // Set encoder controls for motion estimation
    // These controls influence whether FAST corner detection is used
    aom_codec_control(&encoder, AOME_SET_CPUUSED, cpu_used);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_OBMC, enable_obmc);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_TX64, enable_tx64);
    
    // Step 5: Create test image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Step 6: Encode sequence of frames with varying brightness patterns
    // The key innovation: generate pixel data directly from fuzzer input
    // to test the deep conditional logic in aom_fast9_detect
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Calculate how much pixel data we need for this frame
        size_t y_plane_size = width * height;
        size_t uv_plane_size = (width / 2) * (height / 2);
        size_t total_pixels_needed = y_plane_size + uv_plane_size * 2;
        
        // Check if we have enough fuzzer data remaining
        if (fdp.remaining_bytes() < total_pixels_needed) {
            // Not enough data for full frame, but we can still test with partial data
            break;
        }
        
        // Generate Y plane (luminance) pixel data directly from fuzzer input
        // This is critical: pixel values directly influence FAST corner detection
        uint8_t *y_plane = img->planes[0];
        const int y_stride = img->stride[0];
        
        // Apply brightness variation based on mode
        int brightness_shift = 0;
        switch (brightness_variation_mode) {
            case 0:  // Constant brightness
                brightness_shift = 0;
                break;
            case 1:  // Increasing brightness
                brightness_shift = frame_idx * 10;
                break;
            case 2:  // Decreasing brightness  
                brightness_shift = -frame_idx * 10;
                break;
            case 3:  // Alternating brightness
                brightness_shift = (frame_idx % 2) ? 30 : -30;
                break;
        }
        
        // Fill Y plane with fuzzer-generated pixel data
        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
                // Consume pixel value directly from fuzzer input
                uint8_t pixel_value = fdp.ConsumeIntegral<uint8_t>();
                
                // Apply brightness variation
                int adjusted_value = pixel_value + brightness_shift;
                
                // Clamp to valid range and apply threshold simulation
                // This creates patterns that should trigger FAST corner detection
                if (adjusted_value < 0) adjusted_value = 0;
                if (adjusted_value > 255) adjusted_value = 255;
                
                // Apply thresholding to create corner-like patterns
                // When pixel is much brighter/darker than threshold, it becomes a corner candidate
                uint8_t final_value = adjusted_value;
                
                // Create sharp transitions at threshold boundaries
                // This helps trigger the nested conditionals in FAST-9
                if (adjusted_value > (128 + active_brightness_threshold)) {
                    final_value = 255;  // Very bright - potential corner
                } else if (adjusted_value < (128 - active_brightness_threshold)) {
                    final_value = 0;    // Very dark - potential corner
                }
                
                y_plane[y * y_stride + x] = final_value;
            }
        }
        
        // Fill UV planes (chrominance) - simpler mid-gray for 4:2:0
        if (img->fmt == AOM_IMG_FMT_I420) {
            const int uv_width = width / 2;
            const int uv_height = height / 2;
            const int u_stride = img->stride[1];
            const int v_stride = img->stride[2];
            
            uint8_t *u_plane = img->planes[1];
            uint8_t *v_plane = img->planes[2];
            
            for (unsigned int y = 0; y < uv_height; y++) {
                for (unsigned int x = 0; x < uv_width; x++) {
                    u_plane[y * u_stride + x] = 128;  // Mid-gray
                    v_plane[y * v_stride + x] = 128;  // Mid-gray
                }
            }
        }
        
        // Set encoding flags
        aom_enc_frame_flags_t flags = 0;
        if (frame_idx == 0) {
            flags |= AOM_EFLAG_FORCE_KF;  // Force keyframe for first frame
        }
        
        // Step 7: Encode the frame - this should trigger global motion estimation
        // and potentially exercise aom_fast9_detect
        aom_codec_err_t encode_err = aom_codec_encode(&encoder, img, frame_idx, 1, flags);
        
        if (encode_err == AOM_CODEC_OK) {
            // Retrieve encoded data to exercise the full pipeline
            const aom_codec_cx_pkt_t *pkt = nullptr;
            aom_codec_iter_t iter = nullptr;
            
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Encoded frame data available
                    // For fuzzing, we don't need to process the data
                }
            }
        }
        
        // Vary parameters slightly between frames to encourage motion estimation
        // This simulates camera movement or scene changes
        if (frame_idx < num_frames - 1 && fdp.ConsumeBool()) {
            // Small random changes to simulate motion
            width = std::max(64u, std::min(512u, width + (fdp.ConsumeIntegral<uint8_t>() % 7) - 3));
            height = std::max(64u, std::min(512u, height + (fdp.ConsumeIntegral<uint8_t>() % 7) - 3));
            
            // Reallocate image if dimensions changed
            if (width != img->d_w || height != img->d_h) {
                aom_img_free(img);
                img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
                if (!img) {
                    aom_codec_destroy(&encoder);
                    return 0;
                }
            }
        }
    }
    
    // Step 8: Flush encoder to process any remaining data
    bool has_more_data = true;
    while (has_more_data) {
        aom_codec_err_t encode_err = aom_codec_encode(&encoder, nullptr, -1, 1, 0);
        if (encode_err != AOM_CODEC_OK) {
            break;
        }
        
        const aom_codec_cx_pkt_t *pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        has_more_data = false;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            has_more_data = true;
            // Process flushed frame if needed
        }
    }
    
    // Step 9: Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    
    return 0;
}

} // namespace harness_013
