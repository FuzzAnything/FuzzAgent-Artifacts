// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/*
 * Helper function to initialize a dummy encoder context.
 * Returns NULL if initialization fails.
 */
static aom_codec_ctx_t *init_dummy_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Get the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    ctx->iface = iface;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to tweak configuration if size allows
    // Added bounds checking to prevent buffer overflow
    if (size > 6) {
        cfg.g_w = (data[0] % 16) + 16; // Width between 16 and 31
        cfg.g_h = (data[1] % 16) + 16; // Height between 16 and 31
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
        cfg.g_error_resilient = (data[5] % 2);
        cfg.g_lag_in_frames = (data[6] % 20) + 1;
    } else if (size > 0) {
        // Fallback for small inputs to ensure some variation without crashing
        cfg.g_w = (data[0] % 16) + 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 1;
        cfg.rc_target_bitrate = 100;
        cfg.g_error_resilient = 0;
        cfg.g_lag_in_frames = 1;
    }

    // Initialize the encoder context
    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to clean up the encoder context.
 */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_278(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_dummy_encoder(Data, Size);
    if (!ctx) return 0;

    // 1. Test aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(ctx);
    if (global_headers) {
        if (global_headers->buf) {
            // Use the buffer to trigger more states (e.g., copy to dummy file)
            FILE *f = fopen("./dummy_file", "wb");
            if (f) {
                fwrite(global_headers->buf, 1, global_headers->sz, f);
                fclose(f);
            }
        }
        // Free the buffer as per API documentation
        free(global_headers->buf);
        free(global_headers);
    }

    // 2. Test aom_codec_error
    const char *error_str = aom_codec_error(ctx);
    if (error_str) {
        // Just consume the string to avoid unused variable warnings
        (void)strlen(error_str);
    }

    // 3. Test aom_codec_err_to_string with various error codes
    // Using only valid error codes defined in aom_codec.h for this version
    aom_codec_err_t valid_err_codes[] = {
        AOM_CODEC_OK,
        AOM_CODEC_ERROR,
        AOM_CODEC_MEM_ERROR
    };
    int num_codes = sizeof(valid_err_codes) / sizeof(valid_err_codes[0]);
    for (int i = 0; i < num_codes; i++) {
        const char *err_str = aom_codec_err_to_string(valid_err_codes[i]);
        if (err_str) {
            (void)strlen(err_str);
        }
    }

    // 4. Attempt to invoke control functions if available (type-checked variants)
    // Note: Since the signatures for AOME_GET_LAST_QUANTIZER, AV1E_SET_QUANTIZER_ONE_PASS,
    // and AV1E_SET_DELTAQ_STRENGTH are not provided, we cannot directly call them.
    // However, we can attempt to call the generic aom_codec_control with known constants
    // if they are defined in the headers. This is a best-effort approach.

    // Example: Try to get last quantizer (if AOME_GET_LAST_QUANTIZER is defined)
    #ifdef AOME_GET_LAST_QUANTIZER
    int last_quantizer = 0;
    aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER, &last_quantizer);
    (void)last_quantizer;
    #endif

    // Example: Try to set quantizer one pass (if AV1E_SET_QUANTIZER_ONE_PASS is defined)
    #ifdef AV1E_SET_QUANTIZER_ONE_PASS
    int quantizer = Size > 0 ? (Data[0] % 64) : 0;
    aom_codec_control(ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer);
    #endif

    // Example: Try to set deltaq strength (if AV1E_SET_DELTAQ_STRENGTH is defined)
    #ifdef AV1E_SET_DELTAQ_STRENGTH
    int deltaq_strength = Size > 1 ? (Data[1] % 8) : 0;
    aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);
    #endif

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}