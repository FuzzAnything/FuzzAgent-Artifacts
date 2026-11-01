// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>

/* Helper to initialize an encoder context */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzz input to set some config parameters safely */
    if (size > 0) cfg.g_w = (data[0] % 512) + 16;
    if (size > 1) cfg.g_h = (data[1] % 512) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 255) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 255) + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to initialize a decoder context */
static aom_codec_ctx_t *init_decoder(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Manually initialize config */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Cleanup helper */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Fuzz encoder-related controls */
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        int32_t val;
        if (Size > 0) val = Data[0];
        else val = 0;

        /* AOME_SET_SPATIAL_LAYER_ID */
        aom_codec_control(enc_ctx, AOME_SET_SPATIAL_LAYER_ID, val);

        /* AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE */
        if (Size > 1) val = Data[1] ? 1 : 0;
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);

        /* AV1E_SET_SVC_FRAME_DROP_MODE */
        if (Size > 2) val = Data[2] % 4;
        aom_codec_control(enc_ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);

        /* AOME_SET_NUMBER_SPATIAL_LAYERS */
        if (Size > 3) val = (Data[3] % 8) + 1;
        aom_codec_control(enc_ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val);

        /* AV1E_SET_ENABLE_FILTER_INTRA */
        if (Size > 4) val = Data[4] ? 1 : 0;
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);

        cleanup_encoder(enc_ctx);
    }

    /* Fuzz decoder-related controls */
    aom_codec_ctx_t *dec_ctx = init_decoder();
    if (dec_ctx) {
        int32_t op_val;
        if (Size > 5) op_val = Data[5] % 32;
        else op_val = 0;

        /* AV1D_SET_OPERATING_POINT */
        aom_codec_control(dec_ctx, AV1D_SET_OPERATING_POINT, op_val);

        cleanup_decoder(dec_ctx);
    }

    return 0;
}