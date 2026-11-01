// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return 1;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    /* Fixed: Changed allow_highbitdepth to allow_lowbitdepth as per struct definition */
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 1;

    return 0;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_286(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    int tile_col, tile_row;
    int tile_count;
    aom_tile_data tile_info;
    int debug_flag;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (init_decoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    /* 1. AV1_SET_DECODE_TILE_COL */
    if (Size > 0) tile_col = Data[0] % 64;
    else tile_col = 0;
    
    ret = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &tile_col);
    (void)ret;

    /* 4. AV1_SET_DECODE_TILE_ROW */
    if (Size > 1) tile_row = Data[1] % 64;
    else tile_row = 0;
    
    ret = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &tile_row);
    (void)ret;

    /* 2. AV1D_GET_TILE_COUNT */
    tile_count = 0;
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);
    (void)ret;

    /* 5. AV1D_GET_TILE_SIZE */
    size_t tile_size = 0;
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);
    (void)ret;

    /* 3. AV1D_GET_TILE_DATA */
    memset(&tile_info, 0, sizeof(tile_info));
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_info);
    (void)ret;

    /* 6. AV1D_EXT_TILE_DEBUG */
    if (Size > 2) debug_flag = Data[2];
    else debug_flag = 0;
    
    ret = aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &debug_flag);
    (void)ret;

    /* Cleanup */
    cleanup_decoder(&ctx);

    return 0;
}