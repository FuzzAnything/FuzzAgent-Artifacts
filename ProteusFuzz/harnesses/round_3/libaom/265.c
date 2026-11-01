// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

// Define the base if not present in the specific build environment
#ifndef AOM_CODEC_CTRL_BASE
#define AOM_CODEC_CTRL_BASE 0x10000000
#endif

// Helper macro to construct control IDs
#ifndef AOM_CODEC_CTRL_ID
#define AOM_CODEC_CTRL_ID(x) (AOM_CODEC_CTRL_BASE + (x))
#endif

static int fuzz_decode_tile_row(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    int row = (int)(data[0] % 64);
    return aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1_SET_DECODE_TILE_ROW), row);
}

static int fuzz_get_tile_info(aom_codec_ctx_t *ctx) {
    aom_tile_info info;
    memset(&info, 0, sizeof(info));
    return aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AOMD_GET_TILE_INFO), &info);
}

static int fuzz_set_tile_mode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    int mode = (int)(data[0] % 4);
    return aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1_SET_TILE_MODE), mode);
}

static int fuzz_set_single_tile_decoding(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    int enable = (int)(data[0] % 2);
    return aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_SINGLE_TILE_DECODING), enable);
}

static int fuzz_invert_tile_decode_order(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    int invert = (int)(data[0] % 2);
    return aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1_INVERT_TILE_DECODE_ORDER), invert);
}

static int fuzz_set_decode_tile_col(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    int col = (int)(data[0] % 64);
    return aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1_SET_DECODE_TILE_COL), col);
}

int LLVMFuzzerTestOneInput_265(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Fix: Use correct members for aom_codec_dec_cfg_t
    cfg.w = 128;
    cfg.h = 128;
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1_SET_DECODE_TILE_ROW
    fuzz_decode_tile_row(&ctx, Data, Size);

    // Fuzz AOMD_GET_TILE_INFO
    fuzz_get_tile_info(&ctx);

    // Fuzz AV1_SET_TILE_MODE
    fuzz_set_tile_mode(&ctx, Data, Size);

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING (may fail on decoder but safe to call)
    fuzz_set_single_tile_decoding(&ctx, Data, Size);

    // Fuzz AV1_INVERT_TILE_DECODE_ORDER
    fuzz_invert_tile_decode_order(&ctx, Data, Size);

    // Fuzz AV1_SET_DECODE_TILE_COL
    fuzz_set_decode_tile_col(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}