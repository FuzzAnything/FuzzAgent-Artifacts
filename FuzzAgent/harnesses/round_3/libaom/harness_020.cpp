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

namespace harness_020 {
/*
 * Fuzzing harness for libaom AV1 encoder global motion estimation with specialized dark corner detection
 * Specifically targets 1039 blocked branches in aom_fast9_detect, focusing on COMPLETELY UNCOVERED dark corner paths
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_020_guidance.md
 * 
 * Critical analysis from guidance:
 * - Dark Corner Paths: COMPLETELY UNCOVERED (lines 3049-3068 with 0 hits)
 * - Blocked Pattern: Complex pixel comparison chains with 0 hits in "dark corner" detection paths (p[pixel[n]] < c_b conditions)
 * - Root Cause: Current fuzzing doesn't generate images with specific dark corner patterns
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_007: basic corner patterns for global motion estimation
 * - harness_009: varied image patterns for FAST-9
 * - harness_010: circular pixel offset relationships
 * - harness_011: systematic exploration of all 16 circular pixel combinations
 * - harness_012: targeted exploration of specific blocked branch patterns
 * - harness_020: SPECIALIZED DARK CORNER PATTERNS targeting p[pixel[n]] < c_b conditions (0% coverage paths)
 * 
 * Strategy: Create images where many surrounding pixels are significantly darker than center-b threshold
 * to trigger the deeply nested dark corner conditions in aom_fast9_detect.
 */



// Helper function to generate specialized DARK CORNER patterns for aom_fast9_detect
// Targeting the condition: p[pixel[n]] < c_b where c_b = *p - b
// This requires creating patterns where many surrounding pixels are DARKER than (center - threshold)
static void generate_dark_corner_pattern(uint8_t* buffer, int width, int height, int stride,
                                        int pattern_type, int threshold_b,
                                        FuzzedDataProvider& fdp) {
    // Clear buffer to create baseline
    memset(buffer, 128, stride * height);
    
    // Consume pattern parameters from fuzzer input
    int center_intensity = fdp.ConsumeIntegralInRange<int>(100, 200); // Relatively bright center
    int dark_offset = fdp.ConsumeIntegralInRange<int>(30, 100); // How much darker surrounding pixels should be
    int pattern_size = fdp.ConsumeIntegralInRange<int>(6, 24);
    
    // For dark corner detection, we need: p[pixel[n]] < (*p - b)
    // This means surrounding pixels must be DARKER than (center - threshold_b)
    // threshold_b is the parameter 'b' passed to aom_fast9_detect
    
    switch (pattern_type % 8) {
        case 0: // Pattern targeting DARK CORNER path at lines 3049-3068 (completely uncovered)
            // This path requires: p[pixel[8]] < c_b && p[pixel[9]] < c_b && p[pixel[10]] < c_b ...
            // Creating clusters where left-side circular pixels are very dark
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create bright centers with very dark left-side surroundings
                    int grid_x = (x / pattern_size) % 3;
                    int grid_y = (y / pattern_size) % 3;
                    
                    if (grid_x == 1 && grid_y == 1) {
                        // Bright center point
                        buffer[y * stride + x] = center_intensity;
                        
                        // Make LEFT-SIDE circular pixels VERY DARK (pixels 8-15 are on left side)
                        // pixel[8] = 0 + stride * -3 (straight up)
                        int target_y = y - 3;
                        if (target_y >= 0) {
                            buffer[target_y * stride + x] = center_intensity - dark_offset - 20;
                        }
                        
                        // pixel[9] = -1 + stride * -3 (up-left)
                        int target_x = x - 1;
                        target_y = y - 3;
                        if (target_x >= 0 && target_y >= 0) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset - 25;
                        }
                        
                        // pixel[10] = -2 + stride * -2 (up-left diagonal)
                        target_x = x - 2;
                        target_y = y - 2;
                        if (target_x >= 0 && target_y >= 0) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset - 30;
                        }
                        
                        // pixel[11] = -3 + stride * -1 (left-up)
                        target_x = x - 3;
                        target_y = y - 1;
                        if (target_x >= 0 && target_y >= 0) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset - 35;
                        }
                        
                        // pixel[12] = -3 + stride * 0 (straight left)
                        target_x = x - 3;
                        if (target_x >= 0) {
                            buffer[y * stride + target_x] = center_intensity - dark_offset - 40;
                        }
                        
                        // pixel[13] = -3 + stride * 1 (left-down)
                        target_x = x - 3;
                        target_y = y + 1;
                        if (target_x >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset - 35;
                        }
                        
                        // pixel[14] = -2 + stride * 2 (left-down diagonal)
                        target_x = x - 2;
                        target_y = y + 2;
                        if (target_x >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset - 30;
                        }
                        
                        // pixel[15] = -1 + stride * 3 (down-left)
                        target_x = x - 1;
                        target_y = y + 3;
                        if (target_x >= 0 && target_y < height) {
                            buffer[target_y * stride + target_x] = center_intensity - dark_offset - 25;
                        }
                    } else {
                        // Background - moderately dark
                        buffer[y * stride + x] = center_intensity - dark_offset / 2;
                    }
                }
            }
            break;
            
        case 1: // Pattern for mixed dark/right-side bright to trigger complex decision tree
            // Targeting path at lines 3078-3110: p[pixel[6]] < c_b && p[pixel[15]] > cb ...
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    int region = (x / 8 + y / 8) % 4;
                    
                    switch (region) {
                        case 0: { // Region where pixel[6] is dark and pixel[15] is bright
                            buffer[y * stride + x] = center_intensity; // Center
                            
                            // pixel[6] = 2 + stride * -2 (should be dark: < center-b)
                            int px6_x = x + 2;
                            int px6_y = y - 2;
                            if (px6_x < width && px6_y >= 0) {
                                buffer[px6_y * stride + px6_x] = center_intensity - dark_offset - 40;
                            }
                            
                            // pixel[15] = -1 + stride * 3 (should be bright: > center+b)  
                            int px15_x = x - 1;
                            int px15_y = y + 3;
                            if (px15_x >= 0 && px15_y < height) {
                                buffer[px15_y * stride + px15_x] = center_intensity + 40;
                            }
                            break;
                        }
                            
                        case 1: { // Region where pixel[7] is dark (triggers different path)
                            buffer[y * stride + x] = center_intensity;
                            
                            // pixel[7] = 1 + stride * -3 (dark)
                            int px7_x = x + 1;
                            int px7_y = y - 3;
                            if (px7_x < width && px7_y >= 0) {
                                buffer[px7_y * stride + px7_x] = center_intensity - dark_offset - 50;
                            }
                            break;
                        }
                            
                        default:
                            buffer[y * stride + x] = center_intensity - 20;
                            break;
                    }
                }
            }
            break;
            
        case 2: // Gradient-based pattern with precise threshold relationships
            // Create gradients where specific circular positions cross the threshold boundary
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create vertical gradient
                    int vertical_gradient = (y * 150) / std::max(1, height);
                    
                    // Create horizontal modulation
                    int horizontal_mod = (x % 16) * 10;
                    
                    // Base intensity with gradient
                    int intensity = 80 + vertical_gradient;
                    
                    // Add dark spots at specific circular offset positions
                    // These will create p[pixel[n]] < c_b conditions when center is at certain positions
                    if ((x % 8 == 0) && (y % 8 == 0)) {
                        // Grid points - make these potential centers with dark surroundings
                        intensity = center_intensity;
                    } else if (((x + 3) % 8 == 0) && (y % 8 == 0)) {
                        // Position that would be pixel[4] relative to a center
                        intensity = center_intensity - dark_offset - 30;
                    } else if ((x % 8 == 0) && ((y + 3) % 8 == 0)) {
                        // Position that would be pixel[0] relative to a center
                        intensity = center_intensity - dark_offset - 35;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity + horizontal_mod));
                }
            }
            break;
            
        case 3: // Random noise with structured dark clusters
            // Create random background with deliberate dark clusters at circular offset positions
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Random base
                    int noise = fdp.ConsumeIntegralInRange<int>(-20, 20);
                    int intensity = 128 + noise;
                    
                    // Create dark clusters at positions that correspond to circular offsets
                    // from potential center points on a 8x8 grid
                    int grid_x = x % 8;
                    int grid_y = y % 8;
                    
                    // These are positions relative to a center at (0,0) in the 8x8 block
                    // We want certain offset positions to be consistently dark
                    if ((grid_x == 0 && grid_y == 3) ||  // pixel[0] position
                        (grid_x == 3 && grid_y == 0) ||  // pixel[4] position
                        (grid_x == 0 && grid_y == 5) ||  // pixel[8] position (actually -3 in y)
                        (grid_x == 5 && grid_y == 0)) {  // pixel[12] position
                        intensity = 50 + fdp.ConsumeIntegralInRange<int>(-10, 10); // Very dark
                    }
                    
                    // Make center points (every 8 pixels) bright
                    if (grid_x == 0 && grid_y == 0) {
                        intensity = center_intensity + fdp.ConsumeIntegralInRange<int>(-10, 10);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        case 4: // Concentric circles with alternating brightness for circular pattern testing
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Distance from nearest center point on 16x16 grid
                    int center_x = ((x + 8) / 16) * 16;
                    int center_y = ((y + 8) / 16) * 16;
                    
                    double dx = x - center_x;
                    double dy = y - center_y;
                    double distance = sqrt(dx*dx + dy*dy);
                    
                    int intensity;
                    if (distance < 1.5) {
                        // Center - bright
                        intensity = center_intensity;
                    } else if (distance >= 2.5 && distance < 3.5) {
                        // Circle at radius ~3 pixels (matching FAST-9 circular pattern)
                        // Make this circle DARK to trigger p[pixel[n]] < c_b
                        intensity = center_intensity - dark_offset - 40;
                    } else if (distance >= 1.5 && distance < 2.5) {
                        // Intermediate ring - medium brightness
                        intensity = center_intensity - 20;
                    } else {
                        // Background
                        intensity = center_intensity - 40;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        default: // Additional dark corner variations
            // Simple grid of bright points with dark immediate surroundings
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    bool is_center = (x % pattern_size == pattern_size/2) && 
                                    (y % pattern_size == pattern_size/2);
                    bool is_dark_ring = (abs(x % pattern_size - pattern_size/2) <= 3) &&
                                       (abs(y % pattern_size - pattern_size/2) <= 3) &&
                                       !is_center;
                    
                    if (is_center) {
                        buffer[y * stride + x] = center_intensity;
                    } else if (is_dark_ring) {
                        buffer[y * stride + x] = center_intensity - dark_offset - 
                                               fdp.ConsumeIntegralInRange<int>(0, 30);
                    } else {
                        buffer[y * stride + x] = center_intensity - 30;
                    }
                }
            }
            break;
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need: pattern_type (1 byte) + threshold_b (1 byte) + image params + some image data
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 7);
    int threshold_b = fdp.ConsumeIntegralInRange<int>(1, 50); // FAST-9 threshold parameter
    
    // Image dimensions - keep small for efficiency but large enough for FAST-9 (needs border of 3)
    int width = fdp.ConsumeIntegralInRange<int>(32, 128);
    int height = fdp.ConsumeIntegralInRange<int>(32, 128);
    
    // Initialize encoder
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // Encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Enable global motion estimation (triggers aom_fast9_detect)
    cfg.rc_end_usage = AOM_CBR;
    aom_codec_ctx_t encoder;
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Configure global motion estimation parameters
    // This influences when aom_fast9_detect is called
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 5); // Faster encoding
    aom_codec_control(&encoder, AV1E_SET_ENABLE_GLOBAL_MOTION, 1);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_OBMC, 0);
    
    // Create test image with DARK CORNER pattern
    aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 16);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Generate dark corner pattern in Y plane (luminance)
    // aom_fast9_detect operates on single-channel (luma) images
    generate_dark_corner_pattern(img->planes[AOM_PLANE_Y], width, height, img->stride[AOM_PLANE_Y],
                                 pattern_type, threshold_b, fdp);
    
    // Fill U and V planes with neutral values (128 for 8-bit)
    for (int y = 0; y < height / 2; y++) {
        memset(img->planes[AOM_PLANE_U] + y * img->stride[AOM_PLANE_U], 128, width / 2);
        memset(img->planes[AOM_PLANE_V] + y * img->stride[AOM_PLANE_V], 128, width / 2);
    }
    
    // Encode the frame - this should trigger global motion estimation and aom_fast9_detect
    res = aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // Get compressed data (optional - we're mainly interested in the encoding process)
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        // Process packets if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame data available
        }
    }
    
    // Clean up
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    
    return 0;
}

} // namespace harness_020
