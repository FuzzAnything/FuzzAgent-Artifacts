// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
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

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 64;
        cfg.g_h = (data[1] % 1024) + 64;
        cfg.g_timebase.num = (data[2] % 30) + 1;
        cfg.g_timebase.den = (data[3] % 60) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_251(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) {
        return 0;
    }

    // Fuzz AV1E_ENABLE_EXT_TILE_DEBUG
    int enable_debug = (Data[0] % 2);
    aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable_debug);

    // Fuzz AV1E_SET_TILE_ROWS
    int tile_rows = (Data[1] % 8) + 1;
    aom_codec_control(ctx, AV1E_SET_TILE_ROWS, tile_rows);

    // Fuzz AV1E_SET_AUTO_TILES
    int auto_tiles = (Data[2] % 2);
    aom_codec_control(ctx, AV1E_SET_AUTO_TILES, auto_tiles);

    // Fuzz AV1_SET_TILE_MODE
    int tile_mode = (Data[3] % 4);
    aom_codec_control(ctx, AV1_SET_TILE_MODE, tile_mode);

    // Fuzz AV1E_SET_TILE_COLUMNS
    if (Size > 4) {
        int tile_columns = (Data[4] % 8) + 1;
        aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, tile_columns);
    }

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING
    if (Size > 5) {
        int single_tile = (Data[5] % 2);
        aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);
    }

    cleanup_codec(ctx);
    return 0;
}