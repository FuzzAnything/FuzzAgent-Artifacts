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

namespace harness_012 {
/*
 * Fuzzing harness for libaom AV1 encoder targeting aom_fast9_detect with 906 blocked branches
 * Specifically focuses on systematic exploration of deeply nested decision tree branches
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_012_guidance.md
 * Semantic differentiation: 
 * - harness_009: varied image patterns for FAST-9
 * - harness_010: circular pixel offset relationships and threshold binary search  
 * - harness_011: systematic exploration of all 16 circular pixel combinations
 * - harness_012: targeted exploration of specific blocked branch patterns in aom_fast9_detect
 * Targets blocked patterns like: p[pixel[14]] < c_b && p[pixel[8]] < c_b && p[pixel[9]] < c_b etc.
 */



// Helper function to generate images targeting specific blocked branches in aom_fast9_detect
// The function examines 16 pixels in circular pattern around each point (make_offsets):
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
static void generate_targeted_branch_pattern(uint8_t* buffer, int width, int height, int stride,
                                             int pattern_type, int threshold_b,
                                             FuzzedDataProvider& fdp) {
    // Clear buffer to mid-gray baseline
    memset(buffer, 128, stride * height);
    
    // Consume pattern-specific parameters from fuzzer input
    int center_intensity = fdp.ConsumeIntegralInRange<int>(50, 200);
    int bright_offset = fdp.ConsumeIntegralInRange<int>(20, 100);
    int dark_offset = fdp.ConsumeIntegralInRange<int>(20, 100);
    int pattern_scale = fdp.ConsumeIntegralInRange<int>(4, 16);
    
    // The aom_fast9_detect function has complex nested conditions
    // We'll create patterns that target specific blocked branch combinations
    switch (pattern_type % 12) {
        case 0: // Target pattern: p[pixel[14]] < c_b && p[pixel[8]] < c_b && p[pixel[9]] < c_b ...
            // This corresponds to lines 3045-3067 in fast_9.c
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Position determines which pattern to apply
                    int grid_x = (x / pattern_scale) % 2;
                    int grid_y = (y / pattern_scale) % 2;
                    
                    if (grid_x == 0 && grid_y == 0) {
                        // Region where pixel[14] and pixel[8] are dark relative to center
                        buffer[y * stride + x] = center_intensity + bright_offset; // Bright center
                        
                        // Set specific circular positions to be dark
                        // pixel[14] = -2 + stride * 2 (left 2, down 2)
                        int target_x = x - 2;
                        int target_y = y + 2;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                        
                        // pixel[8] = 0 + stride * -3 (left, 3 pixels up)
                        target_x = x;
                        target_y = y - 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                        
                        // pixel[9] = -1 + stride * -3 (left 1, up 3)
                        target_x = x - 1;
                        target_y = y - 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                    } else {
                        buffer[y * stride + x] = center_intensity;
                    }
                }
            }
            break;
            
        case 1: // Target pattern: p[pixel[13]] > cb && p[pixel[14]] > cb && p[pixel[15]] > cb
            // Lines 3137-3144 in fast_9.c
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    int region = ((x / 8) + (y / 8)) % 3;
                    
                    if (region == 0) {
                        // Region where pixels 13,14,15 are bright
                        buffer[y * stride + x] = center_intensity - dark_offset; // Dark center
                        
                        // pixel[13] = -3 + stride * 1 (left 3, down 1)
                        int target_x = x - 3;
                        int target_y = y + 1;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                        }
                        
                        // pixel[14] = -2 + stride * 2 (left 2, down 2)
                        target_x = x - 2;
                        target_y = y + 2;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                        }
                        
                        // pixel[15] = -1 + stride * 3 (left 1, down 3)
                        target_x = x - 1;
                        target_y = y + 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                        }
                    } else {
                        buffer[y * stride + x] = center_intensity;
                    }
                }
            }
            break;
            
        case 2: // Target pattern: p[pixel[7]] < c_b && p[pixel[14]] > cb && p[pixel[15]] > cb
            // Lines 3039-3044 in fast_9.c
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create mixed pattern
                    if ((x + y) % 11 == 0) {
                        // Mixed relationship region
                        buffer[y * stride + x] = center_intensity;
                        
                        // pixel[7] = 1 + stride * -3 (right 1, up 3) - make it dark
                        int target_x = x + 1;
                        int target_y = y - 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                        }
                        
                        // pixel[14] and pixel[15] - make them bright
                        target_x = x - 2;
                        target_y = y + 2;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                        }
                        
                        target_x = x - 1;
                        target_y = y + 3;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                        }
                    } else {
                        buffer[y * stride + x] = center_intensity;
                    }
                }
            }
            break;
            
        case 3: // Target pattern for fast9_corner_score blocked branches
            // Create gradient patterns that vary the threshold b
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create intensity gradient
                    int gradient_x = (x * 200) / std::max(1, width);
                    int gradient_y = (y * 100) / std::max(1, height);
                    
                    // Add noise for threshold variations
                    int noise = fdp.ConsumeIntegralInRange<int>(-30, 30);
                    int intensity = 100 + gradient_x + gradient_y + noise;
                    
                    // Create sudden intensity changes at grid boundaries
                    if ((x % 16 == 0) || (y % 16 == 0)) {
                        intensity += fdp.ConsumeIntegralInRange<int>(-80, 80);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        case 4: // Pattern targeting the binary search threshold logic
            // Create images with precise intensity differences for bmin/bmax search
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create regions with specific intensity relationships
                    int region = (x / 12) + 3 * (y / 12);
                    
                    int intensity;
                    switch (region % 8) {
                        case 0: // Many pixels > center+b
                            intensity = 200 + fdp.ConsumeIntegralInRange<int>(0, 30);
                            break;
                        case 1: // Many pixels < center-b
                            intensity = 60 + fdp.ConsumeIntegralInRange<int>(0, 30);
                            break;
                        case 2: // Mixed bright/dark in circular pattern
                            intensity = 130 + ((x + y) % 7 == 0 ? 70 : -40);
                            break;
                        case 3: // Edge case for threshold boundary
                            intensity = 128 + threshold_b % 10;
                            break;
                        case 4: // Just below threshold
                            intensity = 128 + (threshold_b / 2);
                            break;
                        case 5: // Just above threshold
                            intensity = 128 - (threshold_b / 2);
                            break;
                        default:
                            intensity = 128 + fdp.ConsumeIntegralInRange<int>(-50, 50);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        default: // Various other patterns to explore different code paths
            // Create diverse patterns based on fuzzer input
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int pattern_val = (x * y + pattern_type) % 256;
                    int variation = fdp.ConsumeIntegralInRange<int>(-40, 40);
                    int intensity = (pattern_val + variation + center_intensity) % 256;
                    
                    // Add structured variations for corner detection
                    if ((x % pattern_scale == 0) && (y % pattern_scale == 0)) {
                        intensity += bright_offset;
                    }
                    if (((x + pattern_scale/2) % pattern_scale == 0) && 
                        ((y + pattern_scale/2) % pattern_scale == 0)) {
                        intensity -= dark_offset;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters
    int width = fdp.ConsumeIntegralInRange<int>(64, 320);
    int height = fdp.ConsumeIntegralInRange<int>(64, 240);
    int bitrate = fdp.ConsumeIntegralInRange<int>(100, 1000);
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 100);
    int threshold_b = fdp.ConsumeIntegralInRange<int>(1, 50);
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    // Initialize encoder
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = bitrate;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Configure global motion estimation (critical for FAST-9 path)
    aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, 1);
    
    // Configure quality/bitrate settings that encourage global motion estimation
    aom_codec_control(&codec, AOME_SET_CPUUSED, 8);  // Balanced speed/quality
    aom_codec_control(&codec, AV1E_SET_ENABLE_CDEF, 1);
    aom_codec_control(&codec, AV1E_SET_ENABLE_TPL_MODEL, 0);
    
    // Set up reference frame management
    aom_codec_control(&codec, AV1E_SET_ENABLE_REF_FRAME_MVS, 1);
    aom_codec_control(&codec, AV1E_SET_ENABLE_ORDER_HINT, 0);
    
    // Allocate image using aom_img_alloc() to let library manage buffers and strides
    aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }
    
    // Access the planes allocated by the library
    uint8_t* y_plane = img->planes[0];
    uint8_t* u_plane = img->planes[1];
    uint8_t* v_plane = img->planes[2];
    
    // Generate and encode multiple frames
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Generate Y plane with targeted FAST-9 patterns
        // Use the stride provided by the library: img->stride[0] for Y plane
        generate_targeted_branch_pattern(y_plane, width, height, img->stride[0], 
                                         pattern_type + frame_idx, threshold_b, fdp);
        
        // Generate simple U and V planes (chroma doesn't affect FAST-9 much)
        // Use the stride provided by the library: img->stride[1] for U plane, img->stride[2] for V plane
        size_t uv_width = (width + 1) / 2;  // Handle odd dimensions
        size_t uv_height = (height + 1) / 2; // Handle odd dimensions
        for (size_t y = 0; y < uv_height; y++) {
            for (size_t x = 0; x < uv_width; x++) {
                u_plane[y * img->stride[1] + x] = fdp.ConsumeIntegralInRange<uint8_t>(64, 192);
                v_plane[y * img->stride[2] + x] = fdp.ConsumeIntegralInRange<uint8_t>(64, 192);
            }
        }
        
        // Encode the frame
        aom_codec_encode(&codec, img, frame_idx, 1, 0);
        
        // Check for encoding errors but don't exit - continue to test more patterns
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t* pkt;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just consume the packets, don't process them
        }
        
        // If we're running out of input, break early
        if (fdp.remaining_bytes() < 100) {
            break;
        }
    }
    
    // Flush encoder
    aom_codec_encode(&codec, NULL, -1, 1, 0);
    
    // Clean up
    aom_img_free(img);
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_012
