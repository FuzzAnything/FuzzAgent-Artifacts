// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_206(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = (Data[0] & 0x3F) + 16;
    int height = (Data[1] & 0x3F) + 16;
    int bitrate = (Data[2] & 0x3F) + 100;
    int fps_num = (Data[3] & 0x3F) + 1;
    int fps_den = 1;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = bitrate;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_ENABLE_EXT_TILE_DEBUG
    if (Size > 4) {
        int val = Data[4] & 1;
        fuzz_encoder_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
    }

    // Fuzz AV1E_SET_TILE_ROWS
    if (Size > 5) {
        int val = Data[5] & 0x0F;
        fuzz_encoder_control(&ctx, AV1E_SET_TILE_ROWS, val);
    }

    // Fuzz AV1E_SET_AUTO_TILES
    if (Size > 6) {
        int val = Data[6] & 1;
        fuzz_encoder_control(&ctx, AV1E_SET_AUTO_TILES, val);
    }

    // Fuzz AV1_SET_TILE_MODE
    if (Size > 7) {
        int val = Data[7] & 0x0F;
        fuzz_encoder_control(&ctx, AV1_SET_TILE_MODE, val);
    }

    // Fuzz AV1E_SET_TILE_COLUMNS
    if (Size > 8) {
        int val = Data[8] & 0x0F;
        fuzz_encoder_control(&ctx, AV1E_SET_TILE_COLUMNS, val);
    }

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING
    if (Size > 9) {
        int val = Data[9] & 1;
        fuzz_encoder_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}