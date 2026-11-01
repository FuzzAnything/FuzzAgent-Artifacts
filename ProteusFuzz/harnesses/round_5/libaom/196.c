// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_196(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    aom_codec_enc_cfg_t cfg;

    // Initialize encoder configuration
    err = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use input data to configure width/height to vary states
    uint32_t width = 16 + (Data[0] % 255);
    uint32_t height = 16 + (Data[1] % 255);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (Data[2] * 100);
    cfg.g_error_resilient = Data[3] & 1;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Allocate context
    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz AV1E_ENABLE_EXT_TILE_DEBUG
    int enable_ext_tile_debug = Data[4] % 2;
    aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable_ext_tile_debug);

    // Fuzz AV1E_SET_TILE_ROWS
    int tile_rows = (Data[5] % 16) + 1;
    aom_codec_control(ctx, AV1E_SET_TILE_ROWS, tile_rows);

    // Fuzz AV1E_SET_AUTO_TILES
    int auto_tiles = Data[6] % 2;
    aom_codec_control(ctx, AV1E_SET_AUTO_TILES, auto_tiles);

    // Fuzz AV1E_SET_TILE_COLUMNS
    int tile_columns = (Data[7] % 16) + 1;
    aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, tile_columns);

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING
    int single_tile_decoding = Data[8] % 2;
    aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile_decoding);

    // Attempt to invoke AV1_SET_TILE_MODE if available (decoder side, but try anyway)
    // This might fail on encoder context, which is expected behavior for fuzzing
    int tile_mode = Data[9] % 4;
    aom_codec_control(ctx, AV1_SET_TILE_MODE, tile_mode);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}