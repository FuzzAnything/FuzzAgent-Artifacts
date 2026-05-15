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

namespace harness_009 {
/*
 * Fuzzing harness for libaom AV1 encoder FAST-9 corner detection for global motion estimation
 * Specifically targets aom_fast9_detect with 686 blocked branches and fast9_corner_score with 478 blocked branches
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_009_guidance.md
 * Semantic differentiation: harness_007 targeted global motion estimation with basic corner patterns;
 * harness_009 focuses specifically on varied image patterns to exercise deeply nested conditional logic 
 * in aom_fast9_detect and fast9_corner_score.
 */



// Helper function to generate FAST-9 specific corner patterns
// The FAST-9 algorithm examines 16 pixels in a circular pattern around each point
// Pixel indices 0-15 correspond to positions on a circle of radius 3
static void generate_fast9_pattern(uint8_t* buffer, int width, int height, int stride,
                                   int pattern_type, int brightness_threshold, 
                                   FuzzedDataProvider& fdp) {
    // Clear buffer to mid-gray baseline
    memset(buffer, 128, stride * height);
    
    // FAST-9 circular pattern indices (approximate relative positions):
    // Indices roughly correspond to: 0=right, 4=up, 8=left, 12=down, with others in between
    // We need to create patterns that trigger specific combinations of these 16 pixels
    switch (pattern_type % 8) {
        case 0: // Pattern for "many pixels > center+b" (triggers early exit in detection)
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create bright clusters that satisfy many pixels > center+b
                    if ((x % 8 >= 2 && x % 8 <= 6) && (y % 8 >= 2 && y % 8 <= 6)) {
                        // Bright region that should trigger corner detection
                        int bright_variation = fdp.ConsumeIntegralInRange<int>(0, 40);
                        buffer[y * stride + x] = 200 + bright_variation;
                    } else {
                        // Dark background
                        int dark_variation = fdp.ConsumeIntegralInRange<int>(0, 20);
                        buffer[y * stride + x] = 60 + dark_variation;
                    }
                }
            }
            break;
            
        case 1: // Pattern for "many pixels < center-b" (opposite intensity relationship)
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create dark clusters surrounded by bright areas
                    if ((x % 12 == 0) || (y % 12 == 0)) {
                        // Dark lines/corners
                        int dark_variation = fdp.ConsumeIntegralInRange<int>(0, 30);
                        buffer[y * stride + x] = 30 + dark_variation;
                    } else {
                        // Bright background
                        int bright_variation = fdp.ConsumeIntegralInRange<int>(0, 40);
                        buffer[y * stride + x] = 180 + bright_variation;
                    }
                }
            }
            break;
            
        case 2: // Mixed pattern with alternating bright/dark in circular arrangement
            // This targets specific combinations in the nested if-else tree
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Create checkerboard pattern with varying intensities
                    int checker = ((x / 3) + (y / 3)) % 2;
                    
                    // Alternate between bright and dark with some noise
                    int bright_base = fdp.ConsumeIntegralInRange<int>(180, 220);
                    int dark_base = fdp.ConsumeIntegralInRange<int>(30, 80);
                    int base_value = checker ? bright_base : dark_base;
                    int noise = fdp.ConsumeIntegralInRange<int>(-20, 20);
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, base_value + noise));
                    
                    // Occasionally create corner-like patterns at grid intersections
                    if ((x % 9 == 4) && (y % 9 == 4)) {
                        // Potential corner point - make it stand out
                        if (checker) {
                            int bright_corner = fdp.ConsumeIntegralInRange<int>(230, 250);
                            buffer[y * stride + x] = (uint8_t)bright_corner;
                        } else {
                            int dark_corner = fdp.ConsumeIntegralInRange<int>(10, 40);
                            buffer[y * stride + x] = (uint8_t)dark_corner;
                        }
                    }
                }
            }
            break;
            
        case 3: // Pattern specifically for border conditions (x<3, y<3, x>=xsize-3, y>=ysize-3)
            // The FAST-9 detector skips border pixels (y=3 to ysize-3, x=3 to xsize-3)
            // Test edge cases by making border pixels different
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (y < 3 || y >= height - 3 || x < 3 || x >= width - 3) {
                        // Border region - use different intensity from fuzzer input
                        int border_intensity = fdp.ConsumeIntegralInRange<int>(0, 255);
                        buffer[y * stride + x] = (uint8_t)border_intensity;
                    } else {
                        // Interior - create gradient with potential corners
                        int gradient_x = fdp.ConsumeIntegralInRange<int>(0, 150);
                        int gradient_y = fdp.ConsumeIntegralInRange<int>(0, 100);
                        int gradient = (x * gradient_x / std::max(1, width)) + (y * gradient_y / std::max(1, height));
                        int base_intensity = fdp.ConsumeIntegralInRange<int>(50, 200);
                        buffer[y * stride + x] = (uint8_t)(base_intensity + gradient % 150);
                    }
                }
            }
            break;
            
        case 4: // Pattern for testing score computation variations
            // Create regions with different contrast levels to test fast9_corner_score
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create regions with varying contrast
                    int region_x = x / 8;
                    int region_y = y / 8;
                    int region_id = region_x + region_y;
                    
                    int base = fdp.ConsumeIntegralInRange<int>(64, 192);
                    int amplitude = 30 + (region_id % 5) * 30; // Vary contrast
                    int frequency = 2 + (region_id % 3); // Vary pattern frequency
                    
                    int pattern_val = base + amplitude * sin((x + y) * M_PI / frequency / 8.0);
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, pattern_val));
                }
            }
            break;
            
        case 5: // Pattern targeting specific pixel combinations in the decision tree
            // The nested if-else examines specific subsets of the 16 pixels
            // Create pattern that varies which specific pixels are bright/dark
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Use modular arithmetic to create complex patterns
                    int val = 128;
                    
                    // Different conditions based on position mod 16
                    int x_mod = x % 16;
                    int y_mod = y % 16;
                    
                    // Create patterns that affect different subsets of the 16 circular pixels
                    int bright_intensity = fdp.ConsumeIntegralInRange<int>(160, 200);
                    int dark_intensity = fdp.ConsumeIntegralInRange<int>(60, 100);
                    
                    if ((x_mod < 8 && y_mod < 8)) {
                        val = bright_intensity; // Bright quadrant
                    } else if ((x_mod >= 8 && y_mod >= 8)) {
                        val = bright_intensity; // Another bright quadrant
                    } else {
                        val = dark_intensity; // Dark quadrants
                    }
                    
                    // Add corner points at intersections
                    if ((x_mod == 7 || x_mod == 8) && (y_mod == 7 || y_mod == 8)) {
                        val = fdp.ConsumeIntegralInRange<int>(230, 250); // Very bright potential corner
                    } else if ((x_mod == 0 || x_mod == 15) && (y_mod == 0 || y_mod == 15)) {
                        val = fdp.ConsumeIntegralInRange<int>(20, 60); // Very dark potential corner
                    }
                    
                    buffer[y * stride + x] = (uint8_t)val;
                }
            }
            break;
            
        case 6: // Random pattern with controlled statistics to explore parameter space
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Use fuzzer input to generate controlled random patterns
                    if (fdp.remaining_bytes() > 0) {
                        int rand_val = fdp.ConsumeIntegralInRange<int>(0, 255);
                        buffer[y * stride + x] = (uint8_t)rand_val;
                    } else {
                        // Fallback if we run out of fuzzer input
                        buffer[y * stride + x] = 128 + ((x + y) % 127) - 63;
                    }
                }
            }
            break;
            
        case 7: // Gradient with sharp transitions (edge detection cases)
            int bright_edge = fdp.ConsumeIntegralInRange<int>(200, 250);
            int dark_edge = fdp.ConsumeIntegralInRange<int>(10, 60);
            int edge_threshold = fdp.ConsumeIntegralInRange<int>(100, 150);
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int gradient = (x * 255) / std::max(1, width);
                    
                    // Add sharp transitions at specific intervals
                    if (abs(x - width/2) < 2 || abs(y - height/2) < 2) {
                        // Sharp edge - high contrast
                        buffer[y * stride + x] = (gradient > edge_threshold) ? (uint8_t)bright_edge : (uint8_t)dark_edge;
                    } else {
                        // Smooth gradient
                        buffer[y * stride + x] = (uint8_t)gradient;
                    }
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Encoder config parameters: ~50 bytes
    // - Image parameters: ~20 bytes  
    // - Pattern generation parameters: ~50 bytes
    // - Multiple frames: ~100 bytes
    // Total minimum: ~220 bytes
    if (size < 220) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume image dimensions for corner detection testing
    // Use varied sizes to test different border conditions in aom_fast9_detect
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 128);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 128);
    
    // Ensure minimum size for FAST-9 which requires 3-pixel border
    if (width < 16) width = 16;
    if (height < 16) height = 16;
    
    // Occasionally test very small images to exercise border condition logic
    if (fdp.ConsumeBool()) {
        width = fdp.ConsumeIntegralInRange<unsigned int>(8, 32);
        height = fdp.ConsumeIntegralInRange<unsigned int>(8, 32);
    }
    
    // Create test image (I420 format)
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Step 2: Fill image with FAST-9 specific corner patterns
    // Consume pattern parameters from fuzzer input
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 7);
    int brightness_threshold = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Fill Y plane (luma) with FAST-9 optimized patterns
    if (img->stride[0] > 0 && img->planes[0] != nullptr) {
        generate_fast9_pattern(img->planes[0], width, height, img->stride[0],
                               pattern_type, brightness_threshold, fdp);
    }
    
    // Fill U and V planes (chroma) with consistent values
    // Chroma doesn't significantly affect FAST-9 corner detection (luma-based)
    for (int plane = 1; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 1) ? ((height + 1) / 2) : ((height + 1) / 2);
            size_t plane_width = (plane == 1) ? ((width + 1) / 2) : ((width + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            // Use mid-gray for chroma
            memset(img->planes[plane], 128, plane_size);
        }
    }
    
    aom_image_t* wrapped_img = img;
    
    // Step 3: Initialize encoder context for global motion estimation
    aom_codec_ctx_t codec;
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    
    // Consume initialization flags
    aom_codec_flags_t flags = static_cast<aom_codec_flags_t>(
        fdp.ConsumeIntegral<uint32_t>());
    
    // Get default configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2); // AOM_USAGE values
    
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Failed to get default config
    }
    
    // Step 4: Configure encoder specifically for global motion estimation
    // Set image dimensions
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Configure bitrate/quality settings that affect motion estimation
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(20, 63);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 40);
    
    // Ensure min <= max
    if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) {
        std::swap(cfg.rc_min_quantizer, cfg.rc_max_quantizer);
    }
    
    // Settings that influence global motion estimation
    cfg.g_usage = usage;
    cfg.g_threads = 1; // Single thread for deterministic testing
    cfg.rc_end_usage = AOM_CBR;
    
    // Set lag-in-frames to enable inter-frame motion estimation
    // Global motion estimation works across frames
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, 25);
    
    // Initialize encoder with version check
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Encoder initialization failed
    }

    // Step 5: Set controls specifically for global motion estimation
    // Enable global motion estimation (critical for FAST-9 corner detection)
    int enable_global_motion = fdp.ConsumeBool() ? 1 : 0;
    aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
    
    // Set CPU usage which affects motion search quality
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);
    
    // Additional controls that might affect corner detection
    if (fdp.ConsumeBool()) {
        int enable_warped_motion = fdp.ConsumeBool() ? 1 : 0;
        aom_codec_control(&codec, AV1E_SET_ENABLE_WARPED_MOTION, enable_warped_motion);
    }
    
    // Step 6: Encode multiple frames with varying patterns to thoroughly exercise FAST-9
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // For subsequent frames, modify patterns to create motion
        // This helps exercise global motion estimation across frames
        if (frame_idx > 0 && fdp.remaining_bytes() > 20) {
            // Change pattern or parameters slightly to simulate motion
            pattern_type = (pattern_type + fdp.ConsumeIntegralInRange<int>(1, 3)) % 8;
            brightness_threshold = (brightness_threshold + 40) % 255;
            
            // Regenerate image with new pattern
            if (img->stride[0] > 0 && img->planes[0] != nullptr) {
                generate_fast9_pattern(img->planes[0], width, height, img->stride[0],
                                       pattern_type, brightness_threshold, fdp);
            }
        }
        
        // Encode the frame - this triggers the full pipeline including:
        // 1. Global motion estimation facade
        // 2. Corner list computation  
        // 3. aom_fast9_detect_nonmax -> aom_fast9_detect
        // 4. fast9_corner_score for scoring
        res = aom_codec_encode(&codec, wrapped_img, frame_idx, 1, 0);
        
        // Even if encoding fails, the corner detection may have been attempted
        // Don't abort on encoding errors
        
        // Get compressed data to complete the encoding process
        const aom_codec_cx_pkt_t *pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packets if needed
        }
        
        // Break early if we're running low on fuzzer input
        if (fdp.remaining_bytes() < 50) {
            break;
        }
    }
    
    // Step 7: Flush encoder to process any remaining frames
    res = aom_codec_encode(&codec, nullptr, -1, 1, 0);
    
    // Step 8: Cleanup
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_009
