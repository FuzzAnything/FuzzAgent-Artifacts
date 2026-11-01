// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int result = 0;

    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to set configuration parameters safely
    if (Size > 0) cfg.g_w = Data[0] ? Data[0] : 64;
    if (Size > 1) cfg.g_h = Data[1] ? Data[1] : 64;
    if (Size > 2) cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] ? Data[3] : 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder control functions
    // AV1E_ENABLE_EXT_TILE_DEBUG
    if (Size > 4) {
        int val = (Data[4] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
    }

    // AV1E_SET_TILE_ROWS
    if (Size > 5) {
        int val = (Data[5] % 16) + 1;
        aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val);
    }

    // AV1E_SET_AUTO_TILES
    if (Size > 6) {
        int val = (Data[6] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_AUTO_TILES, val);
    }

    // AV1E_SET_TILE_COLUMNS
    if (Size > 7) {
        int val = (Data[7] % 16) + 1;
        aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, val);
    }

    // AV1E_SET_SINGLE_TILE_DECODING
    if (Size > 8) {
        int val = (Data[8] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
    }

    aom_codec_destroy(&ctx);
    return result;
}

static int fuzz_decoder(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;
    int result = 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Removed invalid members allow_highbitdepth, allow_multiref, allow_screencontenttools
    // The struct aom_codec_dec_cfg in this version does not contain these fields.
    // cfg.allow_lowbitdepth is available but not strictly required for basic init.

    ret = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz decoder control functions
    // AV1D_EXT_TILE_DEBUG
    if (Size > 0) {
        int val = (Data[0] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, val);
    }

    aom_codec_destroy(&ctx);
    return result;
}

int LLVMFuzzerTestOneInput_232(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Alternate between encoder and decoder fuzzing based on input
    if (Data[0] % 2 == 0) {
        fuzz_encoder(Data, Size);
    } else {
        fuzz_decoder(Data, Size);
    }

    return 0;
}