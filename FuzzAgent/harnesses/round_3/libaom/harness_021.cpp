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

namespace harness_021 {
/*
 * Fuzzing harness for libaom AV1 encoder global motion estimation with edge-based FAST corner detection
 * Specifically targets aom_fast9_detect with 1039 blocked branches, focusing on edge and border patterns
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_021_guidance.md
 * 
 * Critical analysis from guidance:
 * - Top blocker: aom_fast9_detect with 1039 blocked branches out of 1974 reachable branches
 * - Function is part of FAST corner detection algorithm used in global motion estimation
 * - Called during global motion estimation for feature matching
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_007: basic corner patterns for global motion estimation
 * - harness_009: varied image patterns for FAST-9
 * - harness_020: SPECIALIZED DARK CORNER PATTERNS targeting p[pixel[n]] < c_b conditions
 * - harness_021: EDGE AND BORDER PATTERNS targeting different code paths in aom_fast9_detect
 * 
 * Strategy: Create images with strong edge patterns, border conditions, and varying contrast
 * to exercise different decision paths in the FAST-9 algorithm, particularly focusing on
 * border pixel handling and edge-based corner detection.
 */



// Helper function to generate edge-based patterns for FAST corner detection
// Targeting border conditions and edge transitions that trigger different code paths
static void generate_edge_pattern(uint8_t* buffer, int width, int height, int stride,
                                 int pattern_type, int edge_threshold,
                                 FuzzedDataProvider& fdp) {
    // Clear buffer to create baseline
    memset(buffer, 128, stride * height);
    
    // Consume pattern parameters from fuzzer input
    int edge_width = fdp.ConsumeIntegralInRange<int>(1, 8);
    int edge_intensity = fdp.ConsumeIntegralInRange<int>(50, 200);
    int background_intensity = fdp.ConsumeIntegralInRange<int>(80, 180);
    int noise_level = fdp.ConsumeIntegralInRange<int>(0, 40);
    
    switch (pattern_type % 6) {
        case 0: { // Vertical edges pattern
            // Create strong vertical edges at regular intervals
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int edge_pos = x % (edge_width * 4);
                    bool is_edge = (edge_pos < edge_width) || (edge_pos >= edge_width * 3);
                    
                    int intensity = is_edge ? edge_intensity : background_intensity;
                    
                    // Add noise for variation
                    int noise = fdp.ConsumeIntegralInRange<int>(-noise_level, noise_level);
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity + noise));
                }
            }
            break;
        }
            
        case 1: { // Horizontal edges pattern  
            // Create strong horizontal edges
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int edge_pos = y % (edge_width * 4);
                    bool is_edge = (edge_pos < edge_width) || (edge_pos >= edge_width * 3);
                    
                    int intensity = is_edge ? edge_intensity : background_intensity;
                    
                    // Add noise for variation
                    int noise = fdp.ConsumeIntegralInRange<int>(-noise_level, noise_level);
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity + noise));
                }
            }
            break;
        }
            
        case 2: { // Checkerboard edges (strong corner patterns)
            // Creates grid of potential corners where edges intersect
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create checkerboard pattern with sharp transitions
                    bool is_white = ((x / edge_width) + (y / edge_width)) % 2 == 0;
                    
                    int intensity = is_white ? edge_intensity + 40 : background_intensity - 40;
                    
                    // Make edges sharper at checkerboard boundaries
                    if ((x % edge_width == 0) || (y % edge_width == 0)) {
                        intensity = edge_intensity;
                    }
                    
                    // Corner points (intersections) get special treatment
                    if ((x % edge_width == 0) && (y % edge_width == 0)) {
                        intensity = fdp.ConsumeIntegralInRange<int>(230, 250);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
        }
            
        case 3: { // Border-focused pattern (targeting x<3, y<3, x>=width-3, y>=height-3)
            // The FAST-9 detector skips border pixels (y=3 to ysize-3, x=3 to xsize-3)
            // Test edge cases by making border regions different from interior
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    bool is_border = (y < 3) || (y >= height - 3) || 
                                    (x < 3) || (x >= width - 3);
                    
                    int intensity;
                    if (is_border) {
                        // Border region - create varying patterns
                        if (y < 3) intensity = fdp.ConsumeIntegralInRange<int>(200, 250); // Top border bright
                        else if (y >= height - 3) intensity = fdp.ConsumeIntegralInRange<int>(30, 80); // Bottom border dark
                        else if (x < 3) intensity = fdp.ConsumeIntegralInRange<int>(150, 200); // Left border medium
                        else intensity = fdp.ConsumeIntegralInRange<int>(80, 130); // Right border medium-dark
                    } else {
                        // Interior - create gradient with potential edge corners
                        int gradient_x = (x * 150) / std::max(1, width);
                        int gradient_y = (y * 100) / std::max(1, height);
                        intensity = 100 + (gradient_x + gradient_y) / 2;
                        
                        // Add occasional bright spots at potential corner locations
                        if ((x % 8 == 4) && (y % 8 == 4)) {
                            intensity = fdp.ConsumeIntegralInRange<int>(220, 240);
                        }
                    }
                    
                    buffer[y * stride + x] = (uint8_t)intensity;
                }
            }
            break;
        }
            
        case 4: { // Gradient with sharp edge transitions
            // Create smooth gradients interrupted by sharp edges
            int edge_count = fdp.ConsumeIntegralInRange<int>(3, 10);
            int edge_spacing = width / (edge_count + 1);
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Base gradient
                    int gradient = (x * 200) / std::max(1, width);
                    
                    // Add sharp edges at intervals
                    for (int e = 1; e <= edge_count; e++) {
                        int edge_pos = e * edge_spacing;
                        if (abs(x - edge_pos) < 2) {
                            // Sharp edge - abrupt intensity change
                            gradient = (x < edge_pos) ? 
                                      fdp.ConsumeIntegralInRange<int>(80, 120) : 
                                      fdp.ConsumeIntegralInRange<int>(180, 220);
                            break;
                        }
                    }
                    
                    // Add vertical modulation
                    int vertical_mod = (y % 16) * 10;
                    int intensity = (gradient + vertical_mod) % 256;
                    
                    buffer[y * stride + x] = (uint8_t)intensity;
                }
            }
            break;
        }
            
        case 5: { // Random edge patterns with controlled statistics
            // Use fuzzer input to create random but structured edge patterns
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int intensity = 128;
                    
                    // Determine if this is an edge region based on position
                    int region_size = 8;
                    int region_x = x / region_size;
                    int region_y = y / region_size;
                    
                    // Use fuzzer data to determine region intensity
                    if (fdp.remaining_bytes() > region_x + region_y) {
                        // Consume different values for different regions
                        int region_val = fdp.ConsumeIntegralInRange<int>(0, 255);
                        
                        // Smooth transition between regions
                        int local_x = x % region_size;
                        int local_y = y % region_size;
                        
                        // Get neighboring region value
                        int next_region_val = fdp.ConsumeIntegralInRange<int>(0, 255);
                        
                        // Linear interpolation between regions
                        float blend_x = (float)local_x / region_size;
                        float blend_y = (float)local_y / region_size;
                        intensity = (int)(region_val * (1.0f - blend_x) + next_region_val * blend_x);
                        
                        // Create edge at region boundaries
                        if (local_x == 0 || local_y == 0) {
                            intensity = fdp.ConsumeIntegralInRange<int>(edge_intensity - 20, edge_intensity + 20);
                        }
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
        }
    }
}

// Create YUV image with edge patterns
static aom_image_t* create_edge_pattern_image(FuzzedDataProvider& fdp, int width, int height) {
    aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                     (unsigned int)width, (unsigned int)height, 16);
    if (!img) return nullptr;
    
    // Generate Y plane with edge patterns
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    int edge_threshold = fdp.ConsumeIntegralInRange<int>(10, 50);
    
    generate_edge_pattern(img->planes[AOM_PLANE_Y], width, height, 
                         img->stride[AOM_PLANE_Y], pattern_type, edge_threshold, fdp);
    
    // For U and V planes, create simpler patterns (chroma subsampling)
    int uv_width = (width + 1) / 2;
    int uv_height = (height + 1) / 2;
    
    // U plane
    uint8_t* u_plane = img->planes[AOM_PLANE_U];
    for (int y = 0; y < uv_height; y++) {
        for (int x = 0; x < uv_width; x++) {
            // Simple gradient for chroma
            int intensity = 128 + ((x * 50) / std::max(1, uv_width)) - 25;
            u_plane[y * img->stride[AOM_PLANE_U] + x] = (uint8_t)intensity;
        }
    }
    
    // V plane  
    uint8_t* v_plane = img->planes[AOM_PLANE_V];
    for (int y = 0; y < uv_height; y++) {
        for (int x = 0; x < uv_width; x++) {
            // Simple gradient for chroma
            int intensity = 128 + ((y * 50) / std::max(1, uv_height)) - 25;
            v_plane[y * img->stride[AOM_PLANE_V] + x] = (uint8_t)intensity;
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) return 0; // Minimum input size
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(64, 256);
    int height = fdp.ConsumeIntegralInRange<int>(64, 256);
    int bit_depth_choice = fdp.ConsumeIntegralInRange<int>(0, 2); // 0=8-bit, 1=10-bit, 2=12-bit
    
    // Initialize encoder
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Configure for global motion estimation
    cfg.g_w = width;
    cfg.g_h = height;
    // Set bit depth based on choice
    switch (bit_depth_choice) {
        case 0: cfg.g_bit_depth = AOM_BITS_8; break;
        case 1: cfg.g_bit_depth = AOM_BITS_10; break;
        case 2: cfg.g_bit_depth = AOM_BITS_12; break;
        default: cfg.g_bit_depth = AOM_BITS_8; break;
    }
    cfg.g_input_bit_depth = (cfg.g_bit_depth == AOM_BITS_8) ? 8 : 
                           (cfg.g_bit_depth == AOM_BITS_10) ? 10 : 12;
    
    // Set global motion estimation parameters
    // Note: The actual configuration for global motion is in internal structures
    // We'll rely on the encoder defaults and image patterns to trigger the feature
    
    // Set reasonable quality/speed tradeoff for fuzzing
    cfg.rc_end_usage = AOM_Q;
    cfg.g_profile = 0;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    
    // Limit frame count for performance
    cfg.g_limit = 3;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Create test image with edge patterns
    aom_image_t* img = create_edge_pattern_image(fdp, width, height);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }
    
    // Encode multiple frames to trigger global motion estimation between frames
    int frame_count = fdp.ConsumeIntegralInRange<int>(2, 4);
    for (int i = 0; i < frame_count; i++) {
        if (fdp.remaining_bytes() < 100) break; // Not enough data for another frame
        
        // For subsequent frames, optionally modify the image slightly
        // to create motion for global motion estimation
        if (i > 0) {
            // Apply slight modifications to create inter-frame motion
            int modify_type = fdp.ConsumeIntegralInRange<int>(0, 2);
            int modify_amount = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            // Simple modifications to create motion
            uint8_t* y_plane = img->planes[AOM_PLANE_Y];
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (modify_type == 0) {
                        // Brighten a region
                        if (x < width/2 && y < height/2) {
                            int new_val = y_plane[y * img->stride[AOM_PLANE_Y] + x] + modify_amount;
                            y_plane[y * img->stride[AOM_PLANE_Y] + x] = (uint8_t)std::min(255, new_val);
                        }
                    } else if (modify_type == 1) {
                        // Darken a region
                        if (x >= width/2 && y >= height/2) {
                            int new_val = y_plane[y * img->stride[AOM_PLANE_Y] + x] - modify_amount;
                            y_plane[y * img->stride[AOM_PLANE_Y] + x] = (uint8_t)std::max(0, new_val);
                        }
                    }
                }
            }
        }
        
        // Encode the frame
        aom_codec_encode(&codec, img, i, 1, 0);
        
        // Get encoded data (even if we don't use it, this exercises the codec)
        const aom_codec_cx_pkt_t* pkt = NULL;
        aom_codec_iter_t iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet if needed
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Frame data available
                // We don't need to do anything with it for fuzzing
            }
        }
    }
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_021
