// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_SHARPNESS__value at vp8cx.h:990:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = NULL;
    int ret;

    // Try VP8 encoder
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        iface = vpx_codec_vp8_dx();
        if (!iface) return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_lag_in_frames = 0;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Extract control values from input data if available
    int arnr_strength = 0;
    int noise_sensitivity = 0;
    int frame_parallel_decoding = 0;
    int sharpness = 0;
    int screen_content_mode = 0;
    int disable_loopfilter = 0;

    size_t idx = 0;
    if (idx < Size) arnr_strength = Data[idx++];
    if (idx < Size) noise_sensitivity = Data[idx++];
    if (idx < Size) frame_parallel_decoding = Data[idx++];
    if (idx < Size) sharpness = Data[idx++];
    if (idx < Size) screen_content_mode = Data[idx++];
    if (idx < Size) disable_loopfilter = Data[idx++];

    // Clamp values to reasonable ranges to avoid invalid states
    if (arnr_strength < 0) arnr_strength = 0;
    if (arnr_strength > 10) arnr_strength = 10;

    if (noise_sensitivity < 0) noise_sensitivity = 0;
    if (noise_sensitivity > 100) noise_sensitivity = 100;

    if (frame_parallel_decoding < 0) frame_parallel_decoding = 0;
    if (frame_parallel_decoding > 1) frame_parallel_decoding = 1;

    if (sharpness < 0) sharpness = 0;
    if (sharpness > 7) sharpness = 7;

    if (screen_content_mode < 0) screen_content_mode = 0;
    if (screen_content_mode > 1) screen_content_mode = 1;

    if (disable_loopfilter < 0) disable_loopfilter = 0;
    if (disable_loopfilter > 1) disable_loopfilter = 1;

    // VP8 controls
    ret = vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    (void)ret;

    ret = vpx_codec_control(&ctx, VP8E_SET_SHARPNESS, sharpness);
    (void)ret;

    ret = vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_content_mode);
    (void)ret;

    // VP9 controls (if using VP9 encoder, we would need to re-init with VP9 iface)
    // Since we initialized with VP8, these may fail, which is acceptable for fuzzing
    ret = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    (void)ret;

    ret = vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding);
    (void)ret;

    ret = vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter);
    (void)ret;

    // Create a dummy frame to encode
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, 64, 64, 1);

    // Fill image with fuzz data
    for (int i = 0; i < 64 * 64; i++) {
        img.planes[0][i] = Data[i % Size];
        img.planes[1][i / 4] = Data[(i / 4) % Size];
        img.planes[2][i / 4] = Data[((i / 4) + 1) % Size];
    }

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}