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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int ctrl, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, ctrl, value);
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = Data[0] > 0 ? Data[0] : 64;
    cfg.g_h = Data[1] > 0 ? Data[1] : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int value = 0;
    if (Size > 2) {
        value = Data[2] % 2; // 0 or 1
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_encoder_control(&codec, AV1E_SET_ENABLE_FLIP_IDTX, value);

    // Fuzz AV1E_SET_ENABLE_TX64
    fuzz_encoder_control(&codec, AV1E_SET_ENABLE_TX64, value);

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    fuzz_encoder_control(&codec, AV1E_SET_INTER_DCT_ONLY, value);

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    fuzz_encoder_control(&codec, AV1E_SET_INTRA_DCT_ONLY, value);

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    fuzz_encoder_control(&codec, AV1E_SET_ENABLE_WARPED_MOTION, value);

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    fuzz_encoder_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, value);

    aom_codec_destroy(&codec);
    return 0;
}