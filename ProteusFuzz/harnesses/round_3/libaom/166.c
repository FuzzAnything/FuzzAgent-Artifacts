// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 decoder control functions related to tiles.
 * Target APIs:
 * - AV1D_GET_TILE_DATA
 * - AV1_SET_DECODE_TILE_ROW
 * - AV1D_GET_TILE_COUNT
 * - AV1_SET_DECODE_TILE_COL
 * - AV1D_EXT_TILE_DEBUG
 * - AV1D_GET_TILE_SIZE
 */

static int try_decode_and_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t res;
    aom_tile_data tile_data_out;
    int tile_row = 0;
    int tile_col = 0;
    int tile_count = 0;
    int tile_width = 0;
    int debug_mode = 0;

    /* Try to decode a frame if input data is non-empty */
    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;

        res = aom_codec_decode(ctx, data, size, NULL);
        if (res != AOM_CODEC_OK) {
            /* Decoding failed, but we can still try control commands on the context */
        }

        /* Iterate over output packets if any */
        while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
            /* No specific action needed here, just consume packets */
            (void)pkt;
        }
    }

    /*
     * Attempt to invoke control functions.
     * Note: The constants AV1D_GET_TILE_DATA, etc., are expected to be defined
     * in aom/aomdx.h.
     */

    /* AV1D_GET_TILE_DATA */
    if (aom_codec_control(ctx, AV1D_GET_TILE_DATA, &tile_data_out) == AOM_CODEC_OK) {
        /* tile_data_out is populated */
        (void)tile_data_out;
    }

    /* AV1_SET_DECODE_TILE_ROW */
    tile_row = (int)(size % 100); /* Fuzz value */
    if (aom_codec_control(ctx, AV1_SET_DECODE_TILE_ROW, tile_row) == AOM_CODEC_OK) {
        /* Set succeeded */
    }

    /* AV1D_GET_TILE_COUNT */
    if (aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &tile_count) == AOM_CODEC_OK) {
        /* tile_count is populated */
    }

    /* AV1_SET_DECODE_TILE_COL */
    tile_col = (int)(size % 100); /* Fuzz value */
    if (aom_codec_control(ctx, AV1_SET_DECODE_TILE_COL, tile_col) == AOM_CODEC_OK) {
        /* Set succeeded */
    }

    /* AV1D_EXT_TILE_DEBUG */
    debug_mode = (size % 2) ? 1 : 0;
    if (aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, debug_mode) == AOM_CODEC_OK) {
        /* Debug mode set */
    }

    /* AV1D_GET_TILE_SIZE */
    if (aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &tile_width) == AOM_CODEC_OK) {
        /* tile_width is populated (or similar) */
    }

    return 0;
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    aom_codec_iface_t *decoder_iface = NULL;
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;
    int ret = 0;

    /* Initialize decoder interface */
    decoder_iface = aom_codec_av1_dx();
    if (decoder_iface == NULL) {
        return 0;
    }

    /* Initialize decoder configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.threads = (Size > 0 && Size < 64) ? (int)Size : 0;

    /* Initialize codec context */
    res = aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    /* Run fuzzing logic */
    ret = try_decode_and_control(&ctx, Data, Size);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return ret;
}