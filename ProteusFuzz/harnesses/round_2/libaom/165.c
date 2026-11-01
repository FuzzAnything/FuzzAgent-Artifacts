// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>

/*
 * Helper function to safely initialize a decoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_decoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    /* Initialize config with default values */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    /* Use the correct API to get the AV1 decoder interface */
    iface = aom_codec_av1_dx();
    if (!iface) {
        return NULL;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }

    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    ctx->iface = iface;
    ctx->config.dec = &cfg;
    ctx->init_flags = 0;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Feed some data to the decoder to ensure it's in a valid state for control calls */
    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        aom_codec_err_t decode_res;
        
        /* Use the standard decode API with raw data pointers */
        decode_res = aom_codec_decode(ctx, data, size, NULL);
        /* We don't care if decode fails, we just need the context initialized */
        (void)decode_res;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;
    int32_t val;
    aom_tile_data tile_data;

    if (Size < 2) {
        return 0;
    }

    /* Use first byte to select the API to fuzz */
    int api_choice = Data[0] % 6;
    
    /* Initialize decoder context */
    ctx = init_decoder_context(Data + 1, Size > 1 ? Size - 1 : 0);
    if (!ctx) {
        return 0;
    }

    /* Prepare random values from fuzz data for parameters */
    int32_t param_int = 0;
    if (Size > 2) {
        param_int = (int32_t)Data[1];
        if (Size > 3) param_int = (param_int << 8) | Data[2];
        if (Size > 4) param_int = (param_int << 8) | Data[3];
    }

    switch (api_choice) {
        case 0:
            /* aom_codec_control_typechecked_AV1D_GET_TILE_DATA */
            /* Signature: aom_codec_control(ctx, AV1D_GET_TILE_DATA, &tile_data) */
            memset(&tile_data, 0, sizeof(tile_data));
            res = aom_codec_control(ctx, AV1D_GET_TILE_DATA, &tile_data);
            (void)res;
            break;

        case 1:
            /* aom_codec_control_typechecked_AV1D_EXT_TILE_DEBUG */
            /* Signature: aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, 0) */
            res = aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, 0);
            (void)res;
            break;

        case 2:
            /* aom_codec_control_typechecked_AV1_SET_DECODE_TILE_ROW */
            /* Signature: aom_codec_control(ctx, AV1_SET_DECODE_TILE_ROW, row) */
            res = aom_codec_control(ctx, AV1_SET_DECODE_TILE_ROW, param_int);
            (void)res;
            break;

        case 3:
            /* aom_codec_control_typechecked_AV1D_GET_TILE_COUNT */
            /* Signature: aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &count) */
            res = aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &val);
            (void)res;
            break;

        case 4:
            /* aom_codec_control_typechecked_AV1D_GET_TILE_SIZE */
            /* Signature: aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &size) */
            res = aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &val);
            (void)res;
            break;

        case 5:
            /* aom_codec_control_typechecked_AV1_SET_DECODE_TILE_COL */
            /* Signature: aom_codec_control(ctx, AV1_SET_DECODE_TILE_COL, col) */
            res = aom_codec_control(ctx, AV1_SET_DECODE_TILE_COL, param_int);
            (void)res;
            break;
    }

    /* Cleanup */
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}