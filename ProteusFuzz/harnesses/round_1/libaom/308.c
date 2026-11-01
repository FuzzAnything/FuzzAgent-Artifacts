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
#include <aom/aom.h>

static int try_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, ctrl_id, value);
}

int LLVMFuzzerTestOneInput_308(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure the encoder safely
    cfg.g_w = (Data[0] % 256) + 1;
    cfg.g_h = (Data[1] % 256) + 1;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to fuzz the control parameters
    int val1 = (int)(Data[0] % 2);
    int val2 = (int)(Data[1] % 64);
    int val3 = (int)(Data[2] % 2);
    int val4 = (int)(Data[3] % 100);
    int val5 = (int)(Data[4] % 10);
    int val6 = (int)(Data[5] % 4);

    // Fuzz AV1E_SET_ENABLE_QM
    try_control(&ctx, AV1E_SET_ENABLE_QM, val1);

    // Fuzz AV1E_SET_QM_MAX
    try_control(&ctx, AV1E_SET_QM_MAX, val2);

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    try_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val3);

    // Fuzz AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP
    try_control(&ctx, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, val4);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    try_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val5);

    // Fuzz AV1E_SET_DELTAQ_MODE
    try_control(&ctx, AV1E_SET_DELTAQ_MODE, val6);

    aom_codec_destroy(&ctx);
    return 0;
}