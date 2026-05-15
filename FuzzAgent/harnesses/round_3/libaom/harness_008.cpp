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

namespace harness_008 {
/*
 * Fuzzing harness for libaom AV1 encoder transform type search and sorting network functions
 * Specifically targets av1_sort_fi32_16 with 585 blocked branches and search_tx_type with 88 blocked branches
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_008_guidance.md
 * Semantic differentiation: harness_007 targeted global motion estimation; 
 * harness_008 focuses on transform type search and sorting network functions.
 */



// Helper function to generate different frame patterns to exercise various TX type decisions
static void generate_frame_pattern(uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane,
                                   int width, int height, int y_stride, int uv_stride,
                                   int pattern_type, FuzzedDataProvider& fdp) {
    // Clear planes
    memset(y_plane, 128, y_stride * height);
    memset(u_plane, 128, uv_stride * (height / 2));
    memset(v_plane, 128, uv_stride * (height / 2));
    
    switch (pattern_type % 4) {
        case 0: // Smooth gradients (favor DCT transforms)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Create smooth gradient
                    int gradient_x = (x * 255) / std::max(1, width);
                    int gradient_y = (y * 255) / std::max(1, height);
                    int value = (gradient_x + gradient_y) / 2;
                    y_plane[y * y_stride + x] = (uint8_t)value;
                }
            }
            // Chroma planes with smoother gradients
            for (int y = 0; y < height / 2; y++) {
                for (int x = 0; x < width / 2; x++) {
                    int gradient = (x * 255) / std::max(1, width / 2);
                    u_plane[y * uv_stride + x] = (uint8_t)(128 + gradient / 4);
                    v_plane[y * uv_stride + x] = (uint8_t)(128 - gradient / 4);
                }
            }
            break;
            
        case 1: // Directional patterns (favor ADST, FLIPADST transforms)
            {
                int angle = fdp.ConsumeIntegralInRange<int>(0, 360);
                float rad = angle * M_PI / 180.0f;
                float cos_a = cos(rad);
                float sin_a = sin(rad);
                
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        // Create directional stripes
                        float pos = (x * cos_a + y * sin_a) / 10.0f;
                        int value = 128 + (int)(127 * sin(pos));
                        y_plane[y * y_stride + x] = (uint8_t)std::max(0, std::min(255, value));
                    }
                }
                
                // Chroma with different directional patterns
                for (int y = 0; y < height / 2; y++) {
                    for (int x = 0; x < width / 2; x++) {
                        float pos_u = (x * sin_a - y * cos_a) / 8.0f;
                        float pos_v = (x * cos_a + y * sin_a) / 12.0f;
                        u_plane[y * uv_stride + x] = (uint8_t)(128 + (int)(64 * sin(pos_u)));
                        v_plane[y * uv_stride + x] = (uint8_t)(128 + (int)(64 * sin(pos_v)));
                    }
                }
            }
            break;
            
        case 2: // Random noise (exercise full TX type search)
            {
                // Generate different random seeds for different blocks to create varied patterns
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        // Create block-based randomness to exercise sorting network equality cases
                        int block_x = x / 8;
                        int block_y = y / 8;
                        uint32_t seed = (block_x * 37 + block_y * 41) % 256;
                        int value = 128 + ((seed + x + y) % 129) - 64;
                        y_plane[y * y_stride + x] = (uint8_t)std::max(0, std::min(255, value));
                    }
                }
                
                // Chroma with different random patterns
                for (int y = 0; y < height / 2; y++) {
                    for (int x = 0; x < width / 2; x++) {
                        int block_x = x / 4;
                        int block_y = y / 4;
                        uint32_t seed_u = (block_x * 31 + block_y * 43) % 256;
                        uint32_t seed_v = (block_x * 47 + block_y * 53) % 256;
                        u_plane[y * uv_stride + x] = (uint8_t)(128 + ((seed_u + x + y) % 97) - 48);
                        v_plane[y * uv_stride + x] = (uint8_t)(128 + ((seed_v + x + y) % 97) - 48);
                    }
                }
            }
            break;
            
        case 3: // Repeating patterns (test equality cases in sorting network)
            {
                int pattern_size = fdp.ConsumeIntegralInRange<int>(2, 16);
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        // Create repeating patterns that generate equal or nearly equal values
                        int pattern_x = x % pattern_size;
                        int pattern_y = y % pattern_size;
                        int value = 64 + ((pattern_x * pattern_y) % 192);
                        
                        // Create clusters of equal values to test sorting network equality branch
                        if ((x / pattern_size + y / pattern_size) % 4 == 0) {
                            value = 128; // Many equal values
                        }
                        y_plane[y * y_stride + x] = (uint8_t)value;
                    }
                }
                
                // Chroma with different repeating patterns
                for (int y = 0; y < height / 2; y++) {
                    for (int x = 0; x < width / 2; x++) {
                        int pattern_x = x % (pattern_size / 2);
                        int pattern_y = y % (pattern_size / 2);
                        u_plane[y * uv_stride + x] = (uint8_t)(128 + ((pattern_x * 7 + pattern_y * 11) % 64) - 32);
                        v_plane[y * uv_stride + x] = (uint8_t)(128 + ((pattern_x * 13 + pattern_y * 17) % 64) - 32);
                    }
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for encoder configuration and at least some frame data
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(64, 320);
    int height = fdp.ConsumeIntegralInRange<int>(64, 240);
    width = (width + 7) & ~7;  // Align to multiple of 8
    height = (height + 7) & ~7;
    
    int bit_depth = fdp.ConsumeBool() ? 8 : 10;
    int usage = fdp.ConsumeIntegralInRange<int>(0, 3); // AOM_USAGE_GOOD_QUALITY=0, REALTIME=1, etc.
    
    // Get encoder interface
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) return 0;
    
    // Set basic configuration from fuzzer input
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = (aom_bit_depth_t)bit_depth;
    cfg.g_input_bit_depth = bit_depth;
    cfg.rc_end_usage = AOM_Q; // Constant quality mode
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<int>(cfg.rc_min_quantizer, 63);
    cfg.g_threads = fdp.ConsumeIntegralInRange<int>(1, 4);
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<int>(0, 25);
    
    // Initialize encoder
    aom_codec_ctx_t codec;
    res = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        // Try with simpler configuration
        cfg.g_lag_in_frames = 0;
        res = aom_codec_enc_init(&codec, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) return 0;
    }
    
    // Set encoder parameters that affect TX type search
    // Enable extended transform types
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX64, fdp.ConsumeBool());
    aom_codec_control(&codec, AV1E_SET_ENABLE_FLIP_IDTX, fdp.ConsumeBool());
    aom_codec_control(&codec, AV1E_SET_ENABLE_RECT_TX, fdp.ConsumeBool());
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, fdp.ConsumeBool());
    
    // Set quantization parameters to trigger different RD decisions
    aom_codec_control(&codec, AOME_SET_CQ_LEVEL, fdp.ConsumeIntegralInRange<int>(0, 63));
    aom_codec_control(&codec, AV1E_SET_QUANT_B_ADAPT, fdp.ConsumeBool());
    
    // Set TX type search parameters
    aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, fdp.ConsumeBool());
    aom_codec_control(&codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, fdp.ConsumeBool());
    
    // Allocate image buffer
    aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 16);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }
    
    // Generate and encode multiple frames with different patterns
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int frame_idx = 0; frame_idx < num_frames && fdp.remaining_bytes() > 100; frame_idx++) {
        // Generate frame with specific pattern to exercise different TX type decisions
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        generate_frame_pattern(img->planes[AOM_PLANE_Y], img->planes[AOM_PLANE_U], img->planes[AOM_PLANE_V],
                              width, height, img->stride[AOM_PLANE_Y], img->stride[AOM_PLANE_U],
                              pattern_type, fdp);
        
        // Set image format
        img->fmt = AOM_IMG_FMT_I420;
        img->bit_depth = bit_depth;
        img->d_w = width;
        img->d_h = height;
        img->r_w = width;
        img->r_h = height;
        img->x_chroma_shift = 1;
        img->y_chroma_shift = 1;
        
        // Encode frame
        aom_codec_encode(&codec, img, frame_idx, 1, 0);
        
        // Get compressed data (not used, but completes the encoding cycle)
        const aom_codec_cx_pkt_t* pkt = NULL;
        aom_codec_iter_t iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just iterate through packets to complete encoding
        }
    }
    
    // Flush encoder
    while (aom_codec_encode(&codec, NULL, -1, 1, 0) == AOM_CODEC_OK) {
        const aom_codec_cx_pkt_t* pkt = NULL;
        aom_codec_iter_t iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just iterate through packets
        }
    }
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_008
