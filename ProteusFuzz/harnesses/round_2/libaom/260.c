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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    if (size >= 8) {
        uint32_t w = *(const uint32_t *)data;
        uint32_t h = *(const uint32_t *)(data + 4);
        if (w > 0 && w <= 8192 && h > 0 && h <= 8192) {
            cfg.g_w = w;
            cfg.g_h = h;
        }
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

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

int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    uint8_t mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    if (mode == 0) {
        aom_codec_ctx_t *ctx = init_encoder(payload, payload_size);
        if (!ctx) return 0;

        int val_int;
        if (payload_size >= 4) {
            val_int = *(const int32_t *)payload;
        } else {
            val_int = 0;
        }

        aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val_int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val_int % 2);
        aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val_int % 2);
        aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val_int % 2);
        aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val_int % 3);

        cleanup_encoder(ctx);
    } else {
        aom_codec_ctx_t *ctx = init_decoder(payload, payload_size);
        if (!ctx) return 0;

        int val_int;
        if (payload_size >= 4) {
            val_int = *(const int32_t *)payload;
        } else {
            val_int = 0;
        }

        aom_codec_control(ctx, AV1D_SET_ROW_MT, val_int % 2);

        cleanup_decoder(ctx);
    }

    return 0;
}