// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control_int(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    if (ctx->err != AOM_CODEC_OK) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 1;
}

static int fuzz_encoder_control_int64(aom_codec_ctx_t *ctx, int control_id, int64_t value) {
    if (!ctx) return 0;
    if (ctx->err != AOM_CODEC_OK) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    cfg.g_w = Data[0] + 16;
    cfg.g_h = Data[1] + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.g_lag_in_frames = (Data[5] % 10) + 1;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    int control_id;
    int64_t value;

    switch (Data[6] % 6) {
        case 0:
            control_id = AV1E_SET_MIN_GF_INTERVAL;
            value = Data[7];
            fuzz_encoder_control_int(&ctx, control_id, (int)value);
            break;
        case 1:
            control_id = AV1E_SET_QM_MIN;
            value = Data[7];
            fuzz_encoder_control_int(&ctx, control_id, (int)value);
            break;
        case 2:
            control_id = AV1E_SET_AQ_MODE;
            value = Data[7] % 4;
            fuzz_encoder_control_int(&ctx, control_id, (int)value);
            break;
        case 3:
            control_id = AV1E_SET_QM_Y;
            value = Data[7];
            fuzz_encoder_control_int(&ctx, control_id, (int)value);
            break;
        case 4:
            control_id = AV1E_SET_CDF_UPDATE_MODE;
            value = Data[7] % 2;
            fuzz_encoder_control_int(&ctx, control_id, (int)value);
            break;
        case 5:
            control_id = AV1E_SET_MODE_COST_UPD_FREQ;
            value = Data[7];
            fuzz_encoder_control_int(&ctx, control_id, (int)value);
            break;
        default:
            break;
    }

    aom_codec_destroy(&ctx);
    return 0;
}