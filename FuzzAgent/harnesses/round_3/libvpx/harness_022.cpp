#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_022 {
/*
 * Fuzzing harness for libvpx VP9 encoder advanced motion estimation with deep sub-pixel searches
 * 
 * Target: vp9_find_best_sub_pixel_tree_pruned (416 blocked branches)
 * Goal: Reach SECOND_LEVEL_CHECKS code paths (halfiters > 1, quarteriters > 1, eighthiters > 1)
 * 
 * This harness specifically targets deep coverage gaps in VP9 sub-pixel motion estimation
 * by configuring encoder parameters for exhaustive motion search, focusing on the critical
 * blocked branches in vp9_find_best_sub_pixel_tree_pruned and related functions.
 * 
 * Key differentiators from existing harnesses (020, 021):
 * 1. Explicitly sets cpu_used <= 0 (best quality/slowest modes) to enable exhaustive motion search
 * 2. Configures motion estimation controls for deep sub-pixel iteration paths
 * 3. Uses VP9E_SET_TUNE_CONTENT with SCREEN and FILM content types for different search behaviors
 * 4. Enables auto altref and auto arf for complex prediction structures
 * 5. Tests multiple frame types (keyframes, altref, golden) to stress different code paths
 * 6. Varies frame dimensions and bitrates to trigger different motion estimation strategies
 * 
 * Testing strategy to trigger halfiters > 1, quarteriters > 1, eighthiters > 1:
 * 1. Configure encoder with best quality settings (cpu_used <= 0)
 * 2. Set content-specific tuning for different motion search behaviors
 * 3. Create textured images with motion patterns
 * 4. Encode sequences with varied frame types and references
 * 5. Use extreme parameter combinations to stress iteration paths
 */




// Minimum input size for advanced motion estimation testing
const size_t MIN_INPUT_SIZE = 1024;

// Maximum dimensions for test images
const unsigned int MAX_DIMENSION = 320;

// Helper to create textured test image with motion patterns for sub-pixel testing
static vpx_image_t* create_motion_test_image(FuzzedDataProvider* fdp, 
                                             unsigned int width, 
                                             unsigned int height,
                                             bool complex_pattern) {
    if (!fdp || width == 0 || height == 0 || width > MAX_DIMENSION || height > MAX_DIMENSION) {
        return nullptr;
    }
    
    // Use I420 format for VP9 encoding
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        return nullptr;
    }
    
    // Create image with patterns that trigger complex motion estimation
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char* buf = img->planes[plane];
        const int stride = img->stride[plane];
        int w = (plane == 0) ? width : (width + 1) / 2;
        int h = (plane == 0) ? height : (height + 1) / 2;
        
        // Ensure we have enough data for this plane
        if (fdp->remaining_bytes() < (size_t)(w * h)) {
            vpx_img_free(img);
            return nullptr;
        }
        
        if (complex_pattern) {
            // Create complex texture with gradients and patterns for deep motion estimation
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    // Create structured patterns to trigger sub-pixel refinement
                    uint8_t base = fdp->ConsumeIntegral<uint8_t>();
                    // Add structure for motion estimation
                    uint8_t pattern = (x % 8 < 4) ? base : 255 - base;
                    pattern = (y % 8 < 4) ? pattern : 255 - pattern;
                    buf[y * stride + x] = pattern;
                }
            }
        } else {
            // Simple fill with fuzzed data
            for (int y = 0; y < h; ++y) {
                std::vector<uint8_t> row_data = fdp->ConsumeBytes<uint8_t>(w);
                memcpy(buf + y * stride, row_data.data(), w);
            }
        }
    }
    
    return img;
}

// Test advanced motion estimation with deep sub-pixel searches
static void test_deep_sub_pixel_motion_estimation(FuzzedDataProvider* fdp) {
    if (!fdp || fdp->remaining_bytes() < MIN_INPUT_SIZE) return;
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Choose VP9 encoder
    const vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) return;
    
    // Initialize encoder configuration
    memset(&codec, 0, sizeof(vpx_codec_ctx_t));
    memset(&cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Get default configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Consume image dimensions from fuzzer input
    unsigned int width = fdp->ConsumeIntegralInRange<unsigned int>(64, MAX_DIMENSION);
    unsigned int height = fdp->ConsumeIntegralInRange<unsigned int>(64, MAX_DIMENSION);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // CRITICAL: Configure for exhaustive motion search
    // Use lower bitrate to force more compression and complex motion estimation
    cfg.rc_target_bitrate = fdp->ConsumeIntegralInRange<unsigned int>(100, 2000);
    
    // Enable lookahead for better motion estimation (affects halfiters/quarteriters)
    cfg.g_lag_in_frames = fdp->ConsumeIntegral<uint8_t>() % 4;  // 0-3 lookahead frames
    
    // Error resilience for robust encoding
    cfg.g_error_resilient = fdp->ConsumeBool() ? 1 : 0;
    
    // Encoding pass mode
    cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Initialize encoder using vpx_codec_enc_init_ver as specified in guidance
    if (vpx_codec_enc_init_ver(&codec, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return;
    }
    
    // ========================================================================
    // CRITICAL: Set control parameters that affect sub-pixel iteration behavior
    // These are the key differentiators from existing harnesses
    // ========================================================================
    
    // 1. CPU USED - MUST be <= 0 for exhaustive motion search (best quality/slowest)
    // This directly affects halfiters, quarteriters, eighthiters iteration counts
    int cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 0);  // Focus on <= 0 range
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    
    // 2. Tune content type - affects motion estimation for different content
    // SCREEN and FILM content trigger different search strategies
    unsigned int tune_content = fdp->ConsumeIntegral<uint8_t>() % 3;
    switch (tune_content) {
        case 0:
            vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_DEFAULT);
            break;
        case 1:
            vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_SCREEN);
            break;
        case 2:
            vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_FILM);
            break;
    }
    
    // 3. Enable auto altref and auto arf for complex prediction structures
    // These affect motion estimation reference frame selection
    unsigned int enable_auto_alt_ref = fdp->ConsumeIntegral<uint8_t>() % 7;  // 0-6 for VP9
    vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    
    // 4. Motion vector unit test - can affect motion search behavior
    unsigned int motion_vector_test = fdp->ConsumeIntegral<uint8_t>() % 3;
    vpx_codec_control(&codec, VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST, motion_vector_test);
    
    // 5. Sharpness parameter - affects quantization and motion estimation decisions
    uint8_t sharpness = fdp->ConsumeIntegral<uint8_t>() % 8;
    vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
    
    // 6. Static threshold - affects motion estimation decisions
    uint16_t static_threshold = fdp->ConsumeIntegral<uint16_t>();
    vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_threshold);
    
    // 7. Tile configuration - affects parallel processing of motion estimation
    uint8_t tile_columns = fdp->ConsumeIntegral<uint8_t>() % 4;
    uint8_t tile_rows = fdp->ConsumeIntegral<uint8_t>() % 4;
    vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
    vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, tile_rows);
    
    // 8. Row-based multi-threading for motion estimation
    unsigned int row_mt = fdp->ConsumeBool() ? 1 : 0;
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, row_mt);
    
    // 9. Color space and range - can affect motion estimation for different video characteristics
    int color_space = fdp->ConsumeIntegralInRange<int>(0, 7);
    int color_range = fdp->ConsumeIntegralInRange<int>(0, 1);
    vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, color_range);
    
    // 10. Additional quality parameters that affect motion search
    int cq_level = fdp->ConsumeIntegralInRange<int>(0, 63);
    vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
    
    // 11. Lossless encoding mode - affects motion estimation completely
    uint8_t lossless = fdp->ConsumeBool();
    vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless);
    
    // 12. AQ mode - affects adaptive quantization which interacts with motion estimation
    unsigned int aq_mode = fdp->ConsumeIntegral<uint8_t>() % 4;
    vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode);
    
    // Create reference and source images for motion estimation testing
    bool complex_pattern = fdp->ConsumeBool();
    vpx_image_t* ref_img = create_motion_test_image(fdp, width, height, complex_pattern);
    vpx_image_t* src_img = create_motion_test_image(fdp, width, height, complex_pattern);
    
    if (!ref_img || !src_img) {
        if (ref_img) vpx_img_free(ref_img);
        if (src_img) vpx_img_free(src_img);
        vpx_codec_destroy(&codec);
        return;
    }
    
    // ========================================================================
    // Encode sequence to trigger deep sub-pixel motion estimation
    // ========================================================================
    
    // Encode reference frame (keyframe)
    vpx_codec_encode(&codec, ref_img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded reference frame data
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            break;
        }
    }
    
    // Encode source frame with motion estimation
    // This triggers internal calls to vp9_find_best_sub_pixel_tree_pruned
    vpx_codec_encode(&codec, src_img, 1, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded source frame data
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            break;
        }
    }
    
    // Test additional scenarios to exercise different motion estimation paths
    
    // Scenario 1: Encode another frame with different parameters
    if (fdp->remaining_bytes() > 256) {
        vpx_image_t* img2 = create_motion_test_image(fdp, width, height, !complex_pattern);
        if (img2) {
            // Change CPU usage to test different motion estimation quality
            int new_cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 0);
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, new_cpu_used);
            
            // Change content tuning
            unsigned int new_tune_content = fdp->ConsumeIntegral<uint8_t>() % 3;
            switch (new_tune_content) {
                case 0:
                    vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_DEFAULT);
                    break;
                case 1:
                    vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_SCREEN);
                    break;
                case 2:
                    vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_FILM);
                    break;
            }
            
            vpx_codec_encode(&codec, img2, 2, 1, 0, VPX_DL_GOOD_QUALITY);
            vpx_img_free(img2);
        }
    }
    
    // Scenario 2: Test with golden frame (altref)
    if (fdp->remaining_bytes() > 128) {
        vpx_image_t* img3 = create_motion_test_image(fdp, width, height, complex_pattern);
        if (img3) {
            // Use altref frame flags
            vpx_codec_encode(&codec, img3, 3, 1, VP8_EFLAG_NO_REF_LAST, VPX_DL_GOOD_QUALITY);
            vpx_img_free(img3);
        }
    }
    
    // Scenario 3: Test flush with NULL image (tests finalization paths)
    vpx_codec_encode(&codec, nullptr, 0, 0, 0, VPX_DL_GOOD_QUALITY);
    
    // Cleanup
    vpx_img_free(ref_img);
    vpx_img_free(src_img);
    vpx_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need sufficient input for comprehensive motion estimation testing
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test multiple motion estimation scenarios to cover different code paths
    int scenario_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    for (int i = 0; i < scenario_count && fdp.remaining_bytes() > MIN_INPUT_SIZE; i++) {
        test_deep_sub_pixel_motion_estimation(&fdp);
    }
    
    return 0;
}

} // namespace harness_022
