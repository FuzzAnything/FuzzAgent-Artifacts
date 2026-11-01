// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int val;
    aom_codec_err_t ret;

    // AV1E_ENABLE_EXT_TILE_DEBUG
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
        (void)ret;
    }

    // AV1E_SET_TILE_ROWS
    if (size >= sizeof(int) + 1) {
        memcpy(&val, data + 1, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_TILE_ROWS, val);
        (void)ret;
    }

    // AV1E_SET_AUTO_TILES
    if (size >= sizeof(int) + 2) {
        memcpy(&val, data + 2, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_AUTO_TILES, val);
        (void)ret;
    }

    // AV1E_SET_TILE_COLUMNS
    if (size >= sizeof(int) + 3) {
        memcpy(&val, data + 3, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, val);
        (void)ret;
    }

    // AV1E_SET_SINGLE_TILE_DECODING
    if (size >= sizeof(int) + 4) {
        memcpy(&val, data + 4, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
        (void)ret;
    }

    // AV1_SET_TILE_MODE
    if (size >= sizeof(int) + 5) {
        memcpy(&val, data + 5, sizeof(int));
        ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default encoder config
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic config values to ensure valid state
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    // Initialize encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the target control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}