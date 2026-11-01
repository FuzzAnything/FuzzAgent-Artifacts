// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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
 * Fuzz driver for libaom AV1 decoder control functions.
 * Targets:
 * - AV1D_GET_TILE_SIZE
 * - AV1_SET_DECODE_TILE_COL
 * - AV1_SET_DECODE_TILE_ROW
 * - AV1D_GET_TILE_COUNT
 * - AV1D_EXT_TILE_DEBUG
 * - AV1D_GET_TILE_DATA
 */

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Initialize decoder configuration */
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    /* Get the AV1 decoder interface */
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    /* Initialize the codec context */
    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    /* 
     * aom_codec_decode signature: 
     * aom_codec_err_t aom_codec_decode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t data_sz, void *user_priv);
     * We pass the fuzz input directly as the bitstream data.
     */
    err = aom_codec_decode(&ctx, Data, Size, NULL);
    /* Ignore decode errors, we just want to reach a state where control calls are valid */

    int32_t dummy_int;
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));

    /* 1. AV1D_GET_TILE_SIZE */
    #ifdef AV1D_GET_TILE_SIZE
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &dummy_int);
    #endif

    /* 2. AV1_SET_DECODE_TILE_COL */
    #ifdef AV1_SET_DECODE_TILE_COL
    int32_t col = (Size > 0) ? (int32_t)Data[0] : 0;
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &col);
    #endif

    /* 3. AV1_SET_DECODE_TILE_ROW */
    #ifdef AV1_SET_DECODE_TILE_ROW
    int32_t row = (Size > 1) ? (int32_t)Data[1] : 0;
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &row);
    #endif

    /* 4. AV1D_GET_TILE_COUNT */
    #ifdef AV1D_GET_TILE_COUNT
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &dummy_int);
    #endif

    /* 5. AV1D_EXT_TILE_DEBUG */
    #ifdef AV1D_EXT_TILE_DEBUG
    aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &dummy_int);
    #endif

    /* 6. AV1D_GET_TILE_DATA */
    #ifdef AV1D_GET_TILE_DATA
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);
    #endif

    /* Cleanup */
    cleanup_codec(&ctx);

    return 0;
}