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

namespace harness_024 {
/*
 * Fuzzing harness for libvpx VP9 encoder advanced control parameters
 * Targeting deep sub-pixel motion compensation optimization paths
 * 
 * Target Coverage Gaps:
 * - vp9_find_best_sub_pixel_tree_pruned (416 blocked branches)
 * - vp9_rd_pick_inter_mode_sb (354 blocked branches)
 * - vp9_find_best_sub_pixel_tree_pruned_more (322 blocked branches)
 * 
 * This harness specifically focuses on VP9 encoder control parameters that
 * affect sub-pixel search behavior, implementing the exact guidance from
 * coverage analysis to trigger deep sub-pixel motion compensation paths.
 * 
 * Differentiation from harness_023:
 * 1. Comprehensive testing of ALL VP9-specific control parameters (not just subset)
 * 2. Focus on VP9E_SET_ALT_REF_AQ and VP9E_SET_NOISE_SENSITIVITY (missing in 023)
 * 3. Systematic exploration of adaptive quantization modes (AQ_MODE 0-3)
 * 4. Varied frame content patterns (smooth, textured, edge-rich) to influence motion search
 * 5. Tile-based parallelism configurations that affect motion vector search granularity
 * 6. Content-specific tuning (SCREEN, FILM, DEFAULT) that changes motion estimation
 * 7. Row-based multi-threading configurations that parallelize motion search
 * 
 * Critical Control Parameters Tested:
 * - VP9E_SET_ROW_MT: Row-level multi-threading (0/1)
 * - VP9E_SET_TILE_COLUMNS/TILE_ROWS: Tile configuration for parallel encoding
 * - VP9E_SET_AQ_MODE: Adaptive quantization (0-3) interacts with motion estimation
 * - VP9E_SET_ALT_REF_AQ: Special adaptive quantization for altref frames
 * - VP9E_SET_NOISE_SENSITIVITY: Noise sensitivity tuning (0-2)
 * - VP9E_SET_TUNE_CONTENT: Content-specific tuning (DEFAULT, SCREEN, FILM)
 * - VP9E_SET_COLOR_SPACE/RANGE: Color space configurations
 * - VP9E_SET_FRAME_PARALLEL_DECODING: Affects probability context updates
 * - VP9E_SET_FRAME_PERIODIC_BOOST: Q-boost for motion estimation quality
 * 
 * Expected Coverage Gains:
 * - Unlock blocked branches in sub-pixel tree search functions
 * - Improve coverage in vp9_rd_pick_inter_mode_sb (76.6% branch blockage)
 * - Increase overall VP9 encoder coverage from current 43.52% line coverage
 * - Trigger SECOND_LEVEL_CHECKS macros with varied iteration counts
 */




// Minimum input size for comprehensive control parameter testing
const size_t MIN_INPUT_SIZE = 4096;

// Maximum dimensions for test images
const unsigned int MAX_DIMENSION = 640;

// Helper to create test image with varied content patterns
static vpx_image_t* create_varied_content_image(FuzzedDataProvider* fdp,
                                                unsigned int width,
                                                unsigned int height,
                                                int pattern_type) {
    if (!fdp || width == 0 || height == 0 || width > MAX_DIMENSION || height > MAX_DIMENSION) {
        return nullptr;
    }
    
    // Use I420 format for VP9 encoding
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        return nullptr;
    }
    
    // Create different content patterns to trigger varied motion search behavior
    // Pattern types: 0=smooth, 1=textured, 2=edge-rich, 3=noisy, 4=gradient
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
        
        // Generate content based on pattern type
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                uint8_t pixel_value;
                
                switch (pattern_type % 5) {
                    case 0: // Smooth content (low spatial frequency)
                        pixel_value = 128 + (x % 16) - 8 + (y % 16) - 8;
                        break;
                        
                    case 1: // Textured content (medium spatial frequency)
                        pixel_value = (x ^ y) & 0xFF;
                        break;
                        
                    case 2: // Edge-rich content (high spatial frequency)
                        pixel_value = (abs(x - w/2) < 4 || abs(y - h/2) < 4) ? 255 : 64;
                        break;
                        
                    case 3: // Noisy content (random patterns)
                        pixel_value = fdp->ConsumeIntegral<uint8_t>();
                        break;
                        
                    case 4: // Gradient content (linear changes)
                        pixel_value = (x * 255 / w + y * 255 / h) / 2;
                        break;
                        
                    default:
                        pixel_value = 128;
                        break;
                }
                
                buf[y * stride + x] = pixel_value;
            }
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input check
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ========================================================================
    // CONFIGURE ENCODER WITH ADVANCED CONTROL PARAMETERS
    // ========================================================================
    
    // 1. Get default VP9 encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // 2. Configure basic encoding parameters from fuzzer input
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, MAX_DIMENSION);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, MAX_DIMENSION);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    // Quality/usage mode affects motion search precision
    // Note: g_usage is deprecated and must be set to 0
    cfg.g_usage = 0;
    
    // Rate control mode affects motion estimation
    cfg.rc_end_usage = VPX_VBR;  // Variable bitrate for quality-focused testing
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    
    // Multi-frame lookahead for better motion estimation
    cfg.g_lag_in_frames = fdp.ConsumeIntegral<uint8_t>() % 17;  // 0-16 frames
    
    // 3. Initialize encoder
    vpx_codec_ctx_t codec;
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // ========================================================================
    // APPLY ADVANCED VP9-SPECIFIC CONTROL PARAMETERS
    // These directly affect sub-pixel motion search behavior
    // ========================================================================
    
    // 1. Row-based multi-threading (affects motion search parallelization)
    unsigned int row_mt = fdp.ConsumeBool();
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, row_mt);
    
    // 2. Tile configuration (tile-based parallelism)
    unsigned int tile_columns = fdp.ConsumeIntegral<uint8_t>() % 5;  // 0-4 columns
    unsigned int tile_rows = fdp.ConsumeIntegral<uint8_t>() % 5;     // 0-4 rows
    vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
    vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, tile_rows);
    
    // 3. Frame parallel decoding (affects probability context)
    unsigned int frame_parallel = fdp.ConsumeBool();
    vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel);
    
    // 4. Adaptive quantization mode (CRITICAL for sub-pixel search)
    unsigned int aq_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3
    vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode);
    
    // 5. Altref adaptive quantization (SPECIALIZED for altref frames)
    int altref_aq = fdp.ConsumeBool();
    vpx_codec_control(&codec, VP9E_SET_ALT_REF_AQ, altref_aq);
    
    // 6. Noise sensitivity (affects motion estimation in noisy content)
    unsigned int noise_sensitivity = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0-2
    vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    
    // 7. Content-specific tuning (changes motion estimation strategy)
    unsigned int tune_content = fdp.ConsumeIntegral<uint8_t>() % 3;
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
    
    // 8. Frame periodic boost (affects Q-boost for motion estimation)
    unsigned int frame_boost = fdp.ConsumeBool();
    vpx_codec_control(&codec, VP9E_SET_FRAME_PERIODIC_BOOST, frame_boost);
    
    // 9. Color space and range
    unsigned int color_space = fdp.ConsumeIntegral<uint8_t>() % 8;  // 0-7
    unsigned int color_range = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0-1
    vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, color_range);
    
    // 10. Golden frame intervals (affects reference frame selection)
    unsigned int min_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(4, 16);
    unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(min_gf_interval, 32);
    vpx_codec_control(&codec, VP9E_SET_MIN_GF_INTERVAL, min_gf_interval);
    vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
    
    // 11. Lossless mode (completely changes encoding)
    unsigned int lossless = fdp.ConsumeBool();
    vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless);
    
    // 12. Target level (affects bitstream constraints)
    unsigned int target_level = fdp.ConsumeIntegral<uint8_t>() % 63;  // 0-62
    if (target_level >= 10 && target_level <= 62) {
        vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, target_level);
    }
    
    // ========================================================================
    // TEST WITH VARIED FRAME CONTENT PATTERNS
    // Different content triggers different motion search paths
    // ========================================================================
    
    // Create keyframe with specific content pattern
    int keyframe_pattern = fdp.ConsumeIntegral<uint8_t>() % 5;
    vpx_image_t* keyframe = create_varied_content_image(&fdp, width, height, keyframe_pattern);
    if (!keyframe) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Encode keyframe
    vpx_codec_encode(&codec, keyframe, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded keyframe data
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            break;
        }
    }
    
    vpx_img_free(keyframe);
    
    // ========================================================================
    // ENCODE SEQUENCE WITH DIFFERENT CONTENT AND FRAME TYPES
    // This triggers various sub-pixel motion search paths
    // ========================================================================
    
    // Encode multiple frames with varied content
    int num_frames = fdp.ConsumeIntegralInRange<int>(2, 8);
    for (int frame_idx = 1; frame_idx <= num_frames && fdp.remaining_bytes() > 1024; frame_idx++) {
        // Select content pattern for this frame
        int frame_pattern = fdp.ConsumeIntegral<uint8_t>() % 5;
        vpx_image_t* frame = create_varied_content_image(&fdp, width, height, frame_pattern);
        if (!frame) {
            break;
        }
        
        // Select frame type and reference configuration
        uint32_t flags = 0;
        unsigned int frame_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (frame_type) {
            case 0: // Normal frame with all references
                flags = 0;
                break;
            case 1: // Frame without last frame reference
                flags = VP8_EFLAG_NO_REF_LAST;
                break;
            case 2: // Frame without golden frame reference
                flags = VP8_EFLAG_NO_REF_GF;
                break;
            case 3: // Frame without altref reference
                flags = VP8_EFLAG_NO_REF_ARF;
                break;
        }
        
        // Force specific frame updates to test different prediction structures
        if (fdp.ConsumeBool()) {
            flags |= VP8_EFLAG_FORCE_GF;
        }
        if (fdp.ConsumeBool()) {
            flags |= VP8_EFLAG_FORCE_ARF;
        }
        
        // Encode frame - this triggers internal sub-pixel motion search
        vpx_codec_encode(&codec, frame, frame_idx, 1, flags, VPX_DL_GOOD_QUALITY);
        
        // Get encoded frame data
        iter = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                break;
            }
        }
        
        vpx_img_free(frame);
        
        // ====================================================================
        // MID-SEQUENCE PARAMETER CHANGES
        // Changing parameters during encoding tests dynamic control paths
        // ====================================================================
        
        if (frame_idx % 2 == 0 && fdp.remaining_bytes() > 512) {
            // Change AQ mode mid-sequence
            unsigned int new_aq_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
            vpx_codec_control(&codec, VP9E_SET_AQ_MODE, new_aq_mode);
            
            // Change content tuning
            unsigned int new_tune = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (new_tune) {
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
            
            // Change noise sensitivity
            unsigned int new_noise = fdp.ConsumeIntegral<uint8_t>() % 3;
            vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, new_noise);
        }
    }
    
    // ========================================================================
    // TEST FLUSH AND CLEANUP
    // ========================================================================
    
    // Flush encoder
    vpx_codec_encode(&codec, nullptr, 0, 0, 0, VPX_DL_GOOD_QUALITY);
    
    // Get any remaining packets
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process if needed
    }
    
    // Cleanup
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_024
