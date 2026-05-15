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

namespace harness_007 {
/*
 * Fuzzing harness for libaom AV1 encoder global motion estimation and corner detection
 * Specifically targets aom_fast9_detect with 409 blocked branches
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_007_guidance.md
 * Semantic differentiation: harness_006 focused on encoder configuration validation;
 * harness_007 focuses on global motion estimation and corner detection to specifically target aom_fast9_detect blocked branches.
 */



// Helper function to create corner detection patterns in image data
static void generate_corner_pattern(uint8_t* buffer, int width, int height, int stride,
                                    int pattern_type, int brightness_threshold) {
    // Clear buffer
    memset(buffer, 128, stride * height); // Start with mid-gray
    
    switch (pattern_type % 5) {
        case 0: // Chessboard pattern
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (((x / 8) + (y / 8)) % 2 == 0) {
                        buffer[y * stride + x] = 200; // Bright squares
                    } else {
                        buffer[y * stride + x] = 50;  // Dark squares
                    }
                }
            }
            break;
            
        case 1: // Random noise with bright/dark spots
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Base noise
                    int value = 128 + (rand() % 64 - 32);
                    
                    // Add occasional bright/dark spots (corners)
                    if ((x % 16 == 0) && (y % 16 == 0)) {
                        value = 240; // Bright corner
                    } else if ((x % 16 == 8) && (y % 16 == 8)) {
                        value = 20;  // Dark corner
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, value));
                }
            }
            break;
            
        case 2: // L-shaped patterns (actual corners)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create L-shaped patterns at regular intervals
                    bool is_corner = false;
                    
                    // Horizontal arm of L
                    if ((y % 32 == 0) && (x % 32 >= 0) && (x % 32 < 16)) {
                        is_corner = true;
                    }
                    // Vertical arm of L  
                    if ((x % 32 == 0) && (y % 32 >= 0) && (y % 32 < 16)) {
                        is_corner = true;
                    }
                    
                    if (is_corner) {
                        buffer[y * stride + x] = 220; // Bright L-shapes
                    } else {
                        buffer[y * stride + x] = 80;  // Dark background
                    }
                }
            }
            break;
            
        case 3: // Varying brightness levels
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create gradient with corners at transitions
                    int gradient = (x * 255) / std::max(1, width);
                    int checker = ((x / 8) + (y / 8)) % 2;
                    
                    int value = gradient;
                    if (checker) {
                        value = 255 - gradient;
                    }
                    
                    // Add threshold-based corners
                    if (abs(value - brightness_threshold) < 10) {
                        value = (value > brightness_threshold) ? 250 : 10;
                    }
                    
                    buffer[y * stride + x] = (uint8_t)std::max(0, std::min(255, value));
                }
            }
            break;
            
        case 4: // Grid of isolated corners
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create isolated bright pixels at grid points
                    bool is_corner = ((x % 16 == 7) || (x % 16 == 8)) &&
                                     ((y % 16 == 7) || (y % 16 == 8));
                    
                    if (is_corner) {
                        buffer[y * stride + x] = 240;
                    } else {
                        buffer[y * stride + x] = 100;
                    }
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Encoder configuration parameters: ~50 bytes
    // - Image pattern parameters: ~20 bytes
    // - Multiple frames worth of pattern data: ~100 bytes
    // Total minimum: ~170 bytes
    if (size < 170) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume encoder configuration parameters for global motion estimation
    // Use small dimensions (<16x16) to trigger border conditions in aom_fast9_detect
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(8, 64);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(8, 64);
    
    // Force some small sizes for border condition testing
    if (fdp.ConsumeBool()) {
        width = fdp.ConsumeIntegralInRange<unsigned int>(4, 16);
        height = fdp.ConsumeIntegralInRange<unsigned int>(4, 16);
    }
    
    // Create test image (I420 format)
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Step 2: Fill image with corner detection patterns
    // Consume pattern parameters from fuzzer input
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 4);
    int brightness_threshold = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Fill Y plane (luma) with corner patterns
    if (img->stride[0] > 0 && img->planes[0] != nullptr) {
        generate_corner_pattern(img->planes[0], width, height, img->stride[0],
                                pattern_type, brightness_threshold);
    }
    
    // Fill U and V planes (chroma) with mid-gray
    for (int plane = 1; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 1) ? ((height + 1) / 2) : ((height + 1) / 2);
            size_t plane_width = (plane == 1) ? ((width + 1) / 2) : ((width + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            memset(img->planes[plane], 128, plane_size);
        }
    }
    
    aom_image_t* wrapped_img = img;
    
    // Step 3: Initialize encoder context
    aom_codec_ctx_t codec;
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    
    // Consume initialization flags
    aom_codec_flags_t flags = static_cast<aom_codec_flags_t>(
        fdp.ConsumeIntegral<uint32_t>());
    
    // Get default configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2); // 0-2 for AOM_USAGE values
    
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Failed to get default config
    }
    
    // Step 4: Modify configuration parameters to enable/affect global motion estimation
    // Set small dimensions for border condition testing in corner detection
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Adjust quality/bitrate settings that influence motion estimation
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(20, 63);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 40);
    
    // Ensure min <= max
    if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) {
        std::swap(cfg.rc_min_quantizer, cfg.rc_max_quantizer);
    }
    
    // Set cpu_used to levels that affect motion search quality
    // Higher cpu_used (1-9) reduces motion search quality, lower (0) is best quality
    cfg.g_usage = usage;
    cfg.g_threads = 1; // Single thread for deterministic testing
    cfg.rc_end_usage = AOM_CBR; // Constant bitrate for consistent testing
    
    // Set lag-in-frames to enable inter-frame motion estimation
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    
    // Initialize encoder with version check
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Encoder initialization failed
    }

    // Step 5: Optionally set global motion related controls via aom_codec_control
    // These controls can influence global motion estimation behavior
    if (fdp.ConsumeBool()) {
        int enable_global_motion = fdp.ConsumeBool() ? 1 : 0;
        aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
    }
    
    // Note: AV1E_SET_GLOBAL_MOTION_QUALITY doesn't exist in the API
    // Use other controls that might affect motion estimation quality instead
    if (fdp.ConsumeBool()) {
        int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
        aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);
    }

    // Step 6: Encode multiple frames with different patterns to trigger corner detection
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // For subsequent frames, modify the pattern slightly to create motion
        if (frame_idx > 0 && fdp.remaining_bytes() > 10) {
            // Shift brightness or change pattern type slightly
            brightness_threshold = (brightness_threshold + 20) % 255;
            pattern_type = (pattern_type + 1) % 5;
            
            // Regenerate image with new pattern
            if (img->stride[0] > 0 && img->planes[0] != nullptr) {
                generate_corner_pattern(img->planes[0], width, height, img->stride[0],
                                        pattern_type, brightness_threshold);
            }
        }
        
        // Encode the frame - this should trigger global motion estimation
        // and corner detection (aom_fast9_detect) internally
        res = aom_codec_encode(&codec, wrapped_img, frame_idx, 1, 0);
        
        // Don't check result - we want to test error paths too
        // The encoding process should exercise aom_fast9_detect regardless of success
        
        // Get compressed data (optional)
        const aom_codec_cx_pkt_t *pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packets if needed, but not required for coverage
        }
    }
    
    // Step 7: Flush encoder to process any remaining frames
    res = aom_codec_encode(&codec, nullptr, -1, 1, 0);
    
    // Step 8: Cleanup
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_007
