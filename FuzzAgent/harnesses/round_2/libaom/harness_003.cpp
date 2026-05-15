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

namespace harness_003 {
/*
 * Fuzzing harness for libaom AV1 encoder configuration APIs
 * Targets uncovered encoder configuration functions identified by CoverageAnalyzerAgent:
 * - aom_codec_enc_config_set (12 undiscovered branches)
 * - aom_codec_get_global_headers (10 undiscovered branches)
 * - aom_codec_get_preview_frame (10 undiscovered branches)
 * - aom_codec_set_option (8 undiscovered branches)
 * Lightweight configuration operations to improve coverage without heavy encoding
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Not enough data for meaningful configuration testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t usage_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: GOOD_QUALITY, 1: REALTIME, 2: ALL_INTRA
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 64);
    uint8_t profile = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t bit_depth_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    bool use_highbitdepth = fdp.ConsumeBool();
    uint8_t num_options = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    
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

    // Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_flags_t flags = use_highbitdepth ? AOM_CODEC_USE_HIGHBITDEPTH : 0;
    
    // Step 2: Initialize encoder with AV1 interface
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, flags, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Step 3: Apply custom configuration to encoder (reconfigure with potentially different settings)
    // Modify some configuration values to test aom_codec_enc_config_set
    if (fdp.remaining_bytes() > 4) {
        cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
        cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
        cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
        
        aom_codec_enc_config_set(&encoder, &cfg);
    }

    // Step 4: Set encoder options via key-value pairs
    // Common encoder option names from libaom documentation
    const char* option_names[] = {
        "min-q", "max-q", "undershoot-pct", "overshoot-pct",
        "buf-sz", "buf-initial-sz", "buf-optimal-sz", "bias-pct",
        "minsection-pct", "maxsection-pct", "arnr-strength",
        "arnr-maxframes", "cpu-used", "auto-alt-ref", "lag-in-frames",
        "error-resilient", "aq-mode", "frame-parallel", "tile-columns",
        "tile-rows", "enable-cdef", "enable-restoration", "enable-global-motion"
    };
    const size_t num_option_names = sizeof(option_names) / sizeof(option_names[0]);

    for (uint8_t i = 0; i < num_options && fdp.remaining_bytes() > 2; i++) {
        // Consume option name index
        uint8_t name_idx = fdp.ConsumeIntegral<uint8_t>() % num_option_names;
        const char* name = option_names[name_idx];
        
        // Consume option value as string
        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (fdp.remaining_bytes() < value_len) {
            value_len = fdp.remaining_bytes();
        }
        if (value_len == 0) break;
        
        std::string value = fdp.ConsumeBytesAsString(value_len);
        
        // Set the option
        aom_codec_set_option(&encoder, name, value.c_str());
    }

    // Step 5: Retrieve global headers after configuration
    // This should be called after all configuration is complete
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Free the memory as required by documentation
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // Step 6: Get preview frame from configured encoder
    // Note: This may return NULL if no preview frame is available
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&encoder);
    // No need to free preview_frame - it's owned by the codec context

    // Step 7: Cleanup - destroy encoder instance
    aom_codec_destroy(&encoder);

    return 0;
}

} // namespace harness_003
