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

namespace harness_020 {
/*
 * Fuzzing harness for libvpx VP9 encoder sub-pixel motion compensation
 * 
 * Target: vp9_find_best_sub_pixel_tree_pruned (top branch blocker with 403 blocked branches)
 * Goal: Reach SECOND_LEVEL_CHECKS code paths in VP9 encoder motion estimation
 * 
 * This harness specifically targets deep coverage gaps in sub-pixel motion
 * estimation with various search patterns and refinement levels. Unlike
 * harness_018 which tests general algorithmic paths, this harness focuses
 * specifically on sub-pixel refinement functions and their complex branching.
 * 
 * Testing strategy:
 * 1. Create encoder context with diverse motion estimation parameters
 * 2. Configure encoder controls that affect sub-pixel refinement paths
 * 3. Test with varied CPU usage, sharpness, static threshold settings
 * 4. Use different encoding scenarios to trigger motion estimation
 * 5. Test multiple frames with different textures to exercise search patterns
 * 
 * Key target paths:
 * - SECOND_LEVEL_CHECKS in sub-pixel refinement
 * - Cost list optimization paths  
 * - Half-pixel refinement conditions
 * - Forced stop branching (full/qtr/half pixel modes)
 * - Quarter and eighth iteration paths
 */




// Minimum input size for meaningful sub-pixel testing
const size_t MIN_INPUT_SIZE = 256;

// Maximum dimensions for test images
const unsigned int MAX_DIMENSION = 256;

// Helper to create a minimal test image for motion estimation
static vpx_image_t* create_test_image_for_motion(FuzzedDataProvider* fdp, 
                                                 unsigned int width, 
                                                 unsigned int height) {
    if (!fdp || width == 0 || height == 0 || width > MAX_DIMENSION || height > MAX_DIMENSION) {
        return nullptr;
    }
    
    // Use I420 format for motion estimation testing
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        return nullptr;
    }
    
    // Fill image planes with fuzzed data to create texture for motion estimation
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
        
        for (int y = 0; y < h; ++y) {
            std::vector<uint8_t> row_data = fdp->ConsumeBytes<uint8_t>(w);
            memcpy(buf, row_data.data(), w);
            buf += stride;
        }
    }
    
    return img;
}

// Initialize minimal structures for motion estimation testing
static bool initialize_motion_test_structures(FuzzedDataProvider* fdp,
                                              vpx_codec_ctx_t* codec,
                                              vpx_codec_enc_cfg_t* cfg,
                                              vpx_image_t** ref_img,
                                              vpx_image_t** src_img) {
    if (!fdp || !codec || !cfg) return false;
    
    // Choose VP9 encoder
    const vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) return false;
    
    // Initialize encoder configuration
    memset(codec, 0, sizeof(vpx_codec_ctx_t));
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Get default configuration
    if (vpx_codec_enc_config_default(encoder_iface, cfg, 0) != VPX_CODEC_OK) {
        return false;
    }
    
    // Set small dimensions for motion estimation testing
    unsigned int width = fdp->ConsumeIntegralInRange<unsigned int>(64, MAX_DIMENSION);
    unsigned int height = fdp->ConsumeIntegralInRange<unsigned int>(64, MAX_DIMENSION);
    
    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;
    cfg->g_error_resilient = 1;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    // Initialize encoder
    if (vpx_codec_enc_init(codec, encoder_iface, cfg, 0) != VPX_CODEC_OK) {
        return false;
    }
    
    // Create reference and source images for motion estimation
    *ref_img = create_test_image_for_motion(fdp, width, height);
    *src_img = create_test_image_for_motion(fdp, width, height);
    
    if (!*ref_img || !*src_img) {
        if (*ref_img) vpx_img_free(*ref_img);
        if (*src_img) vpx_img_free(*src_img);
        vpx_codec_destroy(codec);
        return false;
    }
    
    return true;
}

// Test sub-pixel motion estimation with various parameter combinations
static void test_sub_pixel_motion_estimation(FuzzedDataProvider* fdp) {
    if (!fdp || fdp->remaining_bytes() < MIN_INPUT_SIZE) return;
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t* ref_img = nullptr;
    vpx_image_t* src_img = nullptr;
    
    // Initialize test structures
    if (!initialize_motion_test_structures(fdp, &codec, &cfg, &ref_img, &src_img)) {
        return;
    }
    
    // Consume motion estimation parameters from fuzzer input
    // These parameters affect internal motion estimation decisions
    
    // 1. CPU usage parameter (-9 to 9) - affects motion estimation quality
    int cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 9);
    
    // 2. Sharpness parameter (0-7) - affects quantization decisions
    uint8_t sharpness = fdp->ConsumeIntegral<uint8_t>() % 8;
    
    // 3. Static threshold - affects motion estimation decisions
    uint16_t static_threshold = fdp->ConsumeIntegral<uint16_t>();
    
    // 4. Auto alt reference setting (0-6 for VP9)
    uint8_t enable_auto_alt_ref = fdp->ConsumeIntegral<uint8_t>() % 7;
    
    // 5. Lossless encoding mode
    uint8_t lossless = fdp->ConsumeBool();
    
    // 6. Tile configuration for parallel processing
    uint8_t tile_columns = fdp->ConsumeIntegral<uint8_t>() % 4;
    uint8_t tile_rows = fdp->ConsumeIntegral<uint8_t>() % 4;
    
    // 7. Color space and range for different video characteristics
    int color_space = fdp->ConsumeIntegralInRange<int>(0, 7);
    int color_range = fdp->ConsumeIntegralInRange<int>(0, 1);
    
    // Configure encoder with motion estimation parameters
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
    vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_threshold);
    vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless);
    vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
    vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, tile_rows);
    vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, color_range);
    
    // Additional motion estimation parameters
    int cq_level = fdp->ConsumeIntegralInRange<int>(0, 63);
    vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
    
    // Encode reference frame first (I-frame)
    vpx_codec_encode(&codec, ref_img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded reference frame data
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            break;
        }
    }
    
    // Encode source frame (P-frame) with motion estimation
    // This triggers internal calls to vp9_find_best_sub_pixel_tree_pruned
    // and other motion estimation functions
    vpx_codec_encode(&codec, src_img, 1, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded source frame data
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame encoded successfully - motion estimation executed
            break;
        }
    }
    
    // Test additional scenarios to exercise different motion estimation paths
    
    // Scenario 1: Encode another frame with different parameters
    if (fdp->remaining_bytes() > 128) {
        vpx_image_t* img2 = create_test_image_for_motion(fdp, cfg.g_w, cfg.g_h);
        if (img2) {
            // Change CPU usage for different motion estimation quality
            int new_cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 9);
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, new_cpu_used);
            
            vpx_codec_encode(&codec, img2, 2, 1, 0, VPX_DL_GOOD_QUALITY);
            vpx_img_free(img2);
        }
    }
    
    // Scenario 2: Test flush with NULL image (tests finalization paths)
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
    
    // Test multiple motion estimation scenarios
    int scenario_count = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    for (int i = 0; i < scenario_count && fdp.remaining_bytes() > MIN_INPUT_SIZE; i++) {
        test_sub_pixel_motion_estimation(&fdp);
    }
    
    return 0;
}

} // namespace harness_020
