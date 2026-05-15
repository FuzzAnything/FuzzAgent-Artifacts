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

namespace harness_014 {
/*
 * Fuzzing harness for libaom AV1 encoder targeting the FAST-9 corner detection algorithm
 * Specifically targets aom_fast9_detect with 1039 blocked branches for global motion estimation
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_014_guidance.md
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_009: varied image patterns for FAST-9
 * - harness_010: circular pixel offset relationships and threshold binary search
 * - harness_011: systematic exploration of all 16 circular pixel combinations  
 * - harness_012: targeted exploration of specific blocked branch patterns
 * - harness_013: advanced patterns focusing on remaining 1039 blocked branches
 * - harness_014: Comprehensive exploration of edge cases and complex intensity relationships
 *                specifically for global motion estimation pipeline
 */



// Helper function to generate diverse image patterns targeting FAST-9 corner detection
// Focuses on edge cases mentioned in guidance: homogeneous regions, high-contrast edges, 
// sparse corners, varying noise levels, chessboard/grid patterns
static void generate_fast9_pattern(uint8_t* buffer, int width, int height, int stride,
                                   int pattern_type, int threshold_b,
                                   FuzzedDataProvider& fdp) {
    // Clear buffer to mid-gray baseline
    memset(buffer, 128, stride * height);
    
    // Consume pattern parameters from fuzzer input
    int base_intensity = fdp.ConsumeIntegralInRange<int>(50, 200);
    int contrast_range = fdp.ConsumeIntegralInRange<int>(20, 100);
    int noise_level = fdp.ConsumeIntegralInRange<int>(0, 50);
    int pattern_size = fdp.ConsumeIntegralInRange<int>(4, 32);
    int sparse_factor = fdp.ConsumeIntegralInRange<int>(2, 8);
    
    // Generate different pattern types based on fuzzer input
    switch (pattern_type % 10) {
        case 0: // Chessboard pattern - ideal for corner detection
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int chess_val = ((x / pattern_size) + (y / pattern_size)) % 2;
                    int intensity = base_intensity + (chess_val ? contrast_range : -contrast_range);
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        case 1: // Grid pattern with varying cell sizes
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int grid_x = x % pattern_size;
                    int grid_y = y % pattern_size;
                    int border_size = pattern_size / 4;
                    
                    // Create grid lines (dark) and cells (bright)
                    if (grid_x < border_size || grid_y < border_size || 
                        grid_x >= pattern_size - border_size || grid_y >= pattern_size - border_size) {
                        buffer[y * stride + x] = base_intensity - contrast_range; // Dark grid lines
                    } else {
                        buffer[y * stride + x] = base_intensity + contrast_range; // Bright cells
                    }
                }
            }
            break;
            
        case 2: // Homogeneous regions with sparse corners
            // Create large homogeneous areas
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    buffer[y * stride + x] = base_intensity;
                }
            }
            
            // Add sparse corners at grid points
            for (int y = pattern_size; y < height - 3; y += pattern_size * sparse_factor) {
                for (int x = pattern_size; x < width - 3; x += pattern_size * sparse_factor) {
                    // Create a corner pattern around (x,y)
                    for (int dy = -3; dy <= 3; dy++) {
                        for (int dx = -3; dx <= 3; dx++) {
                            int target_x = x + dx;
                            int target_y = y + dy;
                            if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                // Create intensity pattern that might trigger corner detection
                                int dist = std::max(abs(dx), abs(dy));
                                int intensity = base_intensity + (dist <= 1 ? contrast_range : -contrast_range);
                                buffer[target_y * stride + target_x] = (uint8_t)intensity;
                            }
                        }
                    }
                }
            }
            break;
            
        case 3: // High-contrast edges (vertical and horizontal)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int edge_type = (x / pattern_size) % 3;
                    int intensity;
                    
                    switch (edge_type) {
                        case 0: // Vertical edge
                            intensity = (x % pattern_size < pattern_size / 2) ? 
                                       base_intensity + contrast_range : 
                                       base_intensity - contrast_range;
                            break;
                        case 1: // Horizontal edge  
                            intensity = (y % pattern_size < pattern_size / 2) ?
                                       base_intensity + contrast_range :
                                       base_intensity - contrast_range;
                            break;
                        default: // Diagonal edge
                            intensity = ((x + y) % pattern_size < pattern_size / 2) ?
                                       base_intensity + contrast_range :
                                       base_intensity - contrast_range;
                            break;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)intensity;
                }
            }
            break;
            
        case 4: // Varying noise levels over gradient
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create gradient background
                    float x_norm = (float)x / width;
                    float y_norm = (float)y / height;
                    float gradient = (x_norm + y_norm) / 2.0f;
                    
                    int base_val = base_intensity + (int)(gradient * 100) - 50;
                    
                    // Add varying noise
                    int noise = fdp.ConsumeIntegralInRange<int>(-noise_level, noise_level);
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, base_val + noise));
                }
            }
            break;
            
        case 5: // Concentric circles (radial patterns)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Calculate distance from center
                    float dx = (float)x - width / 2.0f;
                    float dy = (float)y - height / 2.0f;
                    float distance = sqrtf(dx*dx + dy*dy);
                    
                    // Create concentric rings
                    int ring = (int)(distance / pattern_size) % 2;
                    int intensity = base_intensity + (ring ? contrast_range : -contrast_range);
                    
                    buffer[y * stride + x] = (uint8_t)intensity;
                }
            }
            break;
            
        case 6: // Textured patterns with varying complexity
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create complex texture using multiple sine waves
                    float texture = 0.0f;
                    texture += 0.3f * sinf(x * 0.1f);
                    texture += 0.2f * sinf(y * 0.15f);
                    texture += 0.1f * sinf((x + y) * 0.05f);
                    texture += 0.1f * sinf((x - y) * 0.08f);
                    
                    int intensity = base_intensity + (int)(texture * contrast_range);
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        case 7: // Pixel neighborhood intensity relationships for specific branches
            // Target specific circular pixel patterns from aom_fast9_detect
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    int pattern_id = ((x / 8) + (y / 8)) % 4;
                    
                    switch (pattern_id) {
                        case 0: // Pattern for: p[pixel[14]] < c_b && p[pixel[8]] < c_b
                            buffer[y * stride + x] = base_intensity + contrast_range; // Bright center
                            // Set specific circular pixels to be dark
                            if (x - 2 >= 0 && y + 2 < height) buffer[(y+2)*stride + (x-2)] = base_intensity - contrast_range; // pixel[14]
                            if (y - 3 >= 0) buffer[(y-3)*stride + x] = base_intensity - contrast_range; // pixel[8]
                            break;
                            
                        case 1: // Pattern for: p[pixel[7]] < c_b && p[pixel[14]] > cb
                            buffer[y * stride + x] = base_intensity; // Neutral center
                            if (x + 1 >= 0 && y - 3 >= 0) buffer[(y-3)*stride + (x+1)] = base_intensity - contrast_range; // pixel[7]
                            if (x - 2 >= 0 && y + 2 < height) buffer[(y+2)*stride + (x-2)] = base_intensity + contrast_range; // pixel[14]
                            break;
                            
                        case 2: // Pattern for complex nested condition with multiple < c_b checks
                            buffer[y * stride + x] = base_intensity - contrast_range/2; // Slightly dark center
                            // Set multiple circular pixels to be very dark
                            for (int dy = -3; dy <= 3; dy++) {
                                for (int dx = -3; dx <= 3; dx++) {
                                    if (dx == 0 && dy == 0) continue;
                                    if (abs(dx) == 3 || abs(dy) == 3 || (abs(dx) == 2 && abs(dy) == 2)) {
                                        int target_x = x + dx;
                                        int target_y = y + dy;
                                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                            buffer[target_y * stride + target_x] = base_intensity - contrast_range;
                                        }
                                    }
                                }
                            }
                            break;
                            
                        default: // Random pattern for exploration
                            int rand_val = fdp.ConsumeIntegralInRange<int>(0, 255);
                            buffer[y * stride + x] = (uint8_t)rand_val;
                            break;
                    }
                }
            }
            break;
            
        case 8: // Mixed patterns combining multiple approaches
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int region = (x / (width/3)) + 3 * (y / (height/3));
                    
                    switch (region % 4) {
                        case 0: // Homogeneous
                            buffer[y * stride + x] = base_intensity;
                            break;
                        case 1: // High frequency noise
                            buffer[y * stride + x] = base_intensity + fdp.ConsumeIntegralInRange<int>(-30, 30);
                            break;
                        case 2: // Regular pattern
                            buffer[y * stride + x] = ((x/4 + y/4) % 2) ? 
                                                   base_intensity + contrast_range : 
                                                   base_intensity - contrast_range;
                            break;
                        default: // Gradient
                            float grad = (float)x / width;
                            buffer[y * stride + x] = base_intensity + (int)(grad * contrast_range * 2) - contrast_range;
                            break;
                    }
                }
            }
            break;
            
        default: // Random exploration for remaining input
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (fdp.remaining_bytes() > 0) {
                        buffer[y * stride + x] = fdp.ConsumeIntegral<uint8_t>();
                    } else {
                        buffer[y * stride + x] = 128;
                    }
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(64, 320);
    int height = fdp.ConsumeIntegralInRange<int>(64, 240);
    int bitrate = fdp.ConsumeIntegralInRange<int>(100, 1000);
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 100);
    int threshold_b = fdp.ConsumeIntegralInRange<int>(1, 50);  // Brightness threshold for corner detection
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    // Get AV1 encoder interface
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Step 1: Get default encoder configuration using aom_codec_enc_config_default
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
    
    // Enable features that use global motion estimation (which uses FAST-9 corner detection)
    cfg.rc_end_usage = AOM_VBR;
    cfg.g_usage = AOM_USAGE_REALTIME;
    
    // Step 2: Initialize encoder with aom_codec_enc_init_ver
    if (aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set encoder control parameters to enable global motion estimation
    aom_codec_control(&codec, AOME_SET_CPUUSED, 8);  // Faster encoding for fuzzing
    aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, 1);  // Enable global motion estimation
    aom_codec_control(&codec, AV1E_SET_ENABLE_OBMC, 0);  // Disable OBMC
    aom_codec_control(&codec, AOME_SET_ENABLEAUTOALTREF, 0);  // Disable altref
    
    int result = 0;
    
    // Encode multiple frames to exercise different code paths
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Step 3: Create test image with corner patterns using aom_img_alloc
        aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
        
        if (!img) {
            result = 0;
            goto cleanup;
        }
        
        // Generate Y plane data with diverse corner detection patterns
        generate_fast9_pattern(img->planes[0], width, height, img->stride[0],
                               pattern_type + frame_idx, threshold_b, fdp);
        
        // Generate U and V planes (subsampled to 4:2:0 format)
        int uv_width = (width + 1) / 2;
        int uv_height = (height + 1) / 2;
        
        // Fill chroma planes with mid values
        for (int y = 0; y < uv_height; y++) {
            for (int x = 0; x < uv_width; x++) {
                img->planes[1][y * img->stride[1] + x] = 128;
                img->planes[2][y * img->stride[2] + x] = 128;
            }
        }
        
        // Step 4: Encode the frame to trigger global motion estimation and FAST-9 corner detection
        aom_codec_err_t encode_result = aom_codec_encode(&codec, img, frame_idx, 1, 0);
        
        if (encode_result != AOM_CODEC_OK) {
            aom_img_free(img);
            continue;  // Continue with next frame even if this one fails
        }
        
        // Retrieve encoded output data to ensure encoding was successful
        const aom_codec_cx_pkt_t* pkt = NULL;
        aom_codec_iter_t iter = NULL;
        
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process different packet types
            switch (pkt->kind) {
                case AOM_CODEC_CX_FRAME_PKT:
                    // Frame data available - encoding was successful
                    break;
                case AOM_CODEC_STATS_PKT:
                    // Statistics packet
                    break;
                default:
                    // Other packet types
                    break;
            }
        }
        
        // Free the image using aom_img_free
        aom_img_free(img);
        
        // Check if we have enough input remaining for next frame
        if (fdp.remaining_bytes() < 50) {
            break;
        }
    }
    
    // Step 5: Clean up encoder using aom_codec_destroy
    aom_codec_destroy(&codec);
    
    return 0;

cleanup:
    // Ensure cleanup on error
    aom_codec_destroy(&codec);
    return result;
}

} // namespace harness_014
