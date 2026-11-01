// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int initialize_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int initialized = 0;
    aom_codec_err_t ret;
    int idx;

    if (Size < 1) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (initialize_encoder(&ctx)) {
        initialized = 1;
    } else {
        return 0;
    }

    // Use input data to select which control to invoke and parameters
    idx = Data[0] % 6;

    if (idx == 0) {
        // aom_codec_control_typechecked_AV1E_SET_QM_Y
        int val = 0;
        if (Size > 1) val = Data[1];
        ret = aom_codec_control(&ctx, AV1E_SET_QM_Y, &val);
        (void)ret;
    } else if (idx == 1) {
        // aom_codec_control_typechecked_AV1E_SET_AQ_MODE
        int val = 0;
        if (Size > 1) val = Data[1] % 5; // AQ modes usually 0-4
        ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, &val);
        (void)ret;
    } else if (idx == 2) {
        // aom_codec_control_typechecked_AV1E_SET_ENABLE_QM
        int val = 0;
        if (Size > 1) val = Data[1] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, &val);
        (void)ret;
    } else if (idx == 3) {
        // aom_codec_control_typechecked_AV1E_SET_QM_MAX
        int val = 0;
        if (Size > 1) val = Data[1];
        ret = aom_codec_control(&ctx, AV1E_SET_QM_MAX, &val);
        (void)ret;
    } else if (idx == 4) {
        // aom_codec_control_typechecked_AV1E_SET_QM_V
        int val = 0;
        if (Size > 1) val = Data[1];
        ret = aom_codec_control(&ctx, AV1E_SET_QM_V, &val);
        (void)ret;
    } else if (idx == 5) {
        // aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH
        int val = 0;
        if (Size > 1) val = Data[1] % 8; // Typical range 0-7
        ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, &val);
        (void)ret;
    }

    cleanup_codec(&ctx);
    return 0;
}