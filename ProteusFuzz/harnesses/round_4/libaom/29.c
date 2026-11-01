// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (!ctx || !cfg) return -1;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Extract values from fuzz input to diversify function calls
    int tile_columns = (Size > 0) ? (int)(Data[0] % 64) : 1;
    int tile_rows = (Size > 1) ? (int)(Data[1] % 64) : 1;
    int enable_ext_tile_debug = (Size > 2) ? (int)(Data[2] % 2) : 0;
    int enable_rect_partitions = (Size > 3) ? (int)(Data[3] % 2) : 0;
    int single_tile_decoding = (Size > 4) ? (int)(Data[4] % 2) : 0;
    int tile_mode = (Size > 5) ? (int)(Data[5] % 4) : 0; // Assuming tile_mode is an enum with 4 values

    // Invoke target functions with fuzzed parameters
    // Note: Using standard aom_codec_control with AV1 specific control IDs
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_columns);
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, tile_rows);
    aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable_ext_tile_debug);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_rect_partitions);
    aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile_decoding);
    aom_codec_control(&ctx, AV1_SET_TILE_MODE, tile_mode);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}