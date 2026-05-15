#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_005 {
/*
 * Fuzzing harness for libvpx error handling and encoder configuration APIs
 * Targets uncovered error handling paths and invalid parameter testing
 * Focuses on: vpx_codec_error(), vpx_codec_error_detail(), vpx_codec_err_to_string(),
 *             invalid encoder initialization, configuration error scenarios
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 64;

// Helper function to test error handling APIs
static void test_error_handling_apis(vpx_codec_ctx_t *ctx, FuzzedDataProvider *fdp) {
    if (!ctx || !fdp) return;
    
    // Test vpx_codec_error() - should work even on invalid contexts
    const char *error_str = vpx_codec_error(ctx);
    if (error_str) {
        // Just check it's not null, don't rely on content
        (void)error_str;
    }
    
    // Test vpx_codec_error_detail() - may return NULL
    const char *error_detail = vpx_codec_error_detail(ctx);
    if (error_detail) {
        (void)error_detail;
    }
    
    // Test vpx_codec_err_to_string() with random error codes
    if (fdp->remaining_bytes() > 0) {
        uint8_t error_code = fdp->ConsumeIntegral<uint8_t>() % 20; // Sample error codes
        const char *err_str = vpx_codec_err_to_string(static_cast<vpx_codec_err_t>(error_code));
        if (err_str) {
            (void)err_str;
        }
    }
}

// Test invalid encoder initialization scenarios
static void test_invalid_encoder_init(FuzzedDataProvider *fdp) {
    if (!fdp) return;
    
    uint8_t test_type = fdp->ConsumeIntegral<uint8_t>() % 8;
    
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const vpx_codec_iface_t *encoder_iface = nullptr;
    uint8_t codec_choice = fdp->ConsumeIntegral<uint8_t>() % 2;
    
    if (codec_choice == 0) {
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    if (!encoder_iface) return;
    
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Get default config
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    switch (test_type) {
        case 0: {
            // Test 1: NULL context pointer
            vpx_codec_enc_init(nullptr, encoder_iface, &cfg, 0);
            break;
        }
        case 1: {
            // Test 2: NULL interface pointer
            vpx_codec_enc_init(&ctx, nullptr, &cfg, 0);
            break;
        }
        case 2: {
            // Test 3: NULL config pointer
            vpx_codec_enc_init(&ctx, encoder_iface, nullptr, 0);
            break;
        }
        case 3: {
            // Test 4: Invalid width/height (too small)
            cfg.g_w = 0;
            cfg.g_h = 0;
            vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
            break;
        }
        case 4: {
            // Test 5: Invalid width/height (too large)
            cfg.g_w = 65535;
            cfg.g_h = 65535;
            vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
            break;
        }
        case 5: {
            // Test 6: Invalid timebase (denominator = 0)
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 0;
            vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
            break;
        }
        case 6: {
            // Test 7: Invalid flags combination
            vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0xFFFFFFFF);
            break;
        }
        case 7: {
            // Test 8: Valid initialization but test error APIs afterward
            if (vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
                test_error_handling_apis(&ctx, fdp);
                vpx_codec_destroy(&ctx);
            }
            break;
        }
    }
    
    // Clean up if initialization succeeded
    if (ctx.err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    } else {
        // Test error APIs on failed context
        test_error_handling_apis(&ctx, fdp);
    }
}

// Test encoder control error scenarios
static void test_encoder_control_errors(FuzzedDataProvider *fdp) {
    if (!fdp || fdp->remaining_bytes() < 16) return;
    
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const vpx_codec_iface_t *encoder_iface = nullptr;
    uint8_t codec_choice = fdp->ConsumeIntegral<uint8_t>() % 2;
    
    if (codec_choice == 0) {
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    if (!encoder_iface) return;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Use reasonable dimensions
    cfg.g_w = 64;
    cfg.g_h = 48;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Test various control error scenarios
    uint8_t control_test = fdp->ConsumeIntegral<uint8_t>() % 6;
    switch (control_test) {
        case 0: {
            // Invalid control ID using direct function call
            vpx_codec_control_(&ctx, -1, nullptr);
            break;
        }
        case 1: {
            // Out of range control ID using direct function call
            vpx_codec_control_(&ctx, 9999, nullptr);
            break;
        }
        case 2: {
            // Valid control ID with NULL data using direct function call
            vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, nullptr);
            break;
        }
        case 3: {
            // Valid control ID with invalid data type using direct function call
            const char *invalid_str = "invalid";
            vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, invalid_str);
            break;
        }
        case 4: {
            // Multiple invalid controls in sequence using direct function call
            for (int i = 0; i < 5 && fdp->remaining_bytes() > 0; i++) {
                int ctrl_id = fdp->ConsumeIntegral<int>();
                vpx_codec_control_(&ctx, ctrl_id, nullptr);
            }
            break;
        }
        case 5: {
            // Mix of valid and invalid controls
            int cpu_used = fdp->ConsumeIntegralInRange<int>(-16, 16);
            // Use direct function call for valid control
            vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpu_used);
            
            // Then invalid using direct function call
            vpx_codec_control_(&ctx, 9999, nullptr);
            
            // Then another valid using direct function call
            unsigned int flags = fdp->ConsumeIntegral<unsigned int>();
            vpx_codec_control_(&ctx, VP8E_SET_ENABLEAUTOALTREF, flags & 1);
            break;
        }
    }

    // Test error APIs after control operations
    test_error_handling_apis(&ctx, fdp);
    
    // Clean up
    vpx_codec_destroy(&ctx);
}

// Test encoder configuration error scenarios
static void test_encoder_config_errors(FuzzedDataProvider *fdp) {
    if (!fdp || fdp->remaining_bytes() < 32) return;
    
    const vpx_codec_iface_t *encoder_iface = nullptr;
    uint8_t codec_choice = fdp->ConsumeIntegral<uint8_t>() % 2;
    
    if (codec_choice == 0) {
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    if (!encoder_iface) return;
    
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    uint8_t config_test = fdp->ConsumeIntegral<uint8_t>() % 5;
    
    switch (config_test) {
        case 0: {
            // Test with completely zeroed config
            vpx_codec_ctx_t ctx;
            vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
            test_error_handling_apis(&ctx, fdp);
            if (ctx.err == VPX_CODEC_OK) {
                vpx_codec_destroy(&ctx);
            }
            break;
        }
        case 1: {
            // Test vpx_codec_enc_config_default with NULL interface
            vpx_codec_enc_config_default(nullptr, &cfg, 0);
            break;
        }
        case 2: {
            // Test vpx_codec_enc_config_default with NULL config
            vpx_codec_enc_config_default(encoder_iface, nullptr, 0);
            break;
        }
        case 3: {
            // Test with invalid usage parameter
            vpx_codec_enc_config_default(encoder_iface, &cfg, 999);
            break;
        }
        case 4: {
            // Test get_caps with NULL interface
            vpx_codec_get_caps(nullptr);
            break;
        }
    }
}

// Test decoder error handling scenarios
static void test_decoder_error_handling(FuzzedDataProvider *fdp) {
    if (!fdp || fdp->remaining_bytes() < 16) return;
    
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const vpx_codec_iface_t *decoder_iface = nullptr;
    uint8_t codec_choice = fdp->ConsumeIntegral<uint8_t>() % 2;
    
    if (codec_choice == 0) {
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    if (!decoder_iface) return;
    
    vpx_codec_dec_cfg_t cfg = {1, 0, 0};
    
    uint8_t decoder_test = fdp->ConsumeIntegral<uint8_t>() % 4;
    
    switch (decoder_test) {
        case 0: {
            // NULL context
            vpx_codec_dec_init(nullptr, decoder_iface, &cfg, 0);
            break;
        }
        case 1: {
            // NULL interface
            vpx_codec_dec_init(&ctx, nullptr, &cfg, 0);
            break;
        }
        case 2: {
            // Invalid flags
            vpx_codec_dec_init(&ctx, decoder_iface, &cfg, 0xFFFFFFFF);
            break;
        }
        case 3: {
            // Valid init then test error APIs
            if (vpx_codec_dec_init(&ctx, decoder_iface, &cfg, 0) == VPX_CODEC_OK) {
                // Test decode with NULL data
                vpx_codec_decode(&ctx, nullptr, 0, nullptr, 0);
                test_error_handling_apis(&ctx, fdp);
                vpx_codec_destroy(&ctx);
            }
            break;
        }
    }

    // Test error APIs if context exists
    if (ctx.name && ctx.name[0] != '\0') {
        test_error_handling_apis(&ctx, fdp);
        if (ctx.err == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }
    }
}
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test type from fuzzer input
    uint8_t test_category = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (test_category) {
        case 0:
            // Invalid encoder initialization tests
            test_invalid_encoder_init(&fdp);
            break;
        case 1:
            // Encoder control error tests
            test_encoder_control_errors(&fdp);
            break;
        case 2:
            // Encoder configuration error tests
            test_encoder_config_errors(&fdp);
            break;
        case 3:
            // Decoder error handling tests
            test_decoder_error_handling(&fdp);
            break;
        case 4: {
            // Combined error scenario testing
            test_invalid_encoder_init(&fdp);
            if (fdp.remaining_bytes() > 32) {
                test_encoder_control_errors(&fdp);
            }
            if (fdp.remaining_bytes() > 16) {
                test_decoder_error_handling(&fdp);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_005
