#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

namespace harness_011 {
/*
 * Fuzzing harness for libaom AV1 encoder transform search path targeting av1_sort_fi32_16
 * Specifically uses aom_codec_control() with transform search settings to break through
 * the av1_sort_fi32_16 branch blocker (585/650 branches blocked)
 * 
 * Coverage guidance analysis: Need to target encoder transform search path through
 * aom_codec_encode and related control APIs with specific aom_codec_control settings:
 * - AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * - AV1E_SET_ENABLE_ORDER_HINT  
 * - AV1E_SET_ENABLE_OBMC
 * - AV1E_SET_ENABLE_PALETTE
 * 
 * Different from harness_010 which uses aom_codec_set_option(), this harness uses
 * the lower-level aom_codec_control() API as specified in coverage guidance.
 * 
 * Target API call chain: aom_codec_encode → encoder_encode → av1_get_compressed_data → 
 * av1_encode_strategy → ... → search_tx_type → get_tx_mask → prune_tx_2D → av1_sort_fi32_16
 * 
 * Strategy:
 * 1. Use aom_codec_enc_init_ver() and aom_codec_enc_config_default() for proper initialization
 * 2. Set transform search controls via aom_codec_control() with specific settings
 * 3. Create input frames with diverse texture patterns (smooth, edges, noise, checkerboard)
 * 4. Use AOM_USAGE_GOOD_QUALITY with AOM_CQ mode for extensive transform search
 * 5. Test with both enabled and disabled transform search controls
 * 6. Include proper cleanup with aom_img_free() and aom_codec_destroy()
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: encoder config + control parameters + minimal image data
    const size_t MIN_INPUT_SIZE = sizeof(uint8_t) * 16 + 64;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful transform search testing
    }

    FuzzedDataProvider fdp(data, size);

    // ========== Phase 1: Encoder Configuration Parameters ==========
    // Consume fixed-size parameters first
    uint8_t usage_type = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0: GOOD_QUALITY, 1: REALTIME
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(64, 256);
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(64, 256);
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
    uint8_t profile = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t bit_depth_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    bool use_highbitdepth = fdp.ConsumeBool();
    
    // Transform search control parameters - using aom_codec_control() as per guidance
    bool enable_tx_size_search = fdp.ConsumeBool();
    bool enable_order_hint = fdp.ConsumeBool();
    bool enable_obmc = fdp.ConsumeBool();
    bool enable_palette = fdp.ConsumeBool();
    
    // Additional transform-related controls
    bool enable_tx64 = fdp.ConsumeBool();
    bool enable_rect_tx = fdp.ConsumeBool();
    bool enable_flip_idtx = fdp.ConsumeBool();
    
    // CQ level for transform type pruning (affects search depth)
    uint8_t cq_level = fdp.ConsumeIntegralInRange<uint8_t>(20, 50);
    
    // Texture pattern for test frames
    uint8_t texture_pattern = fdp.ConsumeIntegral<uint8_t>() % 5;  // 0: smooth, 1: edges, 2: noise, 3: checkerboard, 4: gradient
    
    // Number of frames to encode
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(1, 3);
    
    // Map usage type
    unsigned int usage;
    switch (usage_type) {
        case 0: usage = AOM_USAGE_GOOD_QUALITY; break;
        case 1: usage = AOM_USAGE_REALTIME; break;
        default: usage = AOM_USAGE_GOOD_QUALITY; break;
    }
    
    // Map bit depth
    aom_bit_depth_t bit_depth;
    switch (bit_depth_choice) {
        case 0: bit_depth = AOM_BITS_8; break;
        case 1: bit_depth = AOM_BITS_10; break;
        case 2: bit_depth = AOM_BITS_12; break;
        default: bit_depth = AOM_BITS_8; break;
    }

    // ========== Phase 2: Encoder Initialization ==========
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }

    // Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(encoder_iface, &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    // Configure encoder
    cfg.g_usage = usage;
    cfg.g_threads = threads;
    cfg.g_profile = profile;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    
    // Set CQ mode for transform type pruning
    cfg.rc_end_usage = AOM_CQ;
    cfg.g_lag_in_frames = 0;  // Intra-only to simplify testing
    
    aom_codec_ctx_t encoder;
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // ========== Phase 3: Set Transform Search Controls via aom_codec_control() ==========
    // Set CQ level using control interface
    aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, cq_level);
    
    // Set transform search controls as specified in guidance
    aom_codec_control(&encoder, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search ? 1 : 0);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_ORDER_HINT, enable_order_hint ? 1 : 0);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_OBMC, enable_obmc ? 1 : 0);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_PALETTE, enable_palette ? 1 : 0);
    
    // Additional transform-related controls
    if (enable_tx64) {
        aom_codec_control(&encoder, AV1E_SET_ENABLE_TX64, 1);
    }
    if (enable_rect_tx) {
        aom_codec_control(&encoder, AV1E_SET_ENABLE_RECT_TX, 1);
    }
    if (enable_flip_idtx) {
        aom_codec_control(&encoder, AV1E_SET_ENABLE_FLIP_IDTX, 1);
    }
    
    // Set other relevant controls for transform search
    aom_codec_control(&encoder, AV1E_SET_ENABLE_CFL_INTRA, 1);  // Enable chroma-from-luma
    aom_codec_control(&encoder, AV1E_SET_ENABLE_SMOOTH_INTRA, 1);  // Enable smooth intra
    aom_codec_control(&encoder, AV1E_SET_ENABLE_ANGLE_DELTA, 1);  // Enable angle delta
    
    // ========== Phase 4: Create Test Frames with Diverse Texture Patterns ==========
    // Calculate image buffer sizes
    size_t y_plane_size = width * height * (use_highbitdepth ? 2 : 1);
    size_t uv_width = (width + 1) / 2;
    size_t uv_height = (height + 1) / 2;
    size_t uv_plane_size = uv_width * uv_height * (use_highbitdepth ? 2 : 1);
    
    // Check if we have enough data for at least one frame
    if (fdp.remaining_bytes() < y_plane_size) {
        // Still test encoder initialization, controls, and cleanup
        aom_codec_encode(&encoder, NULL, 0, 0, 0);  // Flush encoder
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Generate test frames with different texture patterns
    for (uint8_t frame_idx = 0; frame_idx < num_frames && fdp.remaining_bytes() >= y_plane_size; frame_idx++) {
        // Allocate image
        aom_image_t* img = nullptr;
        aom_img_fmt_t fmt = use_highbitdepth ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
        unsigned int align = 32;
        
        img = aom_img_alloc(nullptr, fmt, width, height, align);
        if (!img) {
            // Memory allocation failed - test error path
            break;
        }
        
        // Fill image with pattern based on texture_pattern
        if (fdp.remaining_bytes() >= y_plane_size) {
            std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
            
            // Fill Y plane with pattern
            for (size_t i = 0; i < y_plane_size && i < y_data.size(); i++) {
                uint8_t* plane_ptr = reinterpret_cast<uint8_t*>(img->planes[0]);
                size_t stride = img->stride[0];
                size_t row = i / width;
                size_t col = i % width;
                
                if (row < static_cast<size_t>(height) && col < static_cast<size_t>(width)) {
                    uint8_t pixel_value = y_data[i % y_data.size()];
                    
                    // Apply texture pattern modifications
                    switch (texture_pattern) {
                        case 0:  // Smooth - reduce high frequency
                            pixel_value = (pixel_value & 0xF0) | 0x08;
                            break;
                        case 1:  // Edges - enhance contrast
                            if (pixel_value > 128) pixel_value = 255;
                            else pixel_value = 0;
                            break;
                        case 2:  // Noise - random pattern
                            // Already random from fuzzed data
                            break;
                        case 3:  // Checkerboard pattern
                            if (((row / 8) + (col / 8)) % 2 == 0) {
                                pixel_value = 255;
                            } else {
                                pixel_value = 0;
                            }
                            break;
                        case 4:  // Gradient
                            pixel_value = static_cast<uint8_t>((col * 255) / width);
                            break;
                    }
                    
                    if (use_highbitdepth) {
                        uint16_t* plane_ptr_16 = reinterpret_cast<uint16_t*>(img->planes[0]);
                        plane_ptr_16[row * stride / 2 + col] = pixel_value << (bit_depth - 8);
                    } else {
                        plane_ptr[row * stride + col] = pixel_value;
                    }
                }
            }
            
            // Fill U and V planes with some data if available
            size_t remaining_for_uv = std::min(fdp.remaining_bytes(), uv_plane_size * 2);
            if (remaining_for_uv > 0) {
                std::vector<uint8_t> uv_data = fdp.ConsumeBytes<uint8_t>(remaining_for_uv);
                
                // Fill U plane
                for (size_t i = 0; i < uv_plane_size && i < uv_data.size() / 2; i++) {
                    size_t row = i / uv_width;
                    size_t col = i % uv_width;
                    
                    if (row < static_cast<size_t>(uv_height) && col < static_cast<size_t>(uv_width)) {
                        uint8_t pixel_value = uv_data[i % uv_data.size()];
                        
                        if (use_highbitdepth) {
                            uint16_t* plane_ptr_16 = reinterpret_cast<uint16_t*>(img->planes[1]);
                            plane_ptr_16[row * img->stride[1] / 2 + col] = pixel_value << (bit_depth - 8);
                        } else {
                            uint8_t* plane_ptr = img->planes[1];
                            plane_ptr[row * img->stride[1] + col] = pixel_value;
                        }
                    }
                }
                
                // Fill V plane
                for (size_t i = 0; i < uv_plane_size && i < uv_data.size() / 2; i++) {
                    size_t row = i / uv_width;
                    size_t col = i % uv_width;
                    
                    if (row < static_cast<size_t>(uv_height) && col < static_cast<size_t>(uv_width)) {
                        uint8_t pixel_value = uv_data[(i + uv_data.size() / 2) % uv_data.size()];
                        
                        if (use_highbitdepth) {
                            uint16_t* plane_ptr_16 = reinterpret_cast<uint16_t*>(img->planes[2]);
                            plane_ptr_16[row * img->stride[2] / 2 + col] = pixel_value << (bit_depth - 8);
                        } else {
                            uint8_t* plane_ptr = img->planes[2];
                            plane_ptr[row * img->stride[2] + col] = pixel_value;
                        }
                    }
                }
            }
            
            // Encode the frame
            aom_codec_encode(&encoder, img, frame_idx, 1, 0);
            
            // Get encoded data to exercise full pipeline
            aom_codec_iter_t iter = nullptr;
            const aom_codec_cx_pkt_t* pkt = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                // Packet available - this exercises the transform search path
                // No need to process the packet content for fuzzing
            }
        }
        
        // Free image for this frame
        aom_img_free(img);
    }
    
    // ========== Phase 5: Cleanup ==========
    // Flush any remaining frames
    aom_codec_encode(&encoder, NULL, 0, 0, 0);
    
    // Get any remaining encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process remaining packets
    }
    
    // Destroy encoder
    aom_codec_destroy(&encoder);
    
    return 0;
}

} // namespace harness_011
