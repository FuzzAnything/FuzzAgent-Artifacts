// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_decoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret = 0;
    int tile_col, tile_row, invert_order, tile_mode, single_tile;
    int tile_count;

    if (size < 10) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Parse input data for control parameters
    tile_col = (int)(data[0] % 64);
    tile_row = (int)(data[1] % 64);
    invert_order = (data[2] % 2);
    tile_mode = (int)(data[3] % 4);
    single_tile = (data[4] % 2);

    // AV1_SET_DECODE_TILE_COL
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, tile_col);

    // AV1_INVERT_TILE_DECODE_ORDER
    aom_codec_control(&ctx, AV1_INVERT_TILE_DECODE_ORDER, invert_order);

    // AV1_SET_DECODE_TILE_ROW
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, tile_row);

    // AV1_SET_TILE_MODE
    aom_codec_control(&ctx, AV1_SET_TILE_MODE, tile_mode);

    // AV1D_GET_TILE_COUNT
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_encoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret = 0;
    int single_tile;

    if (size < 5) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_config_default(&cfg, iface, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    single_tile = (data[0] % 2);

    // AV1E_SET_SINGLE_TILE_DECODING
    aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Fuzz decoder controls
    fuzz_decoder_control(Data, Size);

    // Fuzz encoder controls
    fuzz_encoder_control(Data, Size);

    return 0;
}