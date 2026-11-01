// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

/* Helper to initialize an encoder context for testing */
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    /* Initialize context */
    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* Initialize configuration with defaults */
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzz data to tweak config if possible, ensuring valid ranges and bounds */
    if (size > 0) {
        cfg.g_w = (data[0] % 255) + 1;
        if (size > 1) {
            cfg.g_h = (data[1] % 255) + 1;
        }
        /* Fixed: g_time_base -> g_timebase */
        if (size > 2) {
            cfg.g_timebase.num = (data[2] % 30) + 1;
        }
        if (size > 3) {
            cfg.g_timebase.den = (data[3] % 30) + 1;
        }
        if (size > 4) {
            cfg.rc_target_bitrate = (data[4] % 100) + 1;
        }
        if (size > 5) {
            cfg.g_threads = (data[5] % 8) + 1;
        }
        if (size > 6) {
            cfg.g_error_resilient = (data[6] % 2);
        }
    }

    /* Initialize the encoder */
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to initialize a decoder context for testing */
static aom_codec_ctx_t *init_decoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    /* Get the AV1 decoder interface */
    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    /* Initialize context */
    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* Initialize configuration with defaults */
    memset(&cfg, 0, sizeof(cfg));
    if (size > 0) {
        cfg.threads = (data[0] % 8) + 1;
    }
    if (size > 1) {
        cfg.allow_lowbitdepth = (data[1] % 2);
    }

    /* Initialize the decoder */
    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *enc_ctx = NULL;
    aom_codec_ctx_t *dec_ctx = NULL;
    aom_fixed_buf_t *global_headers = NULL;
    const char *err_str = NULL;
    const char *err_code_str = NULL;
    aom_codec_err_t err;

    /* Ensure minimum size for basic operations */
    if (Size < 1) {
        return 0;
    }

    /* 1. Test aom_codec_err_to_string with various error codes */
    /* Iterate through a range of error codes derived from fuzz data */
    for (size_t i = 0; i < 10 && i < Size; i++) {
        err = (aom_codec_err_t)(Data[i] % 100);
        err_code_str = aom_codec_err_to_string(err);
        /* Just consume the result to avoid unused variable warnings */
        if (err_code_str && strlen(err_code_str) > 0) {
            (void)err_code_str[0];
        }
    }

    /* 2. Initialize Encoder Context */
    enc_ctx = init_encoder_context(Data, Size);
    if (enc_ctx) {
        /* Test aom_codec_error with valid context */
        err_str = aom_codec_error(enc_ctx);
        if (err_str) {
            (void)err_str[0];
        }

        /* Test aom_codec_get_global_headers */
        global_headers = aom_codec_get_global_headers(enc_ctx);
        if (global_headers) {
            /* Simulate usage */
            if (global_headers->buf && global_headers->sz > 0) {
                (void)global_headers->buf[0];
            }
            /* Free the buffer as per API spec */
            free(global_headers->buf);
            free(global_headers);
            global_headers = NULL;
        }

        /* Cleanup encoder */
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
        enc_ctx = NULL;
    }

    /* 3. Initialize Decoder Context */
    dec_ctx = init_decoder_context(Data, Size);
    if (dec_ctx) {
        /* Test aom_codec_error with valid context */
        err_str = aom_codec_error(dec_ctx);
        if (err_str) {
            (void)err_str[0];
        }

        /* Test aom_codec_get_global_headers (likely unsupported for decoder, but safe to call) */
        global_headers = aom_codec_get_global_headers(dec_ctx);
        if (global_headers) {
            if (global_headers->buf && global_headers->sz > 0) {
                (void)global_headers->buf[0];
            }
            free(global_headers->buf);
            free(global_headers);
            global_headers = NULL;
        }

        /* Cleanup decoder */
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
        dec_ctx = NULL;
    }

    /* 4. Test aom_codec_error with NULL pointer */
    err_str = aom_codec_error(NULL);
    if (err_str) {
        (void)err_str[0];
    }

    /* 5. Test aom_codec_err_to_string with edge cases */
    err = AOM_CODEC_OK;
    err_code_str = aom_codec_err_to_string(err);
    if (err_code_str) (void)err_code_str[0];

    err = AOM_CODEC_MEM_ERROR;
    err_code_str = aom_codec_err_to_string(err);
    if (err_code_str) (void)err_code_str[0];

    err = (aom_codec_err_t)99999; /* Unrecognized error */
    err_code_str = aom_codec_err_to_string(err);
    if (err_code_str) (void)err_code_str[0];

    /* Note: The following functions were marked as having empty signatures in the prompt:
     * - aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH
     * - aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS
     * - aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER
     * Since their signatures are empty, they cannot be invoked in this driver.
     */

    return 0;
}