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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Use the correct interface getter for AV1 encoder
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    // Fuzz configuration parameters safely
    cfg.g_w = (size > 0) ? (data[0] % 256) + 16 : 128;
    cfg.g_h = (size > 1) ? (data[1] % 256) + 16 : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (size > 2) ? (data[2] % 1000) + 100 : 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;

    if (Size == 0) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    if (Size > 0) {
        aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, Data[0]);
    }

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    if (Size > 1) {
        aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, Data[1]);
    }

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    if (Size > 2) {
        aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, Data[2]);
    }

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    if (Size > 3) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, Data[3] % 2);
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (Size > 4) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, Data[4] % 2);
    }

    // Fuzz AOME_SET_CQ_LEVEL
    if (Size > 5) {
        aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, Data[5] % 64);
    }

    cleanup_codec(&ctx);
    return 0;
}