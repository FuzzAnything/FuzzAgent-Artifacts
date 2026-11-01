// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    aom_codec_err_t res = aom_codec_enc_init(ctx, iface, NULL, 0);
    if (res != AOM_CODEC_OK) return -1;

    // Try to set some configuration if data is available
    if (size > 0) {
        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        
        // Use fuzzing data to tweak config slightly
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        
        res = aom_codec_enc_config_set(ctx, &cfg);
        if (res != AOM_CODEC_OK) return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Test aom_codec_err_to_string with various error codes
    // Using only valid error codes defined in aom_codec.h
    aom_codec_err_t err_codes[] = {
        AOM_CODEC_OK,
        AOM_CODEC_ERROR,
        AOM_CODEC_MEM_ERROR,
        AOM_CODEC_CORRUPT_FRAME,
        AOM_CODEC_INVALID_PARAM,
        0x12345678       // Unrecognized error
    };
    
    for (int i = 0; i < sizeof(err_codes) / sizeof(err_codes[0]); i++) {
        const char *err_str = aom_codec_err_to_string(err_codes[i]);
        (void)err_str; // Suppress unused warning
    }

    // Initialize encoder context
    if (init_encoder_context(&ctx, Data, Size) != 0) {
        // Even if init fails, we can still test error functions
        const char *err_msg = aom_codec_error(&ctx);
        (void)err_msg;
        cleanup_codec(&ctx);
        return 0;
    }

    // Test aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS
    // Using AV1E_SET_QUANTIZER_ONE_PASS instead of AV1E_SET_QUANTIZER
    if (Size > 0) {
        int quantizer = Data[0] % 256;
        aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer);
    }

    // Test aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER
    int last_quantizer;
    aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &last_quantizer);
    (void)last_quantizer;

    // Test aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH
    if (Size > 1) {
        int deltaq_strength = Data[1] % 10;
        aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);
    }

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            // Process the buffer if needed
            (void)global_headers->sz;
        }
        free(global_headers->buf);
        free(global_headers);
    }

    // Test aom_codec_error
    const char *error_msg = aom_codec_error(&ctx);
    (void)error_msg;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}