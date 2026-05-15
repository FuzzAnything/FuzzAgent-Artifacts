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
#include "aom/aom_ext_ratectrl.h"

namespace harness_009 {
/*
 * Fuzzing harness for libaom AV1 rate control and quality optimization subsystem
 * Targets uncovered rate control functions identified by CoverageAnalyzerAgent:
 * - av1_extrc_create (0% covered) - external rate control creation
 * - validate_config (271 blocked branches) - encoder configuration validation  
 * - encoder_set_option (177 blocked branches) - encoder option setting
 * Focus on rate control algorithms, bitrate allocation, quantization parameter selection, and quality metrics
 * Targets numerical overflow bugs in rate control computations similar to previously found bugs
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for rate control testing: various options + image data
    const size_t MIN_INPUT_SIZE = 128;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful rate control testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first for encoder configuration
    uint8_t usage_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: GOOD_QUALITY, 1: REALTIME, 2: ALL_INTRA
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(64, 512);
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(64, 512);
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
    uint8_t profile = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t bit_depth_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    bool use_highbitdepth = fdp.ConsumeBool();
    
    // Rate control specific parameters - critical for targeting rate control subsystem
    uint8_t rc_mode_choice = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: CBR, 1: VBR, 2: CQ, 3: Q
    uint32_t target_bitrate = fdp.ConsumeIntegralInRange<uint32_t>(1, 1000000);  // 1 bps to 1 Mbps
    uint8_t cq_level = fdp.ConsumeIntegralInRange<uint8_t>(0, 63);  // Valid CQ level range
    bool lossless_mode = fdp.ConsumeBool();
    uint8_t tune_choice = fdp.ConsumeIntegral<uint8_t>() % 8;  // Different tune values
    uint8_t arnr_strength = fdp.ConsumeIntegralInRange<uint8_t>(0, 6);  // Valid range 0-6
    
    // Advanced rate control parameters
    uint8_t aq_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // Adaptive quantization mode
    uint8_t frame_boost = fdp.ConsumeBool();  // Frame boost enabled/disabled
    uint8_t min_q = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
    uint8_t max_q = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
    if (min_q > max_q) {
        std::swap(min_q, max_q);
    }
    
    // External rate control parameters
    bool use_external_rc = fdp.ConsumeBool();
    uint8_t external_rc_type = fdp.ConsumeIntegral<uint8_t>() % 5;  // Different external RC types
    
    // Number of encoder options to set - test encoder_set_option extensively
    uint8_t num_options = fdp.ConsumeIntegralInRange<uint8_t>(5, 20);
    
    // Map usage type to actual values
    unsigned int usage;
    switch (usage_type) {
        case 0: usage = AOM_USAGE_GOOD_QUALITY; break;
        case 1: usage = AOM_USAGE_REALTIME; break;
        case 2: usage = AOM_USAGE_ALL_INTRA; break;
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
    
    // Map rate control mode
    aom_rc_mode rc_mode;
    switch (rc_mode_choice) {
        case 0: rc_mode = AOM_CBR; break;
        case 1: rc_mode = AOM_VBR; break;
        case 2: rc_mode = AOM_CQ; break;
        case 3: rc_mode = AOM_Q; break;
        default: rc_mode = AOM_VBR; break;
    }

    // Get encoder interface
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }

    // Step 1: Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(encoder_iface, &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    // Modify configuration with fuzzed values
    cfg.g_usage = usage;
    cfg.g_threads = threads;
    cfg.g_profile = profile;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_input_bit_depth = use_highbitdepth ? (bit_depth == AOM_BITS_8 ? 10 : bit_depth) : 8;
    
    // Set rate control parameters in configuration
    cfg.rc_end_usage = rc_mode;
    cfg.rc_target_bitrate = target_bitrate / 1000;  // Convert to kbps
    cfg.rc_min_quantizer = min_q;
    cfg.rc_max_quantizer = max_q;
    cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    cfg.rc_buf_sz = fdp.ConsumeIntegralInRange<unsigned int>(1, 10000);
    cfg.rc_buf_initial_sz = fdp.ConsumeIntegralInRange<unsigned int>(1, cfg.rc_buf_sz);
    cfg.rc_buf_optimal_sz = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_buf_initial_sz, cfg.rc_buf_sz);
    
    if (lossless_mode) {
        cfg.rc_end_usage = AOM_Q;
        cfg.rc_min_quantizer = 0;
        cfg.rc_max_quantizer = 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_flags_t flags = use_highbitdepth ? AOM_CODEC_USE_HIGHBITDEPTH : 0;
    
    // Step 2: Initialize encoder with AV1 interface
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, flags, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Step 3: Set encoder options via key-value pairs for rate control
    // Comprehensive list of rate control related options
    const char* rate_control_option_names[] = {
        "target-bitrate", "end-usage", "cq-level", "lossless", 
        "arnr-strength", "tune", "aq-mode", "frame-boost",
        "min-q", "max-q", "undershoot-pct", "overshoot-pct",
        "buf-sz", "buf-initial-sz", "buf-optimal-sz", "bias-pct",
        "minsection-pct", "maxsection-pct", "arnr-maxframes",
        "cpu-used", "auto-alt-ref", "lag-in-frames", "drop-frame",
        "static-thresh", "noise-sens", "sharpness", "enable-cdef",
        "enable-restoration", "enable-global-motion", "enable-warped-motion",
        "enable-obmc", "enable-palette", "enable-intrabc", "enable-flip-idtx",
        "enable-rect-tx", "enable-tx64", "enable-filter-intra", "enable-smooth-intra",
        "enable-dual-filter", "enable-diff-wtd-comp", "enable-dist-wtd-comp",
        "enable-interintra-wedge", "enable-interintra-comp", "enable-masked-comp",
        "enable-one-sided-comp", "enable-interinter-wedge", "enable-interinter-comp"
    };
    const size_t num_option_names = sizeof(rate_control_option_names) / sizeof(rate_control_option_names[0]);

    for (uint8_t i = 0; i < num_options && fdp.remaining_bytes() > 2; i++) {
        // Consume option name index
        uint8_t name_idx = fdp.ConsumeIntegral<uint8_t>() % num_option_names;
        const char* name = rate_control_option_names[name_idx];
        
        // Consume option value as string - varying lengths to test validation
        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        if (fdp.remaining_bytes() < value_len) {
            value_len = fdp.remaining_bytes();
        }
        if (value_len == 0) break;
        
        std::string value = fdp.ConsumeBytesAsString(value_len);
        
        // Set the option - this exercises encoder_set_option with various inputs
        aom_codec_set_option(&encoder, name, value.c_str());
    }

    // Step 4: Test external rate control if enabled
    if (use_external_rc && fdp.remaining_bytes() > 64) {
        // Create dummy external rate control functions structure
        aom_rc_funcs_t rc_funcs;
        memset(&rc_funcs, 0, sizeof(rc_funcs));
        
        // Set external rate control type
        aom_rc_type_t rc_type;
        switch (external_rc_type) {
            case 0: rc_type = AOM_RC_NONE; break;
            case 1: rc_type = AOM_RC_QP; break;
            case 2: rc_type = AOM_RC_GOP; break;
            case 3: rc_type = AOM_RC_RDMULT; break;
            case 4: rc_type = AOM_RC_GOP_QP_RDMULT; break;
            default: rc_type = AOM_RC_NONE; break;
        }
        rc_funcs.rc_type = rc_type;
        
        // Try to set external rate control via encoder control
        // This will test AV1E_SET_EXTERNAL_RATE_CONTROL code path
        aom_codec_control(&encoder, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
    }

    // Step 5: Set specific rate control related controls using aom_codec_control
    if (tune_choice < 8) {
        int tune_value;
        switch (tune_choice) {
            case 0: tune_value = AOM_TUNE_PSNR; break;
            case 1: tune_value = AOM_TUNE_SSIM; break;
            case 2: tune_value = AOM_TUNE_VMAF_WITH_PREPROCESSING; break;
            case 3: tune_value = AOM_TUNE_VMAF_WITHOUT_PREPROCESSING; break;
            case 4: tune_value = AOM_TUNE_VMAF_MAX_GAIN; break;
            case 5: tune_value = AOM_TUNE_VMAF_NEG_MAX_GAIN; break;
            case 6: tune_value = AOM_TUNE_BUTTERAUGLI; break;
            case 7: tune_value = AOM_TUNE_VMAF_SALIENCY_MAP; break;
            default: tune_value = AOM_TUNE_PSNR; break;
        }
        aom_codec_control(&encoder, AOME_SET_TUNING, tune_value);
    }
    
    // Set CQ level if in CQ mode
    if (rc_mode == AOM_CQ || rc_mode == AOM_Q) {
        aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, cq_level);
    }
    
    // Set ARNR strength
    aom_codec_control(&encoder, AOME_SET_ARNR_STRENGTH, arnr_strength);

    // Step 6: Allocate test image for encoding
    aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 16);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image with some test data from fuzzer input
    if (fdp.remaining_bytes() > 0) {
        size_t bytes_to_fill = std::min(fdp.remaining_bytes(), 
                                       static_cast<size_t>(width * height * 3 / 2));
        std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(bytes_to_fill);
        
        // Simple fill - just to have some data for encoding
        for (size_t i = 0; i < bytes_to_fill && i < static_cast<size_t>(width * height * 3 / 2); i++) {
            uint8_t* img_ptr = reinterpret_cast<uint8_t*>(img->planes[0]);
            img_ptr[i % (width * height)] = img_data[i % img_data.size()];
        }
    }

    // Step 7: Encode multiple frames with different flags to test rate control
    // This exercises the rate control algorithms during actual encoding
    int num_frames_to_encode = std::min(static_cast<int>(fdp.ConsumeIntegralInRange<uint8_t>(1, 5)), 3);
    
    for (int frame_idx = 0; frame_idx < num_frames_to_encode; frame_idx++) {
        aom_codec_flags_t encode_flags = 0;
        
        // Vary flags for different frames to test rate control adaptation
        if (frame_idx == 0) {
            // First frame - could be keyframe, use 0 or other valid flags
            encode_flags = 0;  // Normal encoding for first frame
        } else if (frame_idx % 2 == 0) {
            // Alternate reference flags to test different encoding paths
            // Use a combination of valid flags from aomcx.h
            encode_flags = AOM_EFLAG_NO_REF_LAST | AOM_EFLAG_NO_REF_LAST2 | 
                          AOM_EFLAG_NO_REF_LAST3 | AOM_EFLAG_NO_REF_GF;
        }
        
        // Encode the frame
        if (aom_codec_encode(&encoder, img, frame_idx, 1, encode_flags) != AOM_CODEC_OK) {
            // Continue even if encoding fails - we want to test error paths too
        }
        
        // Retrieve encoded data to exercise rate control feedback
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t* pkt = nullptr;
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Process different packet types
            switch (pkt->kind) {
                case AOM_CODEC_CX_FRAME_PKT:
                    // Frame data available - rate control uses this for feedback
                    break;
                case AOM_CODEC_STATS_PKT:
                    // Stats packet - contains rate control statistics
                    break;
                case AOM_CODEC_PSNR_PKT:
                    // PSNR data - quality metrics for rate control
                    break;
                default:
                    // Other packet types
                    break;
            }
        }
    }

    // Step 8: Test additional rate control controls with remaining fuzzer data
    if (fdp.remaining_bytes() > 4) {
        // Test setting maximum intra bitrate percentage (this control exists)
        uint8_t max_intra_bitrate_pct = fdp.ConsumeIntegralInRange<uint8_t>(0, 200);
        aom_codec_control(&encoder, AOME_SET_MAX_INTRA_BITRATE_PCT, max_intra_bitrate_pct);
        
        // Test other controls that exist and are safe
        uint8_t sharpness = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
        aom_codec_control(&encoder, AOME_SET_SHARPNESS, sharpness);
        
        uint8_t static_threshold = fdp.ConsumeIntegral<uint8_t>();
        aom_codec_control(&encoder, AOME_SET_STATIC_THRESHOLD, static_threshold);
        
        uint8_t arnr_maxframes = fdp.ConsumeIntegralInRange<uint8_t>(0, 15);
        aom_codec_control(&encoder, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }

    // Step 9: Clean up
    aom_img_free(img);
    aom_codec_destroy(&encoder);

    return 0;
}

} // namespace harness_009
