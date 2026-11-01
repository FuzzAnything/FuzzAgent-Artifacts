// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    
    // AV1E_ENABLE_EXT_TILE_DEBUG
    if (size >= 1) {
        int val = data[0];
        ret = aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
        (void)ret;
    }

    // AV1E_SET_SINGLE_TILE_DECODING
    if (size >= 2) {
        int val = data[1];
        ret = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
        (void)ret;
    }

    // AV1_SET_TILE_MODE
    if (size >= 3) {
        int val = data[2];
        ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, val);
        (void)ret;
    }

    // AV1E_SET_TILE_ROWS
    if (size >= 4) {
        int val = data[3];
        ret = aom_codec_control(ctx, AV1E_SET_TILE_ROWS, val);
        (void)ret;
    }

    // AV1E_SET_AUTO_TILES
    if (size >= 5) {
        int val = data[4];
        ret = aom_codec_control(ctx, AV1E_SET_AUTO_TILES, val);
        (void)ret;
    }

    // AV1E_SET_TILE_COLUMNS
    if (size >= 6) {
        int val = data[5];
        ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, val);
        (void)ret;
    }

    return 0;
}

static int fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;

    // AV1_SET_TILE_MODE is available for decoders as well
    if (size >= 1) {
        int val = data[0];
        ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Determine if we are testing encoder or decoder based on first byte
    int is_encoder = Data[0] & 0x01;
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;

    if (is_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        res = aom_codec_enc_config_default(iface, &cfg, 0);
        if (res != AOM_CODEC_OK) return 0;

        // Use data to configure width/height if possible
        if (Size >= 4) {
            cfg.g_w = Data[1] + 16;
            cfg.g_h = Data[2] + 16;
            cfg.g_timebase.num = Data[3] + 1;
            cfg.g_timebase.den = 30;
        } else {
            cfg.g_w = 16;
            cfg.g_h = 16;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
        }

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_encoder_controls(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;

        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_decoder_controls(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    }

    return 0;
}