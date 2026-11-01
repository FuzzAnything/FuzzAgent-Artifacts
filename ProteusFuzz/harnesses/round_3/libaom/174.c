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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/* 
 * Fuzz driver for libaom AV1 codec control functions.
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    if (ctx == NULL) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    /* Use fuzz data to configure some parameters if possible, but keep valid ranges */
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        if (size > 1) cfg.g_h = (data[1] % 256) + 16;
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
        if (size > 4) cfg.g_threads = (data[4] % 4) + 1;
        if (size > 5) cfg.rc_target_bitrate = (data[5] % 1000) + 100;
    } else {
        cfg.g_w = 320;
        cfg.g_h = 240;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 500;
    }

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    return 0;
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    if (ctx == NULL) return -1;

    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    /* Initialize config manually */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 0;

    if (size > 0) {
        cfg.threads = (data[0] % 4) + 1;
        if (size > 1) cfg.allow_lowbitdepth = (data[1] % 2);
    }

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

static void test_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->priv) return;

    /* AV1E_SET_NOISE_SENSITIVITY */
    int noise_sensitivity = 50;
    if (size > 0) noise_sensitivity = (data[0] % 101);
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    /* AV1E_SET_SINGLE_TILE_DECODING */
    int single_tile = 1;
    if (size > 1) single_tile = (data[1] % 2);
    aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);

    /* AV1E_SET_AUTO_TILES */
    int auto_tiles = 1;
    if (size > 2) auto_tiles = (data[2] % 2);
    aom_codec_control(ctx, AV1E_SET_AUTO_TILES, auto_tiles);
}

static void test_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->priv) return;

    /* AV1D_GET_TILE_COUNT */
    int tile_count = 0;
    aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &tile_count);

    /* AV1D_GET_TILE_SIZE */
    aom_tile_info tile_info;
    memset(&tile_info, 0, sizeof(tile_info));
    aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &tile_info);

    /* AV1D_EXT_TILE_DEBUG */
    int debug_mode = 0;
    if (size > 3) debug_mode = (data[3] % 2);
    aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, debug_mode);
}

int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Determine which codec to test based on first byte */
    int is_encoder = (Data[0] % 2);

    if (is_encoder) {
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));

        if (init_encoder(&ctx, Data, Size) == 0) {
            test_encoder_controls(&ctx, Data, Size);
            cleanup_encoder(&ctx);
        }
    } else {
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));

        if (init_decoder(&ctx, Data, Size) == 0) {
            test_decoder_controls(&ctx, Data, Size);
            cleanup_decoder(&ctx);
        }
    }

    return 0;
}