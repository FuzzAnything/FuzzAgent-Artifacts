// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom AV1 decoder control functions.
 * Targets:
 * - AV1D_GET_TILE_DATA
 * - AV1D_EXT_TILE_DEBUG
 * - AV1_SET_DECODE_TILE_ROW
 * - AV1D_GET_TILE_COUNT
 * - AV1D_GET_TILE_SIZE
 * - AV1_SET_DECODE_TILE_COL
 */

static int init_decoder(aom_codec_ctx_t *ctx, aom_codec_dec_cfg_t *cfg, aom_codec_iface_t *iface) {
    cfg->w = 64;
    cfg->h = 64;
    cfg->threads = 0;
    cfg->allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    fclose(f);
    return 1;
}

static int read_dummy_file(aom_codec_ctx_t *ctx) {
    FILE *f = fopen("./dummy_file", "rb");
    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize <= 0) {
        fclose(f);
        return 0;
    }

    uint8_t *buf = (uint8_t *)malloc(fsize);
    if (!buf) {
        fclose(f);
        return 0;
    }

    if (fread(buf, 1, fsize, f) != (size_t)fsize) {
        free(buf);
        fclose(f);
        return 0;
    }
    fclose(f);

    // aom_codec_decode signature: aom_codec_decode(ctx, data, data_sz, user_priv)
    aom_codec_decode(ctx, buf, fsize, NULL);

    free(buf);
    return 1;
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_dx();

    if (!iface) return 0;

    if (!init_decoder(&ctx, &cfg, iface)) {
        return 0;
    }

    // Create a dummy file with input data to simulate a bitstream
    if (!setup_dummy_file(Data, Size)) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Attempt to decode at least one frame to initialize internal state
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame;

    if (!read_dummy_file(&ctx)) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Try to consume any decoded frames
    // aom_codec_get_frame signature: aom_codec_get_frame(ctx, iter)
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        aom_img_free(frame);
    }

    // Prepare arguments for control functions based on input data
    int32_t tile_row = 0;
    int32_t tile_col = 0;

    if (Size >= 4) {
        tile_row = *(const int32_t *)(Data + 0);
        tile_col = *(const int32_t *)(Data + 4);
    } else if (Size >= 2) {
        tile_row = *(const int16_t *)(Data + 0);
        tile_col = *(const int16_t *)(Data + 2);
    } else if (Size >= 1) {
        tile_row = Data[0];
        tile_col = Data[0];
    }

    // Normalize values to reasonable ranges
    if (tile_row < 0) tile_row = -tile_row;
    if (tile_col < 0) tile_col = -tile_col;
    if (tile_row > 1024) tile_row = 1024;
    if (tile_col > 1024) tile_col = 1024;

    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));

    // 1. AV1D_GET_TILE_DATA
    aom_codec_err_t ret = aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);
    (void)ret;

    // 2. AV1D_EXT_TILE_DEBUG
    ret = aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &tile_data);
    (void)ret;

    // 3. AV1_SET_DECODE_TILE_ROW
    ret = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &tile_row);
    (void)ret;

    // 4. AV1D_GET_TILE_COUNT
    int32_t count = 0;
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &count);
    (void)ret;

    // 5. AV1D_GET_TILE_SIZE
    int32_t size = 0;
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &size);
    (void)ret;

    // 6. AV1_SET_DECODE_TILE_COL
    ret = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &tile_col);
    (void)ret;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}