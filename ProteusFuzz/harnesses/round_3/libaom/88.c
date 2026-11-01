// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

/* Helper to safely initialize a decoder context */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* 
     * Removed unused packet/frame initialization logic that caused compilation errors.
     * The context is now ready for control calls.
     */

    return ctx;
}

/* Helper to cleanup decoder context */
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    /* Extract some values from input to fuzz parameters */
    int row = 0;
    int col = 0;
    int debug_mode = 0;
    
    if (Size >= 1) row = Data[0];
    if (Size >= 2) col = Data[1];
    if (Size >= 3) debug_mode = Data[2];

    /* Limit values to reasonable ranges to avoid immediate crashes on logic errors */
    if (row > 64) row = 64;
    if (col > 64) col = 64;

    /* 1. AV1D_GET_TILE_DATA */
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(ctx, AV1D_GET_TILE_DATA, &tile_data);

    /* 2. AV1_SET_DECODE_TILE_ROW */
    aom_codec_control(ctx, AV1_SET_DECODE_TILE_ROW, row);

    /* 3. AV1D_GET_TILE_COUNT */
    int tile_count = 0;
    aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &tile_count);

    /* 4. AV1_SET_DECODE_TILE_COL */
    aom_codec_control(ctx, AV1_SET_DECODE_TILE_COL, col);

    /* 5. AV1D_EXT_TILE_DEBUG */
    int ext_debug = debug_mode;
    aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, &ext_debug);

    /* 6. AV1D_GET_TILE_SIZE */
    int tile_width = 0;
    int tile_height = 0;
    aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &tile_width);
    aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &tile_height);

    /* Cleanup */
    cleanup_decoder(ctx);

    return 0;
}