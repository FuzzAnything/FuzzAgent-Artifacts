// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

static int fuzz_encoder_control_int_array(aom_codec_ctx_t *ctx, int control_id, int *arr, size_t len) {
    if (!ctx || !arr || len == 0) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, arr);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int op = Data[0] % 6;
    int val_int = (Data[1] << 8) | Data[2];
    int val_int2 = Data[3];

    switch (op) {
        case 0: // AV1E_SET_LOSSLESS
            fuzz_encoder_control(&ctx, AV1E_SET_LOSSLESS, val_int % 2);
            break;
        case 1: // AV1E_SET_ENABLE_OVERLAY
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val_int % 2);
            break;
        case 2: // AV1E_SET_ENABLE_PALETTE
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PALETTE, val_int % 2);
            break;
        case 3: // AV1_SET_TILE_MODE
            {
                int tile_cols = (val_int % 8) + 1;
                int tile_rows = (val_int2 % 8) + 1;
                int log_tile_cols = 0;
                int log_tile_rows = 0;
                while ((1 << log_tile_cols) < tile_cols) log_tile_cols++;
                while ((1 << log_tile_rows) < tile_rows) log_tile_rows++;
                int tile_mode = (log_tile_rows << 8) | log_tile_cols;
                fuzz_encoder_control(&ctx, AV1_SET_TILE_MODE, tile_mode);
            }
            break;
        case 4: // AV1E_SET_QM_MIN
            fuzz_encoder_control(&ctx, AV1E_SET_QM_MIN, val_int % 64);
            break;
        case 5: // AV1E_SET_MTU
            fuzz_encoder_control(&ctx, AV1E_SET_MTU, val_int % 1000 + 100);
            break;
    }

    aom_codec_destroy(&ctx);
    return 0;
}