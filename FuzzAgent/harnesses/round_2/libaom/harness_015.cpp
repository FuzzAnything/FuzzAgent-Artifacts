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

namespace harness_015 {
/*
 * Fuzzing harness for libaom AV1 encoder API with 0% coverage
 * Specifically targets aom/aom_codec.h and complete encoder workflow
 * 
 * Coverage guidance focus: aom/aom_codec.h has 0% coverage (1 region, 0% coverage)
 * Critical gap in AV1 encoder API coverage including:
 * - aom_codec_enc_init_ver(): Encoder initialization
 * - aom_codec_encode(): Main encoding function  
 * - aom_codec_get_cx_data(): Retrieve encoded data
 * - aom_codec_destroy(): Cleanup
 * 
 * Supporting APIs needed for full workflow:
 * - aom_codec_enc_config_default(): Get default config
 * - aom_codec_enc_config_set(): Set encoder configuration
 * - aom_img_alloc(): Image allocation for input frames
 * - aom_img_free(): Image cleanup
 * 
 * Advanced control APIs to test:
 * - aom_codec_control() with AOME_SET_CPUUSED
 * - aom_codec_control() with AV1E_SET_ENABLE_TPL_MODEL
 * Target API call chain: Full encoder workflow
 * 1. Configuration: aom_codec_enc_config_default -> aom_codec_enc_config_set
 * 2. Initialization: aom_codec_enc_init_ver
 * 3. Encoding: aom_img_alloc -> fill frames -> aom_codec_encode -> aom_codec_get_cx_data loop
 * 4. Cleanup: aom_img_free -> aom_codec_destroy
 * 
 * Different from existing harnesses (000-014):
 * 1. Focus on complete encoder workflow (aom_codec_encode + aom_codec_get_cx_data)
 * 2. Tests actual frame encoding with variable input data
 * 3. Implements proper packet retrieval loop from encoder
 * 4. Tests error conditions with invalid configurations
 * 5. Uses advanced encoder controls (CPU_USED, TPL_MODEL)
 * 6. Exercises aom/aom_codec.h APIs that have 0% coverage
 * 7. Includes memory leak detection through proper resource cleanup
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: encoder config + frame dimensions + minimal frame data
    const size_t MIN_INPUT_SIZE = sizeof(uint16_t) * 10 + 128; // Config + small frame
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful encoder workflow testing
    }

    FuzzedDataProvider fdp(data, size);

    // ========== Phase 1: Consume Encoder Configuration Parameters ==========
    // Fixed-size parameters first
    uint8_t usage_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: GOOD_QUALITY, 1: REALTIME, 2: ALL_INTRA
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
    uint8_t profile = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_highbitdepth = fdp.ConsumeBool();
    
    // Advanced encoder control parameters
    bool test_invalid_config = fdp.ConsumeBool();  // Test error paths
    uint8_t cpu_used = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);  // AOME_SET_CPUUSED range
    bool enable_tpl_model = fdp.ConsumeBool();  // AV1E_SET_ENABLE_TPL_MODEL
    bool test_null_pointers = fdp.ConsumeBool();  // Test error handling with NULL
    
    // Frame encoding parameters
    // Frame encoding parameters
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);  // Test single and multi-frame
    // Rate control parameters
    uint8_t rc_mode = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: CQ, 1: Q, 2: VBR
    uint32_t bitrate = fdp.ConsumeIntegralInRange<uint32_t>(10000, 10000000);
    uint8_t cq_level = fdp.ConsumeIntegralInRange<uint8_t>(0, 63);
    
    // Map usage type to libaom constants
    unsigned int usage;
    switch (usage_type) {
        case 0: usage = AOM_USAGE_GOOD_QUALITY; break;
        case 1: usage = AOM_USAGE_REALTIME; break;
        case 2: usage = AOM_USAGE_ALL_INTRA; break;
        default: usage = AOM_USAGE_GOOD_QUALITY; break;
    }
    
    // Map bit depth
    aom_bit_depth_t bit_depth = use_highbitdepth ? AOM_BITS_10 : AOM_BITS_8;
    
    // Get remaining data for frame content
    std::vector<uint8_t> frame_data = fdp.ConsumeRemainingBytes<uint8_t>();

    // ========== Phase 2: Test Error Conditions ==========
    if (test_null_pointers) {
        // Test aom_codec_enc_init_ver with NULL pointers (should fail gracefully)
        aom_codec_err_t err = aom_codec_enc_init_ver(nullptr, nullptr, nullptr, 0, AOM_ENCODER_ABI_VERSION);
        // Error expected, continue testing
    }

    // ========== Phase 3: Encoder Initialization ==========
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }

    // Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t cfg_err = aom_codec_enc_config_default(encoder_iface, &cfg, usage);
    if (cfg_err != AOM_CODEC_OK) {
        return 0;
    }

    // Configure encoder with fuzzed parameters
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.g_profile = profile;
    cfg.g_bit_depth = bit_depth;
    cfg.g_input_bit_depth = use_highbitdepth ? 10 : 8;
    
    // Set rate control based on fuzzed parameters
    switch (rc_mode) {
        case 0:  // Constant Quality
            cfg.rc_end_usage = AOM_CQ;
            cfg.rc_min_quantizer = cq_level;
            cfg.rc_max_quantizer = cq_level;
            break;
        case 1:  // Constant Quantizer
            cfg.rc_end_usage = AOM_Q;
            cfg.rc_min_quantizer = cq_level;
            cfg.rc_max_quantizer = cq_level;
            break;
        case 2:  // Variable Bitrate
            cfg.rc_end_usage = AOM_VBR;
            cfg.rc_target_bitrate = bitrate;
            break;
    }
    // Test invalid configuration if requested
    if (test_invalid_config) {
        // Create invalid configuration to test error paths
        cfg.g_w = 0;  // Invalid width
        cfg.g_h = 0;  // Invalid height
    }

    // Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_err_t init_err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // If initialization failed with invalid config, try with valid config
    if (init_err != AOM_CODEC_OK && test_invalid_config) {
        // Restore valid configuration and retry
        cfg.g_w = width;
        cfg.g_h = height;
        init_err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    }
    
    if (init_err != AOM_CODEC_OK) {
        return 0;  // Encoder initialization failed
    }

    // ========== Phase 4: Advanced Encoder Controls ==========
    // Set CPU usage parameter (speed/quality tradeoff)
    if (cpu_used <= 10) {  // Valid range for AOME_SET_CPUUSED
        aom_codec_control(&encoder, AOME_SET_CPUUSED, cpu_used);
    }
    
    // Enable/disable temporal dependency model
    aom_codec_control(&encoder, AV1E_SET_ENABLE_TPL_MODEL, enable_tpl_model ? 1 : 0);
    
    // ========== Phase 5: Frame Encoding Loop ==========
    for (uint8_t frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Allocate image buffer for input frame
        aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, width, height, 16);
        if (!img) {
            aom_codec_destroy(&encoder);
            return 0;
        }
        
        // Fill image planes with fuzzer data
        // For simplicity, we'll fill with pattern based on fuzzer input
        size_t data_offset = (frame_idx * width * height * 3 / 2) % frame_data.size();
        size_t bytes_to_use = std::min(width * height * 3 / 2, static_cast<int>(frame_data.size() - data_offset));
        
        if (bytes_to_use > 0) {
            // Fill Y plane (luma)
            size_t y_size = width * height;
            if (y_size > 0 && data_offset < frame_data.size()) {
                size_t copy_y = std::min(y_size, frame_data.size() - data_offset);
                memcpy(img->planes[AOM_PLANE_Y], frame_data.data() + data_offset, copy_y);
                data_offset += copy_y;
            }
            
            // Fill U and V planes (chroma) - half resolution
            size_t uv_size = (width / 2) * (height / 2);
            if (uv_size > 0 && data_offset < frame_data.size()) {
                size_t copy_uv = std::min(uv_size, frame_data.size() - data_offset);
                memcpy(img->planes[AOM_PLANE_U], frame_data.data() + data_offset, copy_uv);
                data_offset += copy_uv;
            }
            
            if (uv_size > 0 && data_offset < frame_data.size()) {
                size_t copy_uv = std::min(uv_size, frame_data.size() - data_offset);
                memcpy(img->planes[AOM_PLANE_V], frame_data.data() + data_offset, copy_uv);
            }
        }
        
        // Encode the frame
        aom_codec_err_t encode_err = aom_codec_encode(&encoder, img, frame_idx, 1, 0);
        
        // Free the input image
        aom_img_free(img);
        
        if (encode_err != AOM_CODEC_OK) {
            // Encoding error, but continue to cleanup
            break;
        }
        
        // ========== Phase 6: Retrieve Encoded Packets ==========
        // This is the critical API from aom/aom_codec.h that has 0% coverage
        const aom_codec_cx_pkt_t* pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Process different packet types
            switch (pkt->kind) {
                case AOM_CODEC_CX_FRAME_PKT:
                    // Encoded frame packet - validate basic properties
                    if (pkt->data.frame.pts >= 0 && pkt->data.frame.sz > 0) {
                        // Packet has valid data, nothing more to do for fuzzing
                    }
                    break;
                    
                case AOM_CODEC_STATS_PKT:
                    // Statistics packet
                    break;
                    
                case AOM_CODEC_PSNR_PKT:
                    // PSNR packet
                    break;
                    
                default:
                    // Other packet types
                    break;
            }
        }
        
        // If this is the last frame, flush the encoder
        if (frame_idx == num_frames - 1) {
            aom_codec_encode(&encoder, nullptr, frame_idx, 1, 0);
            
            // Retrieve any remaining packets
            iter = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                // Process flushed packets
            }
        }
    }

    // ========== Phase 7: Cleanup ==========
    // This exercises aom_codec_destroy() which is part of the 0% coverage API
    aom_codec_err_t destroy_err = aom_codec_destroy(&encoder);
    
    // Note: We don't check destroy_err because aom_codec_destroy() returns void
    // in some versions, but we call it for completeness

    return 0;
}

} // namespace harness_015
