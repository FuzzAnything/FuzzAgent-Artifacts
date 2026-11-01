// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1D_GET_TILE_SIZE at aomdx.h:504:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS at aomcx.h:1942:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_AUTO_TILES at aomcx.h:2348:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG at aomcx.h:2252:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING at aomcx.h:2168:1 in aomcx.h
// aom_codec_control_typechecked_AV1D_EXT_TILE_DEBUG at aomdx.h:540:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 codec control functions.
 */

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    // Initialize config with defaults
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    return 1;
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Use the correct function to get default config
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Set some basic config from fuzz data if possible
    if (size >= 4) {
        uint32_t val = *(const uint32_t *)data;
        cfg.g_w = val % 4096;
        if (cfg.g_w < 16) cfg.g_w = 16;
        cfg.g_h = (val >> 16) % 4096;
        if (cfg.g_h < 16) cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_threads = 1;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_threads = 1;
    }

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_242(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t dec_ctx;
    aom_codec_ctx_t enc_ctx;
    aom_codec_err_t res;
    int decoder_init = 0;
    int encoder_init = 0;
    uint32_t val;
    int use_decoder = 1;
    int use_encoder = 1;

    if (Size >= 1) {
        use_decoder = (Data[0] % 2) == 0;
        use_encoder = (Data[0] % 2) == 1;
    }

    // Initialize decoder if requested
    if (use_decoder) {
        decoder_init = init_decoder(&dec_ctx, Data, Size);
    }

    // Initialize encoder if requested
    if (use_encoder) {
        encoder_init = init_encoder(&enc_ctx, Data, Size);
    }

    // Extract a value for control parameters
    if (Size >= 4) {
        val = *(const uint32_t *)Data;
    } else {
        val = 0;
    }

    // 1. aom_codec_control_typechecked_AV1D_GET_TILE_SIZE
    // The macro expands to take 3 arguments: ctx, ctrl, data_ptr
    // The control ID is defined as AV1D_GET_TILE_SIZE in aom/aomcx.h or aom/aomdx.h
    if (decoder_init) {
        unsigned int tile_size;
        // AOM_CODEC_CTRL_AV1D_GET_TILE_SIZE is the command ID
        res = aom_codec_control_typechecked_AV1D_GET_TILE_SIZE(&dec_ctx, AV1D_GET_TILE_SIZE, &tile_size);
        (void)res;
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS
    if (encoder_init) {
        int tile_cols = (val % 64) + 1; // 1 to 64
        res = aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS(&enc_ctx, AV1E_SET_TILE_COLUMNS, tile_cols);
        (void)res;
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_AUTO_TILES
    if (encoder_init) {
        int auto_tiles = (val % 2); // 0 or 1
        res = aom_codec_control_typechecked_AV1E_SET_AUTO_TILES(&enc_ctx, AV1E_SET_AUTO_TILES, auto_tiles);
        (void)res;
    }

    // 4. aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG
    if (encoder_init) {
        int enable_debug = (val % 2); // 0 or 1
        res = aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG(&enc_ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable_debug);
        (void)res;
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING
    if (encoder_init) {
        int single_tile = (val % 2); // 0 or 1
        res = aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING(&enc_ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);
        (void)res;
    }

    // 6. aom_codec_control_typechecked_AV1D_EXT_TILE_DEBUG
    if (decoder_init) {
        int enable_debug = (val % 2); // 0 or 1
        res = aom_codec_control_typechecked_AV1D_EXT_TILE_DEBUG(&dec_ctx, AV1D_EXT_TILE_DEBUG, enable_debug);
        (void)res;
    }

    // Cleanup
    if (decoder_init) {
        aom_codec_destroy(&dec_ctx);
    }
    if (encoder_init) {
        aom_codec_destroy(&enc_ctx);
    }

    return 0;
}