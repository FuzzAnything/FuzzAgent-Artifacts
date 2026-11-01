// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

static aom_codec_ctx_t *init_decoder_context(void) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_decoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_314(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file if needed
    if (write_dummy_file(Data, Size) != 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder_context();
    if (!ctx) return 0;

    // Decode one frame to set up internal state for tile controls
    // Note: Passing raw fuzz data as bitstream; decoder may fail, which is expected.
    aom_codec_decode(ctx, Data, Size, NULL);

    // Fuzz AV1_SET_DECODE_TILE_COL
    int tile_col = 0;
    if (Size > 0) tile_col = Data[0] % 32;
    aom_codec_control(ctx, AV1_SET_DECODE_TILE_COL, tile_col);

    // Fuzz AV1_SET_DECODE_TILE_ROW
    int tile_row = 0;
    if (Size > 1) tile_row = Data[1] % 32;
    aom_codec_control(ctx, AV1_SET_DECODE_TILE_ROW, tile_row);

    // Fuzz AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // Fuzz AV1D_GET_TILE_SIZE
    aom_tile_data tile_size;
    memset(&tile_size, 0, sizeof(tile_size));
    aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &tile_size);

    // Fuzz AV1D_GET_TILE_DATA
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(ctx, AV1D_GET_TILE_DATA, &tile_data);

    // Fuzz AV1D_EXT_TILE_DEBUG (if supported)
    int ext_tile_debug = 0;
    if (Size > 2) ext_tile_debug = Data[2] % 2;
    aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, ext_tile_debug);

    cleanup_decoder_context(ctx);
    return 0;
}