// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_encoder(void) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Use the standard interface getter for AV1 encoder
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    codec = malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *codec = init_encoder();
    if (!codec) return 0;

    int value1 = (int)((Data[0] % 2) - 1); // -1 or 0 or 1
    int value2 = (int)(Data[1] % 256);
    int value3 = (int)(Data[2 % Size] % 4);
    int value4 = (int)(Data[3 % Size] % 256);
    int value5 = (int)(Data[4 % Size] % 16);
    int value6 = (int)(Data[5 % Size] % 2);

    // AV1E_SET_ENABLE_QM
    aom_codec_control(codec, AV1E_SET_ENABLE_QM, value1);

    // AV1E_SET_NOISE_SENSITIVITY
    aom_codec_control(codec, AV1E_SET_NOISE_SENSITIVITY, value2);

    // AV1E_SET_AQ_MODE
    aom_codec_control(codec, AV1E_SET_AQ_MODE, value3);

    // AV1E_SET_QM_MAX
    aom_codec_control(codec, AV1E_SET_QM_MAX, value4);

    // AV1E_SET_DELTAQ_STRENGTH
    aom_codec_control(codec, AV1E_SET_DELTAQ_STRENGTH, value5);

    // AV1E_ENABLE_RATE_GUIDE_DELTAQ
    aom_codec_control(codec, AV1E_ENABLE_RATE_GUIDE_DELTAQ, value6);

    cleanup_encoder(codec);
    return 0;
}