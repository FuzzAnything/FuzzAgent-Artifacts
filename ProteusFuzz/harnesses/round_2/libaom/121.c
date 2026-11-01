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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    aom_codec_err_t res;
    res = aom_codec_control(ctx, ctrl_id, value);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    int val = (Data[0] % 2);
    int control_choice = Data[1] % 6;

    switch (control_choice) {
        case 0:
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
            break;
        case 1:
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);
            break;
        case 2:
            fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
            break;
        case 3:
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
            break;
        case 4:
            fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val);
            break;
        case 5:
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
            break;
        default:
            break;
    }

    aom_codec_destroy(&ctx);

    return 0;
}