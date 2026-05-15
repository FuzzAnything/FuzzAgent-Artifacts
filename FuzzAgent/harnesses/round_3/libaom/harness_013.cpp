#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cmath>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_013 {
/*
 * Fuzzing harness for libaom AV1 encoder targeting global motion estimation with advanced corner detection
 * Specifically targets aom_fast9_detect with 1039 blocked branches (52.6% uncovered) and fast9_corner_score with 676 blocked branches
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_013_guidance.md
 * Semantic differentiation from existing harnesses:
 * - harness_009: varied image patterns for FAST-9
 * - harness_010: circular pixel offset relationships and threshold binary search
 * - harness_011: systematic exploration of all 16 circular pixel combinations
 * - harness_012: targeted exploration of specific blocked branch patterns
 * - harness_013: advanced patterns focusing on remaining 1039 blocked branches with precise intensity relationships
 */



// Helper function to generate advanced patterns targeting specific blocked branches
// Based on make_offsets function which defines 16 circular pixel positions:
// pixel[0] = 0 + stride * 3    (right, 3 pixels down)
// pixel[1] = 1 + stride * 3    (right 1, down 3)
// pixel[2] = 2 + stride * 2    (right 2, down 2)
// pixel[3] = 3 + stride * 1    (right 3, down 1)
// pixel[4] = 3 + stride * 0    (right 3)
// pixel[5] = 3 + stride * -1   (right 3, up 1)
// pixel[6] = 2 + stride * -2   (right 2, up 2)
// pixel[7] = 1 + stride * -3   (right 1, up 3)
// pixel[8] = 0 + stride * -3   (left, 3 pixels up)
// pixel[9] = -1 + stride * -3  (left 1, up 3)
// pixel[10] = -2 + stride * -2 (left 2, up 2)
// pixel[11] = -3 + stride * -1 (left 3, up 1)
// pixel[12] = -3 + stride * 0  (left 3)
// pixel[13] = -3 + stride * 1  (left 3, down 1)
// pixel[14] = -2 + stride * 2  (left 2, down 2)
// pixel[15] = -1 + stride * 3  (left 1, down 3)
static void generate_advanced_branch_pattern(uint8_t* buffer, int width, int height, int stride,
                                             int pattern_type, int threshold_b,
                                             FuzzedDataProvider& fdp) {
    // Clear buffer to mid-gray baseline
    memset(buffer, 128, stride * height);
    
    // Consume pattern-specific parameters from fuzzer input
    int center_intensity = fdp.ConsumeIntegralInRange<int>(50, 200);
    int bright_offset = fdp.ConsumeIntegralInRange<int>(20, 100);
    int dark_offset = fdp.ConsumeIntegralInRange<int>(20, 100);
    int pattern_scale = fdp.ConsumeIntegralInRange<int>(4, 16);
    
    // Target specific blocked branch combinations from aom_fast9_detect
    switch (pattern_type % 8) {
        case 0: // Target complex nested condition: p[pixel[14]] < c_b && p[pixel[8]] < c_b && p[pixel[9]] < c_b ...
            // This corresponds to lines 3045-3067 in fast_9.c
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create region where pixel[14] and pixel[8] are dark relative to center
                    int region_type = ((x / pattern_scale) + (y / pattern_scale)) % 3;
                    
                    if (region_type == 0) {
                        // Region 1: Bright center with specific dark circular pixels
                        buffer[y * stride + x] = center_intensity + bright_offset;
                        
                        // Set pixel[14] (-2, +2) to be dark
                        int target_x = x - 2;
                        int target_y = y + 2;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                        
                        // Set pixel[8] (0, -3) to be dark
                        target_x = x;
                        target_y = y - 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                        
                        // Set pixel[9] (-1, -3) to be dark
                        target_x = x - 1;
                        target_y = y - 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                        
                        // Set pixel[10] (-2, -2) to be dark
                        target_x = x - 2;
                        target_y = y - 2;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                    } else if (region_type == 1) {
                        // Region 2: Alternate pattern for other branch combinations
                        buffer[y * stride + x] = center_intensity;
                        
                        // Create bright circular pixels to trigger > cb conditions
                        if ((x + y) % 5 == 0) {
                            // Set some circular pixels to be bright
                            for (int i = 0; i < 4; i++) {
                                int pixel_idx = fdp.ConsumeIntegralInRange<int>(0, 15);
                                int dx = 0, dy = 0;
                                
                                // Simplified offset mapping
                                if (pixel_idx < 8) {
                                    dx = 3 - pixel_idx % 4;
                                    dy = 3 - pixel_idx / 4;
                                } else {
                                    dx = -3 + (pixel_idx - 8) % 4;
                                    dy = -3 + (pixel_idx - 8) / 4;
                                }
                                
                                int target_x = x + dx;
                                int target_y = y + dy;
                                if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                    buffer[target_y * stride + target_x] = center_intensity + bright_offset + 30;
                                }
                            }
                        }
                    } else {
                        // Region 3: Mixed intensity for varied testing
                        int noise = fdp.ConsumeIntegralInRange<int>(-40, 40);
                        buffer[y * stride + x] = center_intensity + noise;
                    }
                }
            }
            break;
            
        case 1: // Target threshold boundary cases for fast9_corner_score binary search
            // Create precise intensity differences to exercise bmin/bmax search logic
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create regions with specific intensity relationships to threshold
                    int region_x = x / 8;
                    int region_y = y / 8;
                    int region = (region_x + region_y) % 8;
                    
                    int intensity;
                    switch (region) {
                        case 0: // Many pixels just above center+b threshold
                            intensity = 128 + threshold_b + fdp.ConsumeIntegralInRange<int>(1, 5);
                            break;
                        case 1: // Many pixels just below center+b threshold
                            intensity = 128 + threshold_b - fdp.ConsumeIntegralInRange<int>(1, 5);
                            break;
                        case 2: // Many pixels just above center-b threshold
                            intensity = 128 - threshold_b + fdp.ConsumeIntegralInRange<int>(1, 5);
                            break;
                        case 3: // Many pixels just below center-b threshold
                            intensity = 128 - threshold_b - fdp.ConsumeIntegralInRange<int>(1, 5);
                            break;
                        case 4: // Mixed bright/dark for complex decision tree
                            intensity = 128 + ((x + y) % 3 == 0 ? 60 : -40);
                            break;
                        case 5: // Edge intensities near 0/255 boundaries
                            intensity = fdp.ConsumeIntegralInRange<int>(0, 10) + 
                                       (fdp.ConsumeBool() ? 240 : 0);
                            break;
                        default:
                            intensity = 128 + fdp.ConsumeIntegralInRange<int>(-50, 50);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
        case 2: // Pattern for testing early exit conditions (9+ pixels > cb)
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create clusters where many circular pixels are bright
                    int cluster_id = ((x / 12) + (y / 12)) % 4;
                    
                    if (cluster_id == 0) {
                        // Dense bright cluster - should trigger early corner detection
                        buffer[y * stride + x] = 200;
                        
                        // Make many surrounding pixels bright
                        for (int dy = -3; dy <= 3; dy++) {
                            for (int dx = -3; dx <= 3; dx++) {
                                if (dx == 0 && dy == 0) continue;
                                int target_x = x + dx;
                                int target_y = y + dy;
                                if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                    buffer[target_y * stride + target_x] = 220;
                                }
                            }
                        }
                    } else if (cluster_id == 1) {
                        // Sparse bright pixels - should not trigger early exit
                        buffer[y * stride + x] = 128;
                        if ((x + y) % 7 == 0) {
                            buffer[y * stride + x] = 180;
                        }
                    } else {
                        // Gradient pattern
                        int gradient = (x * 100 / width) + (y * 100 / height);
                        buffer[y * stride + x] = 100 + gradient % 100;
                    }
                }
            }
            break;
            
        case 3: // Pattern focusing on specific pixel combinations from decision tree
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Analyze which circular pixel combinations are needed
                    int combo_type = (x * 7 + y * 11) % 16;
                    
                    int base_intensity = 128;
                    
                    // Apply specific intensity relationships based on combo_type
                    switch (combo_type) {
                        case 0:  // pixels 0-7 > cb, pixel[8] <= cb, pixel[15] > cb
                            base_intensity = 150;
                            // We'll adjust specific positions in post-processing
                            break;
                        case 1:  // pixels 0-6 > cb, pixel[7] < c_b, pixels 14-15 > cb
                            base_intensity = 130;
                            break;
                        case 2:  // Complex nested condition with many < c_b checks
                            base_intensity = 110;
                            break;
                        default:
                            base_intensity = 128 + fdp.ConsumeIntegralInRange<int>(-30, 30);
                    }
                    
                    buffer[y * stride + x] = base_intensity;
                    
                    // Apply specific circular pixel adjustments
                    if (combo_type == 0) {
                        // Make pixel[15] bright
                        int target_x = x - 1;
                        int target_y = y + 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = base_intensity + 50;
                        }
                    }
                }
            }
            break;
            
        case 4: // Gradient patterns with sudden changes to test intensity comparisons
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create smooth gradients with sudden discontinuities
                    float x_norm = (float)x / width;
                    float y_norm = (float)y / height;
                    
                    float gradient = sinf(x_norm * 10.0f) * cosf(y_norm * 8.0f) * 0.5f + 0.5f;
                    
                    // Add sudden changes at grid boundaries
                    if (x % 16 == 0 || y % 16 == 0) {
                        gradient += fdp.ConsumeFloatingPointInRange<float>(-0.3f, 0.3f);
                    }
                    
                    // Add noise for variation
                    float noise = fdp.ConsumeFloatingPointInRange<float>(-0.1f, 0.1f);
                    gradient += noise;
                    
                    int intensity = 100 + (int)(gradient * 100);
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        default: // Random pattern exploration for diverse testing
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int pattern_val = (x * 13 + y * 17 + pattern_type) % 256;
                    int variation = fdp.ConsumeIntegralInRange<int>(-50, 50);
                    int intensity = (pattern_val + variation) % 256;
                    
                    // Enhance corner-like features
                    if ((x % pattern_scale == pattern_scale/2) && 
                        (y % pattern_scale == pattern_scale/2)) {
                        intensity = (intensity + bright_offset) % 256;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)intensity;
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(64, 320);
    int height = fdp.ConsumeIntegralInRange<int>(64, 240);
    int bitrate = fdp.ConsumeIntegralInRange<int>(100, 1000);
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 100);
    int threshold_b = fdp.ConsumeIntegralInRange<int>(1, 50);
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    // Get AV1 encoder interface
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Step 1: Get default encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Configure encoder settings
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = bitrate;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    
    // Enable features that use global motion estimation
    cfg.rc_end_usage = AOM_VBR;
    cfg.g_usage = AOM_USAGE_REALTIME;
    
    // Step 2: Initialize encoder with aom_codec_enc_init_ver
    if (aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set encoder control parameters
    aom_codec_control(&codec, AOME_SET_CPUUSED, 8);
    aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, 1);
    aom_codec_control(&codec, AV1E_SET_ENABLE_OBMC, 0);
    aom_codec_control(&codec, AOME_SET_ENABLEAUTOALTREF, 0);
    
    int result = 0;
    
    // Encode multiple frames to exercise different code paths
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Step 3: Create test image with corner patterns using aom_img_alloc
        aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
        
        if (!img) {
            result = 0;
            goto cleanup;
        }
        
        // Generate Y plane data with corner detection patterns
        generate_advanced_branch_pattern(img->planes[0], width, height, img->stride[0],
                                         pattern_type + frame_idx, threshold_b, fdp);
        
        // Generate U and V planes (subsampled)
        int uv_width = (width + 1) / 2;
        int uv_height = (height + 1) / 2;
        
        for (int y = 0; y < uv_height; y++) {
            for (int x = 0; x < uv_width; x++) {
                img->planes[1][y * img->stride[1] + x] = 128;
                img->planes[2][y * img->stride[2] + x] = 128;
            }
        }
        
        // Step 4: Encode the frame to trigger global motion estimation and corner detection
        aom_codec_err_t encode_result = aom_codec_encode(&codec, img, frame_idx, 1, 0);
        
        if (encode_result != AOM_CODEC_OK) {
            aom_img_free(img);
            continue;
        }
        
        // Step 5: Retrieve encoded output data
        const aom_codec_cx_pkt_t* pkt = NULL;
        aom_codec_iter_t iter = NULL;
        
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process different packet types
            switch (pkt->kind) {
                case AOM_CODEC_CX_FRAME_PKT:
                    // Frame data available - could save or process it
                    break;
                case AOM_CODEC_STATS_PKT:
                    // Statistics packet
                    break;
                default:
                    // Other packet types
                    break;
            }
        }
        
        // Free the image
        aom_img_free(img);
        
        // Check if we have enough input remaining for next frame
        if (fdp.remaining_bytes() < 32) {
            break;
        }
    }
    
    // Step 6: Clean up encoder
    aom_codec_destroy(&codec);
    
    return 0;

cleanup:
    // Ensure cleanup on error
    aom_codec_destroy(&codec);
    return result;
}

} // namespace harness_013
