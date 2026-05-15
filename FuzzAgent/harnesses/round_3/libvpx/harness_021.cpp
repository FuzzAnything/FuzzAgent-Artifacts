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

namespace harness_021 {
/*
 * Fuzzing harness for libvpx VP9 encoder sub-pixel motion compensation
 * 
 * Target: vp9_find_best_sub_pixel_tree_pruned (409 blocked branches)
 * Goal: Reach SECOND_LEVEL_CHECKS code paths by varying motion search iteration parameters
 * 
 * This harness specifically targets deep coverage gaps in sub-pixel motion
 * compensation by systematically exploring encoder control parameters that affect
 * motion search iteration counts (halfiters, quarteriters, eighthiters).
 * 
 * Key differentiators from harness_020:
 * 1. Uses VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST control (0% coverage)
 * 2. Uses VP8E_SET_MAX_INTRA_BITRATE_PCT control (0% coverage)
 * 3. Systematically varies VP9E_SET_TUNE_CONTENT for different content types
 * 4. Exercises VP8E_SET_GF_CBR_BOOST_PCT for CBR mode variations
 * 5. Tests VP9E_SET_ROW_MT for multi-threading effects
 * 6. Configures tile columns for parallel processing variations
 * 
 * Testing strategy to trigger halfiters > 1, quarteriters > 1, eighthiters > 1:
 * 1. Create complex motion patterns in test images
 * 2. Use varied encoder configurations that affect motion search quality
 * 3. Test with different content types (SCREEN, FILM, etc.)
 * 4. Vary compression modes (CBR, CQ) to affect motion estimation behavior
 * 5. Test with and without lookahead (g_lag_in_frames > 0)
 * 6. Use extreme parameter values to stress iteration paths
 */




// Minimum input size for comprehensive encoder testing
const size_t MIN_INPUT_SIZE = 512;

// Maximum dimensions for test images
const unsigned int MAX_DIMENSION = 320;

// Helper to create textured test image with motion patterns
static vpx_image_t* create_textured_image(FuzzedDataProvider* fdp, 
                                          unsigned int width, 
                                          unsigned int height,
                                          bool complex_motion) {
    if (!fdp || width == 0 || height == 0 || width > MAX_DIMENSION || height > MAX_DIMENSION) {
        return nullptr;
    }
    
    // Use I420 format for VP9 encoding
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        return nullptr;
    }
    
    // Create textured image with patterns that trigger complex motion estimation
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
        
        if (complex_motion) {
            // Create complex texture with gradients and patterns for motion estimation
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    // Create gradient and pattern for better motion estimation
                    uint8_t value = fdp->ConsumeIntegral<uint8_t>();
                    // Add some structure to trigger motion estimation
                    if ((x + y) % 8 == 0) value = 255 - value;
                    buf[y * stride + x] = value;
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

// Test sub-pixel motion compensation with varied control parameters
static void test_sub_pixel_compensation(FuzzedDataProvider* fdp) {
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
    
    // Consume encoder configuration parameters that affect motion estimation
    // These are critical for reaching deep iteration paths
    // 1. Rate control mode - affects motion search quality
    uint8_t rc_mode = fdp->ConsumeIntegral<uint8_t>() % 3;
    unsigned int cq_level = 0;  // Will be set via control API if CQ mode is selected
    
    switch (rc_mode) {
        case 0:
            cfg.rc_end_usage = VPX_CBR;  // Constant Bitrate
            cfg.rc_target_bitrate = fdp->ConsumeIntegralInRange<unsigned int>(100, 5000);
            break;
        case 1:
            cfg.rc_end_usage = VPX_CQ;   // Constrained Quality
            cq_level = fdp->ConsumeIntegralInRange<unsigned int>(0, 63);
            break;
        case 2:
            cfg.rc_end_usage = VPX_VBR;  // Variable Bitrate
            cfg.rc_target_bitrate = fdp->ConsumeIntegralInRange<unsigned int>(100, 5000);
            break;
    }
    
    // 2. Lookahead frames - affects motion estimation accuracy
    cfg.g_lag_in_frames = fdp->ConsumeIntegral<uint8_t>() % 4;  // 0-3 lookahead frames
    
    // 3. Error resilience for robust encoding
    cfg.g_error_resilient = fdp->ConsumeBool() ? 1 : 0;
    
    // 4. Encoding pass mode
    cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // ========================================================================
    // CRITICAL: Set control parameters that affect sub-pixel iteration behavior
    // ========================================================================
    
    // 1. Motion vector unit test (0% coverage target)
    // 0: off, 1: MAX_EXTREME_MV, 2: MIN_EXTREME_MV
    unsigned int motion_vector_test = fdp->ConsumeIntegral<uint8_t>() % 3;
    vpx_codec_control(&codec, VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST, motion_vector_test);
    
    // 2. Maximum intra bitrate percentage (0% coverage target)
    unsigned int max_intra_bitrate_pct = fdp->ConsumeIntegralInRange<unsigned int>(0, 1000);
    vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_bitrate_pct);
    
    // 3. Tune content type - affects motion estimation for different content
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
    
    // 4. GF CBR boost percentage - affects golden frame allocation
    unsigned int gf_cbr_boost_pct = fdp->ConsumeIntegralInRange<unsigned int>(0, 200);
    vpx_codec_control(&codec, VP8E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
    
    // 5. Row-based multi-threading
    unsigned int row_mt = fdp->ConsumeBool() ? 1 : 0;
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, row_mt);
    
    // 6. Tile columns for parallel processing
    unsigned int tile_columns = fdp->ConsumeIntegral<uint8_t>() % 6;  // 0-5 columns
    vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
    
    // 7. CPU usage - directly affects motion search quality
    int cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 9);
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    
    // 8. Sharpness parameter
    unsigned int sharpness = fdp->ConsumeIntegral<uint8_t>() % 8;
    vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
    
    // 9. CQ level for constrained quality mode
    vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
    
    // 10. Static threshold for motion estimation decisions
    unsigned int static_threshold = fdp->ConsumeIntegral<uint16_t>();
    vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_threshold);
    
    // 11. Lossless mode
    unsigned int lossless = fdp->ConsumeBool() ? 1 : 0;
    vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless);
    
    // 12. Color space and range
    int color_space = fdp->ConsumeIntegralInRange<int>(0, 7);
    int color_range = fdp->ConsumeIntegralInRange<int>(0, 1);
    vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, color_range);

    // Create reference frame (I-frame) with complex texture
    bool use_complex_motion = fdp->ConsumeBool();
    vpx_image_t* ref_img = create_textured_image(fdp, width, height, use_complex_motion);
    if (!ref_img) {
        vpx_codec_destroy(&codec);
        return;
    }
    
    // Encode reference frame
    vpx_codec_encode(&codec, ref_img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded reference frame
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            break;  // Reference frame encoded
        }
    }
    
    // Create source frame with different texture to trigger motion estimation
    vpx_image_t* src_img = create_textured_image(fdp, width, height, !use_complex_motion);
    if (!src_img) {
        vpx_img_free(ref_img);
        vpx_codec_destroy(&codec);
        return;
    }
    
    // ========================================================================
    // CRITICAL: Encode source frame to trigger sub-pixel motion estimation
    // This should call vp9_find_best_sub_pixel_tree_pruned internally
    // ========================================================================
    
    // Encode with motion estimation (P-frame)
    vpx_codec_encode(&codec, src_img, 1, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded frame
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Motion estimation executed successfully
            break;
        }
    }
    
    // ========================================================================
    // Additional scenarios to exercise different iteration paths
    // ========================================================================
    
    // Scenario 1: Encode additional frames with varied parameters
    if (fdp->remaining_bytes() > 256) {
        // Change CPU usage for different motion search quality
        int new_cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 9);
        vpx_codec_control(&codec, VP8E_SET_CPUUSED, new_cpu_used);
        
        // Create another frame with different texture
        vpx_image_t* img2 = create_textured_image(fdp, width, height, fdp->ConsumeBool());
        if (img2) {
            vpx_codec_encode(&codec, img2, 2, 1, 0, VPX_DL_GOOD_QUALITY);
            vpx_img_free(img2);
        }
    }
    
    // Scenario 2: Test flush with NULL image
    vpx_codec_encode(&codec, nullptr, 0, 0, 0, VPX_DL_GOOD_QUALITY);
    
    // Cleanup
    vpx_img_free(ref_img);
    vpx_img_free(src_img);
    vpx_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need sufficient input for comprehensive encoder testing
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test multiple encoder configurations to explore different iteration paths
    // Each configuration tests different combinations of control parameters
    int config_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    for (int i = 0; i < config_count && fdp.remaining_bytes() > MIN_INPUT_SIZE; i++) {
        test_sub_pixel_compensation(&fdp);
    }
    
    return 0;
}

} // namespace harness_021
