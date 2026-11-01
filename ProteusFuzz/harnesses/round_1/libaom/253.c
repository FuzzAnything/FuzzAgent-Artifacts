// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/*
 * Helper function to initialize an encoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzzing data to set some config parameters safely */
    if (size >= 7) {
        cfg.g_w = (uint32_t)(data[0] % 256) + 16;
        cfg.g_h = (uint32_t)(data[1] % 256) + 16;
        cfg.g_timebase.num = (data[2] % 10) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.g_threads = (data[4] % 4) + 1;
        cfg.rc_target_bitrate = (data[5] * 256 + data[6]) + 100;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 100;
    }

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to initialize a decoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    memset(&cfg, 0, sizeof(cfg));
    if (size >= 2) {
        cfg.threads = (data[0] % 4) + 1;
        cfg.allow_lowbitdepth = (data[1] % 2);
    } else {
        cfg.threads = 1;
        cfg.allow_lowbitdepth = 1;
    }

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Fuzz Encoder Control Functions and Global Headers */
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        /* 1. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER */
        int quantizer;
        aom_codec_err_t res = aom_codec_control(enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
        (void)res; /* Ignore errors, just exercise the path */

        /* 2. aom_codec_control_typechecked_AV1E_SET_TILE_ROWS */
        int tile_rows = (Size > 0) ? (Data[0] % 8) + 1 : 1;
        res = aom_codec_control(enc_ctx, AV1E_SET_TILE_ROWS, tile_rows);
        (void)res;

        /* 3. aom_codec_control_typechecked_AV1E_SET_FRAME_PARALLEL_DECODING */
        int parallel_decoding = (Size > 1) ? (Data[1] % 2) : 0;
        res = aom_codec_control(enc_ctx, AV1E_SET_FRAME_PARALLEL_DECODING, parallel_decoding);
        (void)res;

        /* 6. aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS */
        int qp = (Size > 2) ? (Data[2] % 64) + 1 : 1;
        res = aom_codec_control(enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, qp);
        (void)res;

        /* 5. aom_codec_get_global_headers */
        aom_fixed_buf_t *headers = aom_codec_get_global_headers(enc_ctx);
        if (headers) {
            /* Exercise the buffer if valid */
            if (headers->buf && headers->sz > 0) {
                /* Just read to trigger logic, no need to process */
                volatile uint8_t dummy = ((uint8_t *)headers->buf)[0];
                (void)dummy;
            }
            /* Free the buffer as per documentation */
            if (headers->buf) {
                free(headers->buf);
            }
            free(headers);
        }

        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }

    /* Fuzz Decoder Control Functions */
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        /* 4. aom_codec_control_typechecked_AV1D_GET_BIT_DEPTH */
        int bit_depth;
        aom_codec_err_t res = aom_codec_control(dec_ctx, AV1D_GET_BIT_DEPTH, &bit_depth);
        (void)res;

        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    return 0;
}