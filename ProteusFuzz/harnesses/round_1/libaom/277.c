// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_277(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    int val;
    if (Size >= 1) {
        val = Data[0];
    } else {
        val = 0;
    }

    // Fuzz AV1E_SET_QM_MIN
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);

    // Fuzz AV1E_SET_MIN_CR
    aom_codec_control(&ctx, AV1E_SET_MIN_CR, val);

    // Fuzz AV1E_SET_FP_MT
    aom_codec_control(&ctx, AV1E_SET_FP_MT, val);

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_X
    aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);

    // Note: AV1E_SET_ENABLE_DIST_8X8 does not exist in the current libaom API.
    // Skipping this control to ensure compilation.
    // If a similar feature is needed, AV1E_SET_ENABLE_RECT_TX could be considered,
    // but strictly following the request to fuzz the specific named function
    // which is invalid, we omit it to prevent compilation errors.

    aom_codec_destroy(&ctx);

    return 0;
}