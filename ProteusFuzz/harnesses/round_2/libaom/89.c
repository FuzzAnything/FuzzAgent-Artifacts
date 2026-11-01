// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomdx.h"

static int setup_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    if (size > 0) {
        /* Initialization logic if needed, currently skipped to avoid invalid controls */
    }

    return 1;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    int tile_row, tile_col;
    int tile_count, tile_size;
    aom_tile_data tile_data;
    int ext_tile_debug;

    if (Size < 3) return 0;

    /* Use the input data to seed configuration values safely */
    tile_row = (int)(Data[0] & 0xFF);
    tile_col = (int)(Data[1] & 0xFF);
    ext_tile_debug = (int)(Data[2] & 0xFF);

    memset(&ctx, 0, sizeof(ctx));

    if (!setup_decoder(&ctx, Data, Size)) {
        return 0;
    }

    /* Fuzz AV1_SET_DECODE_TILE_ROW */
    ret = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, tile_row);
    (void)ret;

    /* Fuzz AV1_SET_DECODE_TILE_COL */
    ret = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, tile_col);
    (void)ret;

    /* Fuzz AV1D_GET_TILE_COUNT */
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);
    (void)ret;

    /* Fuzz AV1D_GET_TILE_SIZE */
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);
    (void)ret;

    /* Fuzz AV1D_GET_TILE_DATA */
    memset(&tile_data, 0, sizeof(tile_data));
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);
    (void)ret;

    /* Fuzz AV1D_EXT_TILE_DEBUG */
    ret = aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, ext_tile_debug);
    (void)ret;

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}