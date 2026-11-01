// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

// Define the control IDs if they are not available in the headers or macros
#ifndef AV1E_SET_DELTAQ_MODE
#define AV1E_SET_DELTAQ_MODE 1001
#endif
#ifndef AV1E_SET_COEFF_COST_UPD_FREQ
#define AV1E_SET_COEFF_COST_UPD_FREQ 1002
#endif
#ifndef AV1E_SET_FRAME_PERIODIC_BOOST
#define AV1E_SET_FRAME_PERIODIC_BOOST 1003
#endif
#ifndef AV1E_SET_ENABLE_TPL_MODEL
#define AV1E_SET_ENABLE_TPL_MODEL 1004
#endif
#ifndef AOME_SET_CQ_LEVEL
#define AOME_SET_CQ_LEVEL 1005
#endif
#ifndef AOME_SET_ENABLEAUTOBWDREF
#define AOME_SET_ENABLEAUTOBWDREF 1006
#endif

// Helper macro to construct control ID if AOM_CODEC_CTRL_ID is missing
#ifndef AOM_CODEC_CTRL_ID
#define AOM_CODEC_CTRL_ID(x) (x)
#endif

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    int32_t val;
    size_t offset = 0;

    // Control 1: AV1E_SET_DELTAQ_MODE
    if (size - offset >= sizeof(int32_t)) {
        memcpy(&val, &data[offset], sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_DELTAQ_MODE), val);
    }

    // Control 2: AV1E_SET_COEFF_COST_UPD_FREQ
    if (size - offset >= sizeof(int32_t)) {
        memcpy(&val, &data[offset], sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_COEFF_COST_UPD_FREQ), val);
    }

    // Control 3: AV1E_SET_FRAME_PERIODIC_BOOST
    if (size - offset >= sizeof(int32_t)) {
        memcpy(&val, &data[offset], sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_FRAME_PERIODIC_BOOST), val);
    }

    // Control 4: AV1E_SET_ENABLE_TPL_MODEL
    if (size - offset >= sizeof(int32_t)) {
        memcpy(&val, &data[offset], sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_ENABLE_TPL_MODEL), val);
    }

    // Control 5: AOME_SET_CQ_LEVEL
    if (size - offset >= sizeof(int32_t)) {
        memcpy(&val, &data[offset], sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AOME_SET_CQ_LEVEL), val);
    }

    // Control 6: AOME_SET_ENABLEAUTOBWDREF
    if (size - offset >= sizeof(int32_t)) {
        memcpy(&val, &data[offset], sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AOME_SET_ENABLEAUTOBWDREF), val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Initialize encoder config
    // Use the standard AV1 encoder interface via aom_codec_av1_cx() from aomcx.h
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;

    // Initialize codec
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy image
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * img.d_h);
    memset(img.planes[1], 128, img.stride[1] * img.d_h);
    memset(img.planes[2], 128, img.stride[2] * img.d_h);

    // Send a frame to ensure encoder is in a valid state
    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}