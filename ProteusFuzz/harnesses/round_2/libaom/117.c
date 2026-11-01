// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder(&ctx)) {
        return 0;
    }

    // Interpret input data to control parameters
    int val1 = Data[0];
    int val2 = Size > 1 ? Data[1] : 0;
    int val3 = Size > 2 ? Data[2] : 0;
    int val4 = Size > 3 ? Data[3] : 0;
    int val5 = Size > 4 ? Data[4] : 0;
    int val6 = Size > 5 ? Data[5] : 0;

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val1);

    // Fuzz AV1E_SET_QM_MIN
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val2);

    // Fuzz AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val3);

    // Fuzz AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP
    aom_codec_control(&ctx, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, val4);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val5);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val6);

    cleanup_codec(&ctx);
    return 0;
}