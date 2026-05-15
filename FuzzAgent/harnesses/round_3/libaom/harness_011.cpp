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

namespace harness_011 {
/*
 * Fuzzing harness for libaom AV1 encoder global motion estimation with advanced FAST-9 corner detection
 * Specifically targets aom_fast9_detect with 875 blocked branches - focusing on precise circular pixel offset relationships
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_011_guidance.md
 * Semantic differentiation: harness_009 focused on varied image patterns; harness_010 focused on circular pixel relationships;
 * harness_011 focuses specifically on systematic exploration of all 16 circular pixel combinations from make_offsets()
 * to comprehensively exercise the deeply nested if-else decision tree in aom_fast9_detect.
 */



// Helper function to generate patterns based on exact circular pixel offsets from make_offsets()
// Pixel[0] = 0 + stride * 3    (right, 3 pixels down)
// Pixel[1] = 1 + stride * 3    (right 1, down 3)
// Pixel[2] = 2 + stride * 2    (right 2, down 2)
// Pixel[3] = 3 + stride * 1    (right 3, down 1)
// Pixel[4] = 3 + stride * 0    (right 3)
// Pixel[5] = 3 + stride * -1   (right 3, up 1)
// Pixel[6] = 2 + stride * -2   (right 2, up 2)
// Pixel[7] = 1 + stride * -3   (right 1, up 3)
// Pixel[8] = 0 + stride * -3   (left, 3 pixels up)
// Pixel[9] = -1 + stride * -3  (left 1, up 3)
// Pixel[10] = -2 + stride * -2 (left 2, up 2)
// Pixel[11] = -3 + stride * -1 (left 3, up 1)
// Pixel[12] = -3 + stride * 0  (left 3)
// Pixel[13] = -3 + stride * 1  (left 3, down 1)
// Pixel[14] = -2 + stride * 2  (left 2, down 2)
// Pixel[15] = -1 + stride * 3  (left 1, down 3)

static void generate_precise_circular_pattern(uint8_t* buffer, int width, int height, int stride,
                                              int pattern_type, int threshold_b,
                                              FuzzedDataProvider& fdp) {
    // Clear buffer to mid-gray baseline
    memset(buffer, 128, stride * height);
    
    // Consume pattern-specific parameters
    int center_intensity = fdp.ConsumeIntegralInRange<int>(50, 200);
    int bright_offset = fdp.ConsumeIntegralInRange<int>(20, 100);
    int dark_offset = fdp.ConsumeIntegralInRange<int>(20, 100);
    
    // The decision tree in aom_fast9_detect checks if pixels are > center+b or < center-b
    // We need to create patterns that systematically test all combinations
    switch (pattern_type % 10) {
        case 0: { // Pattern targeting specific early exit conditions (many pixels > center+b)
            // Create clusters where 9+ pixels are bright relative to center
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    // Position determines which circular pixels are bright
                    int pos_mod = (x + y) % 17; // 16 pixels + center
                    
                    if (pos_mod < 9) {
                        // Many bright pixels case - should trigger early is_a_corner
                        buffer[y * stride + x] = center_intensity + bright_offset;
                        
                        // Make specific circular positions bright based on pattern
                        for (int i = 0; i < 9; i++) {
                            int pixel_idx = (pos_mod + i) % 16;
                            int dx = 0, dy = 0;
                            
                            // Map pixel index to offset (simplified mapping)
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
                                buffer[target_y * stride + target_x] = center_intensity + bright_offset + 20;
                            }
                        }
                    } else {
                        buffer[y * stride + x] = center_intensity;
                    }
                }
            }
            break;
        }
            
        case 1: { // Pattern targeting opposite condition (many pixels < center-b)
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    int pos_mod = (x + y) % 17;
                    
                    if (pos_mod < 9) {
                        // Many dark pixels case
                        buffer[y * stride + x] = center_intensity;
                        
                        // Make specific circular positions dark
                        for (int i = 0; i < 9; i++) {
                            int pixel_idx = (pos_mod + i) % 16;
                            int dx = 0, dy = 0;
                            
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
                                buffer[target_y * stride + target_x] = center_intensity - dark_offset - 20;
                            }
                        }
                    } else {
                        buffer[y * stride + x] = center_intensity;
                    }
                }
            }
            break;
        }
            
        case 2: { // Pattern targeting mixed conditions (some bright, some dark)
            // This should exercise the complex nested if-else branches
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    int pattern_id = (x / 4) % 4 + ((y / 4) % 4) * 4;
                    
                    // Center intensity
                    buffer[y * stride + x] = center_intensity;
                    
                    // Create specific patterns for each 4x4 block
                    switch (pattern_id % 8) {
                        case 0: // pixels[0-7] bright, [8-15] dark
                            for (int i = 0; i < 8; i++) {
                                int dx = (i < 4) ? 3 - i : 8 - i;
                                int dy = (i < 4) ? 3 : -3;
                                int target_x = x + dx;
                                int target_y = y + dy;
                                if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                    buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                                }
                            }
                            break;
                            
                        case 1: // pixels[8-15] bright, [0-7] dark
                            for (int i = 8; i < 16; i++) {
                                int dx = (i < 12) ? -3 : -10 + i;
                                int dy = (i < 12) ? -3 : 3;
                                int target_x = x + dx;
                                int target_y = y + dy;
                                if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                    buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                                }
                            }
                            break;
                            
                        case 2: // Alternate bright/dark in circular pattern
                            for (int i = 0; i < 16; i++) {
                                int dx, dy;
                                if (i < 8) {
                                    dx = (i < 4) ? 3 - i : 8 - i;
                                    dy = (i < 4) ? 3 : -3;
                                } else {
                                    dx = (i < 12) ? -3 : -10 + i;
                                    dy = (i < 12) ? -3 : 3;
                                }
                                
                                int target_x = x + dx;
                                int target_y = y + dy;
                                if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                                    if (i % 2 == 0) {
                                        buffer[target_y * stride + target_x] = center_intensity + bright_offset;
                                    } else {
                                        buffer[target_y * stride + target_x] = center_intensity - dark_offset;
                                    }
                                }
                            }
                            break;
                            
                        default: // Gradient with corner points
                            int gradient = (x * 100) / width + (y * 100) / height;
                            buffer[y * stride + x] = center_intensity + gradient % 100;
                            break;
                    }
                }
            }
            break;
        }
            
        case 3: { // Pattern specifically for border condition testing
            // The algorithm skips border pixels (y=3 to ysize-3, x=3 to xsize-3)
            // Test edge cases thoroughly
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (y < 3 || y >= height - 3 || x < 3 || x >= width - 3) {
                        // Border region - use extreme values
                        if ((x + y) % 3 == 0) {
                            buffer[y * stride + x] = fdp.ConsumeIntegralInRange<int>(0, 50);
                        } else if ((x + y) % 3 == 1) {
                            buffer[y * stride + x] = fdp.ConsumeIntegralInRange<int>(100, 150);
                        } else {
                            buffer[y * stride + x] = fdp.ConsumeIntegralInRange<int>(200, 255);
                        }
                    } else {
                        // Interior - systematic pattern
                        int grid_x = (x - 3) / 4;
                        int grid_y = (y - 3) / 4;
                        int cell_type = (grid_x + grid_y) % 8;
                        
                        int base = center_intensity;
                        int variation = fdp.ConsumeIntegralInRange<int>(-30, 30);
                        
                        switch (cell_type) {
                            case 0: base = 50 + variation; break;
                            case 1: base = 100 + variation; break;
                            case 2: base = 150 + variation; break;
                            case 3: base = 200 + variation; break;
                            case 4: base = 50 - variation; break;
                            case 5: base = 100 - variation; break;
                            case 6: base = 150 - variation; break;
                            case 7: base = 200 - variation; break;
                        }
                        
                        buffer[y * stride + x] = std::max(0, std::min(255, base));
                    }
                }
            }
            break;
        }
            

            
        case 4: { // Pattern for testing threshold 'b' variations in fast9_corner_score
            // The binary search in fast9_corner_score uses bmin/bmax
            // Create patterns with precise intensity differences
            int b_value = threshold_b;
            int intensity_step = fdp.ConsumeIntegralInRange<int>(5, 40);
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create regions with specific intensity relationships to center
                    int region_x = x / 8;
                    int region_y = y / 8;
                    int region_id = (region_x + region_y) % 4;
                    
                    int base_intensity = 128;
                    
                    switch (region_id) {
                        case 0: // Region where pixels are just above center+b
                            base_intensity = 128 + b_value + intensity_step;
                            break;
                        case 1: // Region where pixels are just below center-b
                            base_intensity = 128 - b_value - intensity_step;
                            break;
                        case 2: // Region where pixels are between center-b and center+b
                            base_intensity = 128 + (intensity_step / 2);
                            break;
                        case 3: // Mixed region
                            if ((x + y) % 2 == 0) {
                                base_intensity = 128 + b_value + intensity_step;
                            } else {
                                base_intensity = 128 - b_value - intensity_step;
                            }
                            break;
                    }
                    
                    // Add noise to break perfect patterns
                    int noise = fdp.ConsumeIntegralInRange<int>(-10, 10);
                    buffer[y * stride + x] = std::max(0, std::min(255, base_intensity + noise));
                }
            }
            break;
        }
            
        case 5: { // Pattern targeting specific decision tree branches
            // Based on analyzing the nested if-else structure
            // Create patterns that force specific code paths
            for (int y = 3; y < height - 3; y++) {
                for (int x = 3; x < width - 3; x++) {
                    int branch_selector = ((x - 3) / 2) % 16 + ((y - 3) / 2) % 16;
                    
                    // Center value
                    buffer[y * stride + x] = center_intensity;
                    
                    // Set specific circular pixels based on branch
                    for (int i = 0; i < 16; i++) {
                        int should_be_bright = (branch_selector >> i) & 1;
                        int dx, dy;
                        
                        // Simplified offset calculation
                        if (i < 8) {
                            dx = (i < 4) ? 3 - i : 8 - i;
                            dy = (i < 4) ? 3 : -3;
                        } else {
                            dx = (i < 12) ? -3 : -10 + i;
                            dy = (i < 12) ? -3 : 3;
                        }
                        
                        int target_x = x + dx;
                        int target_y = y + dy;
                        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
                            if (should_be_bright) {
                                buffer[target_y * stride + target_x] = center_intensity + bright_offset + 30;
                            } else {
                                buffer[target_y * stride + target_x] = center_intensity - dark_offset - 30;
                            }
                        }
                    }
                }
            }
            break;
        }
            
        default: { // Combined approach with multiple strategies
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int pattern_select = (x / 16) % 3;
                    int intensity;
                    
                    if (pattern_select == 0) {
                        // Checkerboard pattern
                        intensity = ((x / 4 + y / 4) % 2 == 0) ? 
                            center_intensity + bright_offset : 
                            center_intensity - dark_offset;
                    } else if (pattern_select == 1) {
                        // Gradient pattern
                        intensity = center_intensity + (x * 100) / width - 50;
                    } else {
                        // Random-ish pattern from fuzzer input
                        if (fdp.remaining_bytes() > 0) {
                            intensity = fdp.ConsumeIntegralInRange<int>(0, 255);
                        } else {
                            intensity = center_intensity;
                        }
                    }
                    
                    buffer[y * stride + x] = intensity;
                }
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for comprehensive FAST-9 testing:
    // - Image dimensions: 8 bytes
    // - Pattern parameters: 20 bytes
    // - Encoder config: 50 bytes
    // - Multiple frames: 100 bytes
    // Total: ~178 bytes minimum
    if (size < 178) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume image dimensions with emphasis on testing border conditions
    // FAST-9 requires 3-pixel border, so we need at least 6x6 images
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(8, 192);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(8, 192);
    
    // Ensure minimum size for meaningful FAST-9 testing
    if (width < 8) width = 8;
    if (height < 8) height = 8;
    
    // Occasionally test edge cases: very small or very large images
    if (fdp.ConsumeBool()) {
        width = fdp.ConsumeIntegralInRange<unsigned int>(6, 64);
        height = fdp.ConsumeIntegralInRange<unsigned int>(6, 64);
    }
    
    // Create test image (I420 format - YUV planar)
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Step 2: Generate precise FAST-9 patterns targeting circular pixel relationships
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 9);
    int threshold_b = fdp.ConsumeIntegralInRange<int>(1, 100); // b parameter in aom_fast9_detect
    
    // Fill Y plane (luma) with precise circular patterns
    if (img->stride[0] > 0 && img->planes[0] != nullptr) {
        generate_precise_circular_pattern(img->planes[0], width, height, img->stride[0],
                                         pattern_type, threshold_b, fdp);
    }
    
    // Fill U and V planes (chroma) with consistent values
    // Chroma planes don't significantly affect FAST-9 corner detection
    for (int plane = 1; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 1) ? ((height + 1) / 2) : ((height + 1) / 2);
            size_t plane_width = (plane == 1) ? ((width + 1) / 2) : ((width + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            // Use mid-gray for chroma (128)
            memset(img->planes[plane], 128, plane_size);
        }
    }
    
    aom_image_t* wrapped_img = img;
    
    // Step 3: Initialize encoder context specifically for global motion estimation
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
    
    // Step 4: Configure encoder for optimal global motion estimation testing
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Configure quality settings that affect motion estimation
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 5000);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(20, 63);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 40);
    
    // Ensure min <= max
    if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) {
        std::swap(cfg.rc_min_quantizer, cfg.rc_max_quantizer);
    }
    
    // Critical: Enable inter-frame motion estimation for global motion
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, 25);
    cfg.g_usage = usage;
    cfg.g_threads = 1; // Single thread for deterministic fuzzing
    cfg.rc_end_usage = AOM_CBR;
    
    // Initialize encoder
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Encoder initialization failed
    }
    
    // Step 5: Set critical controls for global motion estimation
    // MUST enable global motion estimation to reach aom_fast9_detect
    int enable_global_motion = 1; // Always enable for this harness
    aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
    
    // Set CPU usage (affects motion search quality)
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);
    
    // Enable warped motion (related to global motion)
    if (fdp.ConsumeBool()) {
        int enable_warped_motion = fdp.ConsumeBool() ? 1 : 0;
        aom_codec_control(&codec, AV1E_SET_ENABLE_WARPED_MOTION, enable_warped_motion);
    }
    
    // Additional controls that might affect corner detection
    if (fdp.ConsumeBool()) {
        // Enable or disable other motion-related features
        int enable_obmc = fdp.ConsumeBool() ? 1 : 0;
        aom_codec_control(&codec, AV1E_SET_ENABLE_OBMC, enable_obmc);
    }
    
    // Step 6: Encode multiple frames with varying patterns
    // This is critical to exercise the full global motion estimation path
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 12);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // For subsequent frames, modify patterns to simulate motion
        // This helps exercise inter-frame global motion estimation
        if (frame_idx > 0 && fdp.remaining_bytes() > 50) {
            // Change pattern to create motion between frames
            pattern_type = (pattern_type + fdp.ConsumeIntegralInRange<int>(1, 5)) % 10;
            threshold_b = (threshold_b + fdp.ConsumeIntegralInRange<int>(5, 30)) % 100;
            
            // Regenerate image with new pattern
            if (img->stride[0] > 0 && img->planes[0] != nullptr) {
                generate_precise_circular_pattern(img->planes[0], width, height, img->stride[0],
                                                 pattern_type, threshold_b, fdp);
            }
        }
        
        // Encode the frame - this triggers:
        // 1. aom_codec_encode -> encoder_encode -> av1_get_compressed_data
        // 2. Global motion estimation facade (22-function deep call chain)
        // 3. aom_fast9_detect with 875 blocked branches
        // 4. fast9_corner_score with binary search
        res = aom_codec_encode(&codec, wrapped_img, frame_idx, 1, 0);
        
        // Even if encoding fails, the corner detection may have been attempted
        // Continue to test other code paths
        
        // Retrieve encoded data to complete the encoding process
        const aom_codec_cx_pkt_t *pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packets if needed - just consume them
        }
        
        // Break early if we're running low on fuzzer input
        if (fdp.remaining_bytes() < 30) {
            break;
        }
    }
    
    // Step 7: Clean up
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_011
