// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_200(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    
    // Ensure we have enough data for basic operations before accessing indices
    if (Size < 7) return 0;

    int mode = Data[0] % 3; // 0: encoder, 1: decoder, 2: mixed/error checks

    // Initialize context to zero
    memset(&ctx, 0, sizeof(ctx));

    if (mode == 0) {
        // Encoder path
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        res = aom_codec_enc_config_default(iface, &cfg, 0);
        if (res != AOM_CODEC_OK) return 0;

        // Fuzz configuration parameters based on input
        cfg.g_w = (Data[1] % 1024) + 16;
        cfg.g_h = (Data[2] % 1024) + 16;
        cfg.g_timebase.num = (Data[3] % 255) + 1;
        cfg.g_timebase.den = (Data[4] % 255) + 1;
        cfg.rc_target_bitrate = (Data[5] % 1000) + 100;

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_encoder(&ctx);
            return 0;
        }

        // Test aom_codec_get_global_headers
        aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
        if (headers) {
            if (headers->buf) {
                // Use the data slightly to avoid unused warnings, though logic is simple
                volatile uint8_t *p = (uint8_t *)headers->buf;
                volatile size_t sz = headers->sz;
                (void)p;
                (void)sz;
            }
            free(headers->buf);
            free(headers);
        }

        // Test aom_codec_error
        const char *err_str = aom_codec_error(&ctx);
        (void)err_str;

        // Test aom_codec_err_to_string with various codes
        aom_codec_err_t test_err = AOM_CODEC_OK;
        if (Data[6] & 1) test_err = AOM_CODEC_ERROR;
        if (Data[6] & 2) test_err = AOM_CODEC_MEM_ERROR;
        if (Data[6] & 4) test_err = AOM_CODEC_UNSUP_BITSTREAM;
        const char *err_str2 = aom_codec_err_to_string(test_err);
        (void)err_str2;

        cleanup_encoder(&ctx);

    } else if (mode == 1) {
        // Decoder path
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        res = aom_codec_dec_init(&ctx, iface, NULL, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_decoder(&ctx);
            return 0;
        }

        // Test aom_codec_error
        const char *err_str = aom_codec_error(&ctx);
        (void)err_str;

        // Test aom_codec_err_to_string
        aom_codec_err_t test_err = AOM_CODEC_OK;
        if (Data[6] & 1) test_err = AOM_CODEC_ERROR;
        const char *err_str2 = aom_codec_err_to_string(test_err);
        (void)err_str2;

        cleanup_decoder(&ctx);

    } else {
        // Mixed/Error path: Test with NULL or invalid contexts
        const char *err_str = aom_codec_error(NULL);
        (void)err_str;

        // Test aom_codec_err_to_string with random error codes
        aom_codec_err_t test_err = (aom_codec_err_t)(Data[1] % 100);
        const char *err_str2 = aom_codec_err_to_string(test_err);
        (void)err_str2;

        // Attempt to call get_global_headers on uninitialized context (should return NULL)
        aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
        if (headers) {
            free(headers->buf);
            free(headers);
        }
    }

    return 0;
}