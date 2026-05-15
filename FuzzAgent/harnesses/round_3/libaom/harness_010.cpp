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

namespace harness_010 {
/*
 * Fuzzing harness for libaom AV1 encoder global motion estimation with advanced FAST-9 corner detection
 * Specifically targets aom_fast9_detect with 686 blocked branches and the complex nested if-else logic
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_010_guidance.md
 * Semantic differentiation: harness_009 focused on varied image patterns; 
 * harness_010 focuses on the circular pixel offset relationships and threshold binary search logic
 * in the FAST-9 algorithm to explore deeply nested conditional branches.
 */



// Helper function to create patterns specifically targeting FAST-9 circular pixel relationships
// The 16 circular pixels around each point are at specific offsets (make_offsets function)
// We need to create patterns that trigger specific combinations of these 16 pixels
static void generate_fast9_circular_pattern(uint8_t* buffer, int width, int height, int stride,
                                            int pattern_type, int brightness_threshold,
                                            FuzzedDataProvider& fdp) {
    // Clear buffer to mid-gray baseline
    memset(buffer, 128, stride * height);
    
    // Get parameters for pattern generation
    int base_brightness = fdp.ConsumeIntegralInRange<int>(50, 200);
    int contrast_range = fdp.ConsumeIntegralInRange<int>(20, 100);
    int pattern_size = fdp.ConsumeIntegralInRange<int>(4, 32);
    
    switch (pattern_type % 6) {
        case 0: // Pattern targeting specific pixel combinations (pixel[0]-pixel[15])
            // Create patterns that vary intensity relationships for specific circular positions
            // Based on make_offsets: pixel[0]=0+stride*3, pixel[4]=3+stride*0, pixel[8]=0+stride*-3, etc.
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Calculate position relative to pattern grid
                    int grid_x = x % pattern_size;
                    int grid_y = y % pattern_size;
                    
                    // Create different intensity zones for different circular positions
                    int intensity = base_brightness;
                    
                    // Zone for pixel[0] (right, 3 pixels down) - vertical relationships
                    if (grid_y > pattern_size * 2 / 3) {
                        intensity += contrast_range;
                    }
                    
                    // Zone for pixel[4] (right 3 pixels) - horizontal relationships  
                    if (grid_x > pattern_size * 2 / 3) {
                        intensity -= contrast_range / 2;
                    }
                    
                    // Zone for pixel[8] (left, 3 pixels up) - diagonal relationships
                    if (grid_x < pattern_size / 3 && grid_y < pattern_size / 3) {
                        intensity += contrast_range * 3 / 4;
                    }
                    
                    // Zone for pixel[12] (left 3 pixels) - opposite horizontal
                    if (grid_x < pattern_size / 3) {
                        intensity -= contrast_range;
                    }
                    
                    // Add threshold-based variations to trigger different comparisons
                    if ((grid_x + grid_y) % 7 == 0) {
                        intensity += brightness_threshold % 50;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
            
        case 1: // Pattern for testing threshold binary search logic (fast9_corner_score)
            // Create regions with precise intensity differences to test the bmin/bmax search
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create gradient with precise thresholds
                    int gradient = (x * 200) / std::max(1, width);
                    
                    // Add regions with specific intensity relationships
                    int region_x = x / 8;
                    int region_y = y / 8;
                    
                    int base = base_brightness;
                    
                    // Create different threshold conditions in different regions
                    switch ((region_x + region_y) % 4) {
                        case 0:
                            // Region where many pixels > center + b
                            base = 180 + (gradient % 40);
                            break;
                        case 1:
                            // Region where many pixels < center - b  
                            base = 60 + (gradient % 40);
                            break;
                        case 2:
                            // Region with mixed relationships
                            base = 120 + ((x + y) % 80) - 40;
                            break;
                        case 3:
                            // Region with near-threshold values
                            base = brightness_threshold + ((x + y) % 20) - 10;
                            break;
                    }
                    
                    // Add corner-like points at intersections
                    if ((x % 16 == 8) && (y % 16 == 8)) {
                        base = fdp.ConsumeIntegralInRange<int>(200, 250);
                    } else if ((x % 16 == 0) && (y % 16 == 0)) {
                        base = fdp.ConsumeIntegralInRange<int>(20, 60);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, base));
                }
            }
            break;
            
        case 2: // Pattern targeting border conditions (y=3 to ysize-3, x=3 to xsize-3)
            // FAST-9 skips border pixels, test edge cases near borders
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int intensity = 128;
                    
                    // Create special patterns near borders (y=2,3,4 and y=ysize-4,ysize-3,ysize-2)
                    bool near_border = (y <= 4) || (y >= height - 5) || 
                                       (x <= 4) || (x >= width - 5);
                    
                    if (near_border) {
                        // Border region - create varying patterns
                        if ((y == 3 || y == height - 4) && (x == 3 || x == width - 4)) {
                            // Corner of valid region - potential corner point
                            intensity = fdp.ConsumeIntegralInRange<int>(200, 240);
                        } else if (y < 3 || y >= height - 3 || x < 3 || x >= width - 3) {
                            // Invalid region (skipped by FAST-9) - use random
                            intensity = fdp.ConsumeIntegralInRange<int>(0, 255);
                        } else {
                            // Near border but valid - gradient
                            intensity = 100 + ((x + y) % 100);
                        }
                    } else {
                        // Interior - create patterns for corner detection
                        int grid_x = x / 6;
                        int grid_y = y / 6;
                        
                        // Create checkerboard with intensity variations
                        if ((grid_x + grid_y) % 2 == 0) {
                            intensity = base_brightness + contrast_range;
                        } else {
                            intensity = base_brightness - contrast_range;
                        }
                        
                        // Add isolated bright/dark points
                        if ((x % 12 == 6) && (y % 12 == 6)) {
                            intensity = 240;
                        } else if ((x % 12 == 0) && (y % 12 == 0)) {
                            intensity = 30;
                        }
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
        case 3: { // Pattern for testing the complex nested if-else decision tree
            // Create patterns that will take different paths through the decision tree
            // Use fuzzer input to determine intensity relationships for different pixel positions
            int pixel_intensities[16];
            for (int i = 0; i < 16 && fdp.remaining_bytes() > 0; i++) {
                pixel_intensities[i] = fdp.ConsumeIntegralInRange<int>(30, 180);
            }
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Use position to determine which branch to test
                    int branch_selector = (x * 7 + y * 11) % 16;
                    int intensity = 128;
                    
                    // Different intensity relationships for different branches
                    // Use fuzzer-derived values instead of hardcoded constants
                    if (fdp.remaining_bytes() > 0) {
                        // Use the pre-consumed pixel intensities
                        intensity = pixel_intensities[branch_selector];
                    } else {
                        // Fallback if we ran out of fuzzer input
                        intensity = 128 - (branch_selector * 10);
                    }
                    
                    // Add some noise from fuzzer input
                    if (fdp.remaining_bytes() > 0) {
                        intensity += fdp.ConsumeIntegralInRange<int>(-20, 20);
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
        }
            
        case 4: { // Pattern for testing both >cb and <c_b conditions
            // Create regions with pixels both brighter and darker than center
            // Consume intensity parameters from fuzzer input
            int bright_base = fdp.ConsumeIntegralInRange<int>(150, 210);
            int dark_base = fdp.ConsumeIntegralInRange<int>(40, 90);
            int mixed_variation = fdp.ConsumeIntegralInRange<int>(20, 50);
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int intensity = 128;
                    
                    // Create alternating regions for >cb and <c_b testing
                    int region = (x / 4 + y / 4) % 4;
                    
                    switch (region) {
                        case 0:
                            // Region where pixels are mostly > center + threshold
                            intensity = bright_base + ((x + y) % 40);
                            break;
                        case 1:
                            // Region where pixels are mostly < center - threshold
                            intensity = dark_base + ((x + y) % 40);
                            break;
                        case 2:
                            // Mixed region - some >cb, some <c_b
                            intensity = 128 + ((x % 3 - 1) * mixed_variation) + ((y % 3 - 1) * mixed_variation);
                            break;
                        case 3:
                            // Near-threshold region
                            intensity = brightness_threshold + ((x + y) % 30) - 15;
                            break;
                    }
                    
                    // Add specific corner patterns using fuzzer input
                    if ((x % 8 == 4) && (y % 8 == 4)) {
                        int bright_corner = fdp.ConsumeIntegralInRange<int>(200, 240);
                        intensity = bright_corner; // Bright corner candidate
                    } else if ((x % 8 == 0) && (y % 8 == 0)) {
                        int dark_corner = fdp.ConsumeIntegralInRange<int>(20, 60);
                        intensity = dark_corner;  // Dark corner candidate
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, intensity));
                }
            }
            break;
        }
            
        case 5: // Random pattern with controlled corner distribution
            // Use fuzzer input to generate random but corner-rich patterns
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Base random value
                    int intensity;
                    
                    if (fdp.remaining_bytes() > 0) {
                        intensity = fdp.ConsumeIntegralInRange<int>(0, 255);
                    } else {
                        intensity = 128 + ((x * 37 + y * 41) % 129) - 64;
                    }
                    
                    // Increase likelihood of corners at certain positions
                    if ((x % 7 == 3) && (y % 7 == 3)) {
                        // Make these positions more likely to be corners
                        if (intensity > 128) {
                            intensity = std::min(255, intensity + 50);
                        } else {
                            intensity = std::max(0, intensity - 50);
                        }
                    }
                    
                    buffer[y * stride + x] = (uint8_t)intensity;
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Image dimensions and pattern parameters: ~30 bytes
    // - Encoder configuration: ~50 bytes
    // - Multiple frames worth of pattern data: ~100 bytes
    // Total minimum: ~180 bytes
    if (size < 180) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume image dimensions specifically for FAST-9 testing
    // FAST-9 requires border of 3 pixels, so minimum size is 7x7
    // Use varied sizes to test different execution paths
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 128);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 128);
    
    // Test edge cases: very small images, odd dimensions, etc.
    if (fdp.ConsumeBool()) {
        width = fdp.ConsumeIntegralInRange<unsigned int>(7, 32);
        height = fdp.ConsumeIntegralInRange<unsigned int>(7, 32);
    }
    
    // Ensure minimum size for FAST-9 algorithm
    if (width < 7) width = 7;
    if (height < 7) height = 7;
    
    // Step 2: Create test image (I420 format)
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Step 3: Fill image with FAST-9 optimized circular patterns
    // Consume pattern parameters from fuzzer input
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    int brightness_threshold = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Fill Y plane (luma) with FAST-9 circular patterns
    if (img->stride[0] > 0 && img->planes[0] != nullptr) {
        generate_fast9_circular_pattern(img->planes[0], width, height, img->stride[0],
                                        pattern_type, brightness_threshold, fdp);
    }
    
    // Fill U and V planes (chroma) with mid-gray
    // Chroma doesn't significantly affect FAST-9 corner detection (luma-based)
    for (int plane = 1; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 1) ? ((height + 1) / 2) : ((height + 1) / 2);
            size_t plane_width = (plane == 1) ? ((width + 1) / 2) : ((width + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            memset(img->planes[plane], 128, plane_size);
        }
    }
    
    aom_image_t* wrapped_img = img;
    
    // Step 4: Initialize encoder context for global motion estimation
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
    
    // Step 5: Configure encoder specifically for global motion estimation
    // Set image dimensions
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Configure settings that affect motion estimation quality
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

    // Step 6: Set controls specifically for global motion estimation
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
    
    // Set threshold parameter that affects FAST-9 detection
    // Note: The actual threshold 'b' is internal, but we can set related controls
    if (fdp.ConsumeBool()) {
        int motion_search_param = fdp.ConsumeIntegralInRange<int>(0, 10);
        aom_codec_control(&codec, AOME_SET_ENABLEAUTOALTREF, motion_search_param);
    }
    
    // Step 7: Encode multiple frames with varying patterns
    // This triggers: global motion estimation -> corner detection -> aom_fast9_detect
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 6);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // For subsequent frames, modify patterns to create motion
        // This helps exercise global motion estimation across frames
        if (frame_idx > 0 && fdp.remaining_bytes() > 30) {
            // Change pattern or parameters to simulate motion
            pattern_type = (pattern_type + fdp.ConsumeIntegralInRange<int>(1, 3)) % 6;
            brightness_threshold = (brightness_threshold + fdp.ConsumeIntegralInRange<int>(10, 60)) % 255;
            
            // Regenerate image with new pattern
            if (img->stride[0] > 0 && img->planes[0] != nullptr) {
                generate_fast9_circular_pattern(img->planes[0], width, height, img->stride[0],
                                                pattern_type, brightness_threshold, fdp);
            }
        }
        
        // Encode the frame - this triggers the full pipeline
        // The encoder will call global motion estimation which uses FAST-9
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
        if (fdp.remaining_bytes() < 40) {
            break;
        }
    }
    
    // Step 8: Flush encoder to process any remaining frames
    res = aom_codec_encode(&codec, nullptr, -1, 1, 0);
    
    // Step 9: Cleanup
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_010
