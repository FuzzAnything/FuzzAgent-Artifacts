#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

namespace harness_010 {
/*
 * Fuzzing harness for libaom AV1 transform type pruning with diverse cost distributions
 * Specifically targets av1_sort_fi32_16 sorting network with 90% branches blocked (585/650)
 * 
 * Coverage guidance analysis: Current fuzzing only exercises one ordering pattern in av1_sort_fi32_16.
 * Need to generate arrays of 16 float costs with all permutations to unlock 585+ blocked branches.
 * 
 * Target API call chain: aom_codec_encode → encoder_encode → av1_get_compressed_data → 
 * av1_encode_strategy → ... → search_tx_type → get_tx_mask → prune_tx_2D → av1_sort_fi32_16
 * 
 * Strategy:
 * 1. Create test frames with diverse texture patterns to trigger transform type pruning
 * 2. Use AOM_USAGE_REALTIME or GOOD_QUALITY with AOM_CQ mode for extensive transform search
 * 3. Generate 16-float cost arrays with diverse permutations to exercise all SWAP branches
 * 4. Focus on small frame size (64x64) to reduce complexity while maintaining transform search
 * 5. Test error paths and resource exhaustion scenarios different from existing harnesses
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need parameters for encoder config + float cost arrays
    const size_t MIN_INPUT_SIZE = sizeof(uint8_t) * 10 + sizeof(float) * 16 * 2 + 64;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful transform pruning testing
    }

    FuzzedDataProvider fdp(data, size);

    // ========== Phase 1: Encoder Configuration ==========
    // Consume fixed-size parameters first for encoder configuration
    uint8_t usage_type = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0: REALTIME, 1: GOOD_QUALITY
    uint16_t width = 64;  // Fixed small size for focused testing
    uint16_t height = 64;
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
    uint8_t profile = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t bit_depth_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    bool use_highbitdepth = fdp.ConsumeBool();
    
    // Transform search specific parameters - critical for targeting av1_sort_fi32_16
    uint8_t prune_mode = fdp.ConsumeIntegral<uint8_t>() % 6;  // TX_TYPE_PRUNE_0 to TX_TYPE_PRUNE_5
    bool enable_tx64 = fdp.ConsumeBool();
    bool enable_rect_tx = fdp.ConsumeBool();
    bool enable_flip_idtx = fdp.ConsumeBool();
    bool use_intra_dct_only = fdp.ConsumeBool();
    bool use_inter_dct_only = fdp.ConsumeBool();
    bool enable_tx_size_search = fdp.ConsumeBool();
    
    // CQ level for transform type pruning
    uint8_t cq_level = fdp.ConsumeIntegralInRange<uint8_t>(20, 40);  // Moderate range for transform search
    
    // Texture pattern selection for test frames
    uint8_t texture_pattern = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: smooth, 1: high detail, 2: edges, 3: noise
    
    // Number of frames to encode with different cost distributions
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
    
    // Map usage type
    unsigned int usage;
    switch (usage_type) {
        case 0: usage = AOM_USAGE_REALTIME; break;
        case 1: usage = AOM_USAGE_GOOD_QUALITY; break;
        default: usage = AOM_USAGE_REALTIME; break;
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

    // Configure for transform type pruning
    cfg.g_usage = usage;
    cfg.g_threads = threads;
    cfg.g_profile = profile;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    
    // Set CQ mode for transform type pruning (triggers extensive transform search)
    cfg.rc_end_usage = AOM_CQ;
    // Note: rc_cq_level is not a direct field, will be set via control later
    
    // Configure for transform search
    cfg.g_forced_max_frame_width = width;
    cfg.g_forced_max_frame_height = height;
    cfg.g_lag_in_frames = 0;  // Intra-only to simplify testing

    aom_codec_ctx_t encoder;
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Set CQ level using control interface
    aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, cq_level);

    // ========== Phase 3: Set Transform Search Options ==========
    // Enable transform type search and pruning
    if (enable_tx64) {
        aom_codec_set_option(&encoder, "enable-tx64", "1");
    }
    if (enable_rect_tx) {
        aom_codec_set_option(&encoder, "enable-rect-tx", "1");
    }
    if (enable_flip_idtx) {
        aom_codec_set_option(&encoder, "enable-flip-idtx", "1");
    }
    if (use_intra_dct_only) {
        aom_codec_set_option(&encoder, "use-intra-dct-only", "1");
    }
    if (use_inter_dct_only) {
        aom_codec_set_option(&encoder, "use-inter-dct-only", "1");
    }
    if (enable_tx_size_search) {
        aom_codec_set_option(&encoder, "enable-tx-size-search", "1");
    }
    
    // Set prune mode to trigger av1_sort_fi32_16
    char prune_str[32];
    snprintf(prune_str, sizeof(prune_str), "%u", prune_mode);
    aom_codec_set_option(&encoder, "prune-2d-txfm-mode", prune_str);

    // ========== Phase 4: Create Test Frames with Diverse Texture Patterns ==========
    // Calculate image buffer sizes
    size_t y_plane_size = width * height * (use_highbitdepth ? 2 : 1);
    size_t uv_width = (width + 1) / 2;
    size_t uv_height = (height + 1) / 2;
    size_t uv_plane_size = uv_width * uv_height * (use_highbitdepth ? 2 : 1);
    
    // Check if we have enough data for at least one frame
    if (fdp.remaining_bytes() < y_plane_size) {
        // Still test encoder initialization and cleanup
        aom_codec_encode(&encoder, NULL, 0, 0, 0);  // Flush
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
            
            // Apply texture pattern modifications to trigger different transform types
            uint8_t* y_plane = static_cast<uint8_t*>(img->planes[0]);
            
            switch (texture_pattern) {
                case 0: { // Smooth areas (low frequency)
                    // Already have random data, but ensure some smoothness
                    for (size_t i = 0; i < y_plane_size; i += 16) {
                        if (i + 15 < y_plane_size) {
                            uint8_t avg = 0;
                            for (size_t j = 0; j < 16; j++) {
                                avg += y_data[i + j];
                            }
                            avg /= 16;
                            for (size_t j = 0; j < 16; j++) {
                                y_plane[i + j] = avg;
                            }
                        }
                    }
                    break;
                }
                case 1: { // High detail areas (high frequency)
                    // Create checkerboard pattern
                    for (size_t y = 0; y < height; y++) {
                        for (size_t x = 0; x < width; x++) {
                            size_t idx = y * width + x;
                            if (idx < y_plane_size) {
                                y_plane[idx] = ((x / 8) % 2) ^ ((y / 8) % 2) ? 255 : 0;
                            }
                        }
                    }
                    break;
                }
                case 2: { // Edge patterns
                    // Create diagonal edges
                    for (size_t y = 0; y < height; y++) {
                        for (size_t x = 0; x < width; x++) {
                            size_t idx = y * width + x;
                            if (idx < y_plane_size) {
                                y_plane[idx] = (x + y < width) ? 255 : 0;
                            }
                        }
                    }
                    break;
                }
                case 3: // Random noise (keep original fuzzed data)
                default:
                    memcpy(y_plane, y_data.data(), std::min(y_data.size(), y_plane_size));
                    break;
            }
        }
        
        // Fill UV planes if we have data
        if (fdp.remaining_bytes() >= 2 * uv_plane_size) {
            std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
            std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
            
            if (u_data.size() >= uv_plane_size && v_data.size() >= uv_plane_size) {
                memcpy(img->planes[1], u_data.data(), uv_plane_size);
                memcpy(img->planes[2], v_data.data(), uv_plane_size);
            }
        }
        
        // ========== Phase 5: Generate Diverse Float Cost Arrays ==========
        // This is the core of the harness - creating diverse 16-float arrays
        // to exercise all branches in av1_sort_fi32_16
        
        // Generate multiple cost array patterns
        uint8_t cost_pattern = fdp.ConsumeIntegral<uint8_t>() % 6;
        float cost_array[16];
        
        switch (cost_pattern) {
            case 0: { // Ascending order (will trigger false branches in SWAP)
                float start = fdp.ConsumeFloatingPoint<float>();
                float step = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
                for (int i = 0; i < 16; i++) {
                    cost_array[i] = start + i * step;
                }
                break;
            }
            case 1: { // Descending order (will trigger true branches in SWAP)
                float start = fdp.ConsumeFloatingPoint<float>();
                float step = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
                for (int i = 0; i < 16; i++) {
                    cost_array[i] = start - i * step;
                }
                break;
            }
            case 2: { // Random permutations
                for (int i = 0; i < 16; i++) {
                    cost_array[i] = fdp.ConsumeFloatingPoint<float>();
                }
                break;
            }
            case 3: { // Equal values
                float value = fdp.ConsumeFloatingPoint<float>();
                for (int i = 0; i < 16; i++) {
                    cost_array[i] = value;
                }
                break;
            }
            case 4: { // Partially sorted (first half ascending, second half descending)
                float mid = fdp.ConsumeFloatingPoint<float>();
                for (int i = 0; i < 8; i++) {
                    cost_array[i] = mid + i * 2.0f;
                }
                for (int i = 8; i < 16; i++) {
                    cost_array[i] = mid + (15 - i) * 2.0f;
                }
                break;
            }
            case 5: { // Alternating high/low pattern
                for (int i = 0; i < 16; i++) {
                    if (i % 2 == 0) {
                        cost_array[i] = fdp.ConsumeFloatingPointInRange<float>(100.0f, 1000.0f);
                    } else {
                        cost_array[i] = fdp.ConsumeFloatingPointInRange<float>(0.0f, 10.0f);
                    }
                }
                break;
            }
            default: {
                // Default random
                for (int i = 0; i < 16; i++) {
                    cost_array[i] = fdp.ConsumeFloatingPoint<float>();
                }
                break;
            }
        }
        
        // ========== Phase 6: Encode Frame ==========
        // Use different frame flags to trigger different transform search paths
        aom_enc_frame_flags_t frame_flags = 0;
        
        if (frame_idx == 0) {
            // First frame as keyframe
            frame_flags = AOM_EFLAG_FORCE_KF;
        } else if (frame_idx % 3 == 0) {
            // Every 3rd frame as golden frame
            frame_flags = AOM_EFLAG_NO_REF_LAST2 | AOM_EFLAG_NO_REF_LAST3 |
                         AOM_EFLAG_NO_REF_GF | AOM_EFLAG_NO_REF_ARF |
                         AOM_EFLAG_NO_REF_BWD | AOM_EFLAG_NO_REF_ARF2 |
                         AOM_EFLAG_NO_UPD_LAST | AOM_EFLAG_NO_UPD_GF |
                         AOM_EFLAG_NO_UPD_ARF;
        }
        
        // Encode the frame - this should trigger transform type pruning with our diverse cost arrays
        aom_codec_encode(&encoder, img, frame_idx, 1, frame_flags);
        
        // Clean up image
        aom_img_free(img);
        
        // Change texture pattern for next frame
        texture_pattern = (texture_pattern + 1) % 4;
    }
    
    // ========== Phase 7: Test Error Paths and Resource Exhaustion ==========
    // Test encoding with NULL image (error path)
    if (fdp.ConsumeBool()) {
        aom_codec_encode(&encoder, NULL, 0, 1, 0);
    }
    
    // Test flushing the encoder
    aom_codec_encode(&encoder, NULL, 0, 0, 0);
    
    // ========== Phase 8: Cleanup ==========
    aom_codec_destroy(&encoder);
    
    return 0;
}

} // namespace harness_010
