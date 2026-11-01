// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    (void)err; // Ignore error for fuzzing, just want to trigger code paths
    return 1;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to vary configuration slightly
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control commands with values derived from input
    // AV1E_SET_INTER_DCT_ONLY
    if (Size > 2) {
        int val = Data[2] % 2;
        try_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val);
    }

    // AV1E_SET_ALLOW_REF_FRAME_MVS
    if (Size > 3) {
        int val = Data[3] % 2;
        try_control(&ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, val);
    }

    // AV1E_SET_ERROR_RESILIENT_MODE
    if (Size > 4) {
        int val = Data[4] % 4; // Assuming enum range 0-3
        try_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val);
    }

    // AOME_SET_CPUUSED
    if (Size > 5) {
        int val = Data[5] % 8; // Typical range 0-7
        try_control(&ctx, AOME_SET_CPUUSED, val);
    }

    // AV1E_SET_BITRATE_ONE_PASS_CBR
    if (Size > 6) {
        int val = Data[6] % 2;
        try_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, val);
    }

    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    if (Size > 7) {
        int val = Data[7] % 2;
        try_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);
    }

    // Try to encode a dummy frame to exercise more state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 128, cfg.g_w * cfg.g_h / 4);
    memset(img.planes[2], 128, cfg.g_w * cfg.g_h / 4);

    // Fixed: aom_codec_encode expects 5 arguments, not 6
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}