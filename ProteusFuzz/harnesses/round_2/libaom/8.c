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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t err;
    int ret = 0;

    if (Size < 1) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_QM_MIN
    if (Size >= 1) {
        int qm_min = Data[0];
        err = aom_codec_control(&ctx, AV1E_SET_QM_MIN, qm_min);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz AV1E_SET_AQ_MODE
    if (Size >= 2) {
        int aq_mode = Data[1];
        err = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    if (Size >= 3) {
        int deltaq_strength = Data[2];
        err = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz AV1E_SET_ENABLE_QM
    if (Size >= 4) {
        int enable_qm = Data[3];
        err = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz AV1E_SET_QM_MAX
    if (Size >= 5) {
        int qm_max = Data[4];
        err = aom_codec_control(&ctx, AV1E_SET_QM_MAX, qm_max);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz AV1E_SET_QM_Y
    if (Size >= 6) {
        int qm_y = Data[5];
        err = aom_codec_control(&ctx, AV1E_SET_QM_Y, qm_y);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    cleanup_encoder(&ctx);

    return ret;
}