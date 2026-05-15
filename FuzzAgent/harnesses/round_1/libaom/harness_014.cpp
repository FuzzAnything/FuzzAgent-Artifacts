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

namespace harness_014 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom AV1 encoder focusing on deep FAST-9
 * corner detection paths (aom_fast9_detect) with precise pixel intensity
 * relationships to trigger 1903 blocked branches (96.4% uncovered).
 * 
 * Primary Target: aom_fast9_detect blocked branches - targeting specific
 * pixel intensity relationships p[pixel[7..15]] < center - threshold
 * 
 * APIs targeted as per coverage guidance:
 * - aom_codec_enc_init_ver (Required Setup)
 * - aom_img_alloc (Create input frames with precise corner patterns)
 * - aom_codec_encode (Primary entry point triggering global motion estimation)
 * - aom_codec_get_cx_data (Retrieve encoded output)
 * - aom_img_free, aom_codec_destroy (Cleanup)
 * 
 * Semantic differentiation from previous 14 harnesses:
 * - harness_012: Global motion estimation with algorithmic corner patterns
 * - harness_013: Direct fuzzer-driven pixel generation with varying brightness thresholds
 * - harness_014: Precise FAST-9 circular pattern simulation targeting specific
 *                blocked branches with exact pixel intensity relationships
 *                (center pixel vs. surrounding circular pattern positions)
 */



// Helper to generate images with precise FAST-9 corner patterns
// FAST-9 uses a circular pattern of 16 pixels around a center pixel
// Positions (relative to center, in circular order):
// pixel[0]: (-1, -3)   pixel[8]: (1, 3)
// pixel[1]: (0, -3)    pixel[9]: (3, 2)
// pixel[2]: (1, -3)    pixel[10]: (3, 1)
// pixel[3]: (2, -2)    pixel[11]: (3, 0)
// pixel[4]: (3, -1)    pixel[12]: (3, -1)
// pixel[5]: (3, 0)     pixel[13]: (2, -2)
// pixel[6]: (3, 1)     pixel[14]: (1, -3)
// pixel[7]: (2, 2)     pixel[15]: (0, -3)
//
// The algorithm checks for consecutive sets of 9+ pixels that are all
// brighter than center+threshold (cb) or darker than center-threshold (c_b)
static void generate_fast9_targeted_pattern(aom_image_t *img, uint8_t pattern_type,
                                           uint8_t threshold, uint8_t center_intensity,
                                           int frame_index) {
    if (!img || !img->planes[0]) return;
    
    const int width = img->d_w;
    const int height = img->d_h;
    const int stride = img->stride[0];
    uint8_t *y_plane = img->planes[0];
    
    // Fill with base intensity
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            y_plane[y * stride + x] = 128; // Mid-gray background
        }
    }
    
    // Place multiple corner patterns throughout the image
    // Each pattern targets specific blocked branches in aom_fast9_detect
    int patterns_placed = 0;
    
    // Use pattern_type to determine which blocked branches to target
    uint8_t branch_target = pattern_type % 16;
    
    // Place corner patterns on a grid
    for (int grid_y = 20; grid_y < height - 20 && patterns_placed < 10; grid_y += 40) {
        for (int grid_x = 20; grid_x < width - 20 && patterns_placed < 10; grid_x += 40) {
            int center_x = grid_x + (patterns_placed * 3) % 20;
            int center_y = grid_y + (patterns_placed * 7) % 20;
            
            if (center_x < 3 || center_x >= width - 3 || 
                center_y < 3 || center_y >= height - 3) {
                continue;
            }
            
            // Set center pixel intensity
            y_plane[center_y * stride + center_x] = center_intensity;
            
            // Calculate cb and c_b thresholds
            uint8_t cb = (center_intensity > 255 - threshold) ? 255 : center_intensity + threshold;
            uint8_t c_b = (center_intensity < threshold) ? 0 : center_intensity - threshold;
            
            // Target specific blocked branches based on pattern_type
            switch (branch_target) {
                case 0: // Target p[pixel[7..15]] < c_b (darker pixels)
                    // Make pixels 7-15 significantly darker than center-threshold
                    for (int i = 7; i <= 15; i++) {
                        int offset_x = 0, offset_y = 0;
                        // Simplified circular pattern positions
                        if (i == 7) { offset_x = 2; offset_y = 2; }
                        else if (i == 8) { offset_x = 1; offset_y = 3; }
                        else if (i == 9) { offset_x = 3; offset_y = 2; }
                        else if (i == 10) { offset_x = 3; offset_y = 1; }
                        else if (i == 11) { offset_x = 3; offset_y = 0; }
                        else if (i == 12) { offset_x = 3; offset_y = -1; }
                        else if (i == 13) { offset_x = 2; offset_y = -2; }
                        else if (i == 14) { offset_x = 1; offset_y = -3; }
                        else if (i == 15) { offset_x = 0; offset_y = -3; }
                        
                        int px = center_x + offset_x;
                        int py = center_y + offset_y;
                        if (px >= 0 && px < width && py >= 0 && py < height) {
                            y_plane[py * stride + px] = c_b - 10; // Ensure darker than threshold
                        }
                    }
                    break;
                    
                case 1: // Target p[pixel[0..8]] > cb (brighter pixels)
                    // Make pixels 0-8 significantly brighter than center+threshold
                    for (int i = 0; i <= 8; i++) {
                        int offset_x = 0, offset_y = 0;
                        if (i == 0) { offset_x = -1; offset_y = -3; }
                        else if (i == 1) { offset_x = 0; offset_y = -3; }
                        else if (i == 2) { offset_x = 1; offset_y = -3; }
                        else if (i == 3) { offset_x = 2; offset_y = -2; }
                        else if (i == 4) { offset_x = 3; offset_y = -1; }
                        else if (i == 5) { offset_x = 3; offset_y = 0; }
                        else if (i == 6) { offset_x = 3; offset_y = 1; }
                        else if (i == 7) { offset_x = 2; offset_y = 2; }
                        else if (i == 8) { offset_x = 1; offset_y = 3; }
                        
                        int px = center_x + offset_x;
                        int py = center_y + offset_y;
                        if (px >= 0 && px < width && py >= 0 && py < height) {
                            y_plane[py * stride + px] = cb + 10; // Ensure brighter than threshold
                        }
                    }
                    break;
                    
                case 2: // Mixed pattern to trigger complex conditional paths
                    // Alternate bright/dark in specific sequences
                    for (int i = 0; i < 16; i++) {
                        int offset_x = 0, offset_y = 0;
                        // Simplified offsets
                        if (i < 8) {
                            offset_x = (i % 4) * (i < 4 ? 1 : -1);
                            offset_y = ((i / 4) * 2 + 1) * (i < 4 ? -1 : 1);
                        } else {
                            offset_x = ((i-8) % 4) * (i < 12 ? 1 : -1);
                            offset_y = (((i-8) / 4) * 2 + 2) * (i < 12 ? -1 : 1);
                        }
                        
                        int px = center_x + offset_x;
                        int py = center_y + offset_y;
                        if (px >= 0 && px < width && py >= 0 && py < height) {
                            // Create specific intensity relationships to trigger
                            // different conditional branches
                            if ((patterns_placed + i) % 3 == 0) {
                                y_plane[py * stride + px] = cb + 5; // Slightly brighter
                            } else if ((patterns_placed + i) % 3 == 1) {
                                y_plane[py * stride + px] = c_b - 5; // Slightly darker
                            } else {
                                y_plane[py * stride + px] = center_intensity; // Same as center
                            }
                        }
                    }
                    break;
                    
                default:
                    // Generic corner pattern with varying intensities
                    for (int dy = -3; dy <= 3; dy++) {
                        for (int dx = -3; dx <= 3; dx++) {
                            int px = center_x + dx;
                            int py = center_y + dy;
                            if (px >= 0 && px < width && py >= 0 && py < height) {
                                // Circular pattern approximation
                                int dist_sq = dx*dx + dy*dy;
                                if (dist_sq <= 9) { // Within radius 3
                                    // Vary intensity based on position
                                    uint8_t intensity = 128 + ((dx + dy + patterns_placed) % 7) * 20;
                                    y_plane[py * stride + px] = intensity;
                                }
                            }
                        }
                    }
                    break;
            }
            
            patterns_placed++;
        }
    }
    
    // Fill UV planes for I420 format
    if (img->fmt == AOM_IMG_FMT_I420) {
        const int uv_width = width / 2;
        const int uv_height = height / 2;
        const int u_stride = img->stride[1];
        const int v_stride = img->stride[2];
        
        uint8_t *u_plane = img->planes[1];
        uint8_t *v_plane = img->planes[2];
        
        for (int y = 0; y < uv_height; y++) {
            for (int x = 0; x < uv_width; x++) {
                u_plane[y * u_stride + x] = 128;
                v_plane[y * v_stride + x] = 128;
            }
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum input size: need config params, threshold, center intensity, and pattern type
    const size_t MIN_INPUT_SIZE = sizeof(int) * 10 + 16;
    if (size < MIN_INPUT_SIZE) {
        return 0;
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
    
    // Consume FAST-9 specific parameters from fuzzed input
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(128, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(128, 512);
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(500, 5000);
    
    // FAST-9 threshold parameter (b in the algorithm)
    uint8_t fast9_threshold = fdp.ConsumeIntegralInRange<uint8_t>(10, 50);
    
    // Center pixel intensity variations
    uint8_t center_intensity_low = fdp.ConsumeIntegralInRange<uint8_t>(50, 100);
    uint8_t center_intensity_high = fdp.ConsumeIntegralInRange<uint8_t>(150, 200);
    bool use_high_center = fdp.ConsumeBool();
    uint8_t center_intensity = use_high_center ? center_intensity_high : center_intensity_low;
    
    // Pattern type for targeting specific blocked branches
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>();
    
    // Motion estimation configuration
    uint8_t enable_global_motion = fdp.ConsumeIntegral<uint8_t>() % 2;
    uint8_t cpu_used = fdp.ConsumeIntegralInRange<uint8_t>(0, 8);
    
    // Frame sequence parameters
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(2, 6);
    uint8_t pattern_variation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Update encoder configuration
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_threads = threads;
    enc_cfg.rc_target_bitrate = bitrate;
    enc_cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0; // No lookahead
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_profile = 0; // Main profile
    
    // Step 3: Initialize encoder with aom_codec_enc_init_ver
    aom_codec_ctx_t encoder;
    aom_codec_flags_t flags = 0;
    
    aom_codec_err_t init_err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg,
                                                     flags, AOM_ENCODER_ABI_VERSION);
    if (init_err != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set encoder controls for motion estimation
    AOM_CODEC_CONTROL_TYPECHECKED(&encoder, AOME_SET_CPUUSED, cpu_used);
    
    // Enable global motion estimation (critical for reaching aom_fast9_detect)
    int enable_global_motion_val = enable_global_motion ? 1 : 0;
    AOM_CODEC_CONTROL_TYPECHECKED(&encoder, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion_val);
    // Step 4: Create test image with aom_img_alloc
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Encode multiple frames with varying patterns
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Generate image with targeted FAST-9 patterns
        generate_fast9_targeted_pattern(img, 
                                       (pattern_type + frame_idx) % 16,
                                       fast9_threshold,
                                       center_intensity,
                                       frame_idx);
        
        // Set encoding flags
        aom_enc_frame_flags_t flags = 0;
        if (frame_idx == 0) {
            flags |= AOM_EFLAG_FORCE_KF; // Force keyframe for first frame
        }
        
        // Step 5: Encode the frame - should trigger global motion estimation
        // and potentially exercise aom_fast9_detect
        aom_codec_err_t encode_err = aom_codec_encode(&encoder, img, frame_idx, 1, flags);
        
        if (encode_err == AOM_CODEC_OK) {
            // Step 6: Retrieve encoded data with aom_codec_get_cx_data
            const aom_codec_cx_pkt_t *pkt = nullptr;
            aom_codec_iter_t iter = nullptr;
            
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Encoded frame data available
                    // For fuzzing, we don't need to process the data
                }
            }
        }
        
        // Vary parameters between frames to encourage motion estimation
        if (frame_idx < num_frames - 1 && fdp.ConsumeBool()) {
            // Small variations to simulate scene changes
            fast9_threshold = std::max<uint8_t>(10, std::min<uint8_t>(50, 
                fast9_threshold + (fdp.ConsumeIntegral<uint8_t>() % 5) - 2));
            
            center_intensity = std::max<uint8_t>(50, std::min<uint8_t>(200,
                center_intensity + (fdp.ConsumeIntegral<uint8_t>() % 11) - 5));
        }
    }
    
    // Flush encoder
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
        }
    }
    
    // Step 7 & 8: Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    
    return 0;
}

} // namespace harness_014
