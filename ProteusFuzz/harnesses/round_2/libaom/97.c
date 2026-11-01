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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use the correct interface function for AV1 encoder
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration slightly for diversity
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    int disable_trellis_quant = (Data[4] % 2);
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis_quant);

    // Fuzz AV1E_SET_QM_MIN
    int qm_min = Data[5] % 64;
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, qm_min);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = Data[6] % 5;
    aom_codec_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);

    // Fuzz AV1E_SET_ENABLE_QM
    int enable_qm = (Data[7] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm);

    // Fuzz AV1E_SET_QM_MAX
    int qm_max = Data[8] % 64;
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, qm_max);

    // Fuzz AV1E_SET_QM_Y
    int qm_y = Data[9] % 64;
    aom_codec_control(&ctx, AV1E_SET_QM_Y, qm_y);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}