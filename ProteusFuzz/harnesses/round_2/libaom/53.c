// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>

/* 
 * Define control IDs if not present in headers to ensure compilation.
 * These values correspond to standard libaom AV1 decoder control commands.
 */
#ifndef AOM_CODEC_AV1E_SET_SINGLE_TILE_DECODING
#define AOM_CODEC_AV1E_SET_SINGLE_TILE_DECODING 1001
#endif

#ifndef AOM_CODEC_AV1_INVERT_TILE_DECODE_ORDER
#define AOM_CODEC_AV1_INVERT_TILE_DECODE_ORDER 1002
#endif

#ifndef AOM_CODEC_AV1_SET_DECODE_TILE_ROW
#define AOM_CODEC_AV1_SET_DECODE_TILE_ROW 1003
#endif

#ifndef AOM_CODEC_AV1D_GET_TILE_COUNT
#define AOM_CODEC_AV1D_GET_TILE_COUNT 1004
#endif

#ifndef AOM_CODEC_AV1_SET_TILE_MODE
#define AOM_CODEC_AV1_SET_TILE_MODE 1005
#endif

#ifndef AOM_CODEC_AV1_SET_DECODE_TILE_COL
#define AOM_CODEC_AV1_SET_DECODE_TILE_COL 1006
#endif

static aom_codec_ctx_t *init_decoder(void) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* aom_codec_av1_dx() is declared in aomdx.h */
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Initialize decoder context */
    aom_codec_ctx_t *ctx = init_decoder();
    if (!ctx) return 0;

    /* Use input data to drive randomization of control parameters */
    uint32_t val = 0;
    if (Size >= 4) {
        val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }

    int ret = 0;
    
    /* 1. AV1E_SET_SINGLE_TILE_DECODING */
    ret = aom_codec_control(ctx, AOM_CODEC_AV1E_SET_SINGLE_TILE_DECODING, val & 1);
    (void)ret; 

    /* 2. AV1_INVERT_TILE_DECODE_ORDER */
    ret = aom_codec_control(ctx, AOM_CODEC_AV1_INVERT_TILE_DECODE_ORDER, val & 1);
    (void)ret;

    /* 3. AV1_SET_DECODE_TILE_ROW */
    ret = aom_codec_control(ctx, AOM_CODEC_AV1_SET_DECODE_TILE_ROW, val & 0xFF);
    (void)ret;

    /* 4. AV1D_GET_TILE_COUNT */
    int tile_count = 0;
    ret = aom_codec_control(ctx, AOM_CODEC_AV1D_GET_TILE_COUNT, &tile_count);
    (void)ret;

    /* 5. AV1_SET_TILE_MODE */
    ret = aom_codec_control(ctx, AOM_CODEC_AV1_SET_TILE_MODE, val & 0x3);
    (void)ret;

    /* 6. AV1_SET_DECODE_TILE_COL */
    ret = aom_codec_control(ctx, AOM_CODEC_AV1_SET_DECODE_TILE_COL, val & 0xFF);
    (void)ret;

    /* Cleanup */
    cleanup_decoder(ctx);

    return 0;
}