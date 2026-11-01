// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret; // Suppress unused variable warning
    return 0;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;

    // Initialize codec context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Set default encoder configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder with minimal settings
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_usage = AOM_CQ; // Fixed: AOM_CQP -> AOM_CQ
    cfg.rc_target_bitrate = 100;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from input data
    int val1 = Data[0];
    int val2 = Data[1];
    int val3 = Data[0] ^ Data[1];
    int val4 = (Size > 2) ? Data[2] : 0;
    int val5 = (Size > 3) ? Data[3] : 0;

    // Fuzz AV1E_SET_QM_MAX
    fuzz_encoder_control(&ctx, AV1E_SET_QM_MAX, val1);

    // Fuzz AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val2);

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val3);

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val4);

    // Fuzz AV1E_SET_FP_MT_UNIT_TEST
    fuzz_encoder_control(&ctx, AV1E_SET_FP_MT_UNIT_TEST, val5);

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    fuzz_encoder_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val1 + val2);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}