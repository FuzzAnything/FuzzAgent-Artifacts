// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_TILE_ROWS at aomcx.h:1945:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_AUTO_TILES at aomcx.h:2348:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG at aomcx.h:2252:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING at aomcx.h:2168:1 in aomcx.h
// aom_codec_control_typechecked_AV1_SET_TILE_MODE at aomdx.h:528:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS at aomcx.h:1942:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int setup_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int width = 64;
    int height = 64;

    if (size >= 4) {
        width = data[0] & 0x7F;
        height = data[1] & 0x7F;
        if (width == 0 || height == 0) {
            width = 64;
            height = 64;
        }
        if (width > 4096 || height > 4096) {
            width = 4096;
            height = 4096;
        }
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_profile = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    int tile_cols, tile_rows, auto_tiles, ext_tile_debug, single_tile_decoding, tile_mode;

    if (Size < 6) {
        return 0;
    }

    tile_cols = Data[0] & 0x7F;
    tile_rows = Data[1] & 0x7F;
    auto_tiles = Data[2] & 0x01;
    ext_tile_debug = Data[3] & 0x01;
    single_tile_decoding = Data[4] & 0x01;
    tile_mode = Data[5] & 0x07;

    memset(&ctx, 0, sizeof(ctx));

    if (!setup_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_TILE_COLUMNS
    if (tile_cols > 0 && tile_cols <= 64) {
        ret = aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);
        (void)ret; 
    }

    // Fuzz AV1E_SET_TILE_ROWS
    if (tile_rows > 0 && tile_rows <= 64) {
        ret = aom_codec_control_typechecked_AV1E_SET_TILE_ROWS(&ctx, AV1E_SET_TILE_ROWS, tile_rows);
        (void)ret;
    }

    // Fuzz AV1E_SET_AUTO_TILES
    ret = aom_codec_control_typechecked_AV1E_SET_AUTO_TILES(&ctx, AV1E_SET_AUTO_TILES, auto_tiles);
    (void)ret;

    // Fuzz AV1E_ENABLE_EXT_TILE_DEBUG
    ret = aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, ext_tile_debug);
    (void)ret;

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING
    ret = aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING(&ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile_decoding);
    (void)ret;

    // Fuzz AV1_SET_TILE_MODE
    ret = aom_codec_control_typechecked_AV1_SET_TILE_MODE(&ctx, AV1_SET_TILE_MODE, tile_mode);
    (void)ret;

    cleanup_encoder(&ctx);

    return 0;
}