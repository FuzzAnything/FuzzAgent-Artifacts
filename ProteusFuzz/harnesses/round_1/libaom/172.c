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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_172(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int32_t param;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Extract parameters from input data to diversify fuzzing
    if (Size >= 4) {
        param = *(const int32_t *)Data;
    } else {
        param = 0;
    }

    // Fuzz AV1E_SET_ENABLE_QM
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, param);
    (void)ret;

    // Fuzz AV1E_SET_QM_MAX
    ret = aom_codec_control(&ctx, AV1E_SET_QM_MAX, param);
    (void)ret;

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, param);
    (void)ret;

    // Fuzz AV1E_SET_AQ_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, param);
    (void)ret;

    // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
    ret = aom_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, param);
    (void)ret;

    // Fuzz AV1E_SET_DELTAQ_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, param);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}