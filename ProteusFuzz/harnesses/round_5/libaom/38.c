// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1_SET_TILE_MODE at aomdx.h:528:1 in aomdx.h
// aom_codec_control_typechecked_AV1D_GET_TILE_COUNT at aomdx.h:507:1 in aomdx.h
// aom_codec_control_typechecked_AV1D_GET_TILE_SIZE at aomdx.h:504:1 in aomdx.h
// aom_codec_control_typechecked_AV1D_GET_TILE_DATA at aomdx.h:534:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control_typechecked_AV1_SET_DECODE_TILE_COL at aomdx.h:525:1 in aomdx.h
// aom_codec_control_typechecked_AV1_SET_DECODE_TILE_ROW at aomdx.h:522:1 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/* 
 * Helper to initialize a decoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    if (!ctx || !data || size == 0) {
        return -1;
    }

    iface = aom_codec_av1_dx();
    if (!iface) {
        return -1;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/* 
 * Helper to decode a frame to prepare the context for tile-related controls.
 * Returns 0 if a frame was successfully decoded, -1 otherwise.
 */
static int decode_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t ret;
    aom_codec_iter_t iter = NULL;
    const aom_image_t *img;

    ret = aom_codec_decode(ctx, data, size, NULL);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    img = aom_codec_get_frame(ctx, &iter);
    if (!img) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int tile_col, tile_row, tile_count;
    aom_tile_data tile_data;

    if (Size < 10) {
        return 0;
    }

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    if (init_decoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Attempt to decode a frame to ensure context is valid for tile queries
    if (decode_frame(&ctx, Data, Size) != 0) {
        // If decode fails, we still try the controls as they might have side effects
    }

    // Fuzz AV1_SET_DECODE_TILE_COL
    if (Size > 0) {
        tile_col = Data[0];
        ret = aom_codec_control_typechecked_AV1_SET_DECODE_TILE_COL(&ctx, AV1_SET_DECODE_TILE_COL, tile_col);
        (void)ret;
    }

    // Fuzz AV1_SET_DECODE_TILE_ROW
    if (Size > 1) {
        tile_row = Data[1];
        ret = aom_codec_control_typechecked_AV1_SET_DECODE_TILE_ROW(&ctx, AV1_SET_DECODE_TILE_ROW, tile_row);
        (void)ret;
    }

    // Fuzz AV1_SET_TILE_MODE
    if (Size > 2) {
        int tile_mode = Data[2];
        ret = aom_codec_control_typechecked_AV1_SET_TILE_MODE(&ctx, AV1_SET_TILE_MODE, tile_mode);
        (void)ret;
    }

    // Fuzz AV1D_GET_TILE_COUNT
    ret = aom_codec_control_typechecked_AV1D_GET_TILE_COUNT(&ctx, AV1D_GET_TILE_COUNT, &tile_count);
    (void)ret;

    // Fuzz AV1D_GET_TILE_SIZE
    ret = aom_codec_control_typechecked_AV1D_GET_TILE_SIZE(&ctx, AV1D_GET_TILE_SIZE, &tile_data);
    (void)ret;

    // Fuzz AV1D_GET_TILE_DATA
    ret = aom_codec_control_typechecked_AV1D_GET_TILE_DATA(&ctx, AV1D_GET_TILE_DATA, &tile_data);
    (void)ret;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}