// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vp8cx.h"
#include "vp8dx.h"

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) {
        return 0;
    }
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    // Initialize encoder context
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        cleanup_encoder(ctx);
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        cleanup_encoder(ctx);
        return 0;
    }

    // Set some basic configuration from fuzz input
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != 0) {
        cleanup_encoder(ctx);
        return 0;
    }

    // Create a dummy frame to encode
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 16, 16, 1);
    memset(img.planes[0], Data[0], 16 * 16);
    memset(img.planes[1], Data[0] ^ 0xFF, 8 * 8);
    memset(img.planes[2], Data[0] ^ 0x55, 8 * 8);

    // Invoke target API functions in the specified order
    // 1. VP9E_SET_FRAME_PERIODIC_BOOST__value
    unsigned int boost_val = Data[0];
    if (vpx_codec_control(ctx, VP9E_SET_FRAME_PERIODIC_BOOST, boost_val) != 0) {
        // Continue even if control fails to explore more states
    }

    // 2. VP9E_SET_NOISE_SENSITIVITY__value
    unsigned int noise_val = Data[1] % 64; // Clamp to reasonable range
    if (vpx_codec_control(ctx, VP9E_SET_NOISE_SENSITIVITY, noise_val) != 0) {
        // Continue
    }

    // 3. VP8E_SET_STATIC_THRESHOLD__value
    unsigned int static_thresh_val = Data[2] % 256;
    if (vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, static_thresh_val) != 0) {
        // Continue
    }

    // 4. VP9E_SET_TUNE_CONTENT__value
    // Tune content types: 0=VP9E_TUNE_UNSET, 1=VP9E_TUNE_FILM, 2=VP9E_TUNE_SCREEN
    unsigned int tune_content = Data[3] % 3;
    if (vpx_codec_control(ctx, VP9E_SET_TUNE_CONTENT, tune_content) != 0) {
        // Continue
    }

    // 5. VP9E_SET_TILE_COLUMNS__value
    unsigned int tile_cols = Data[4] % 8; // Max 8 columns typically
    if (tile_cols == 0) tile_cols = 1; // At least 1 column
    if (vpx_codec_control(ctx, VP9E_SET_TILE_COLUMNS, tile_cols) != 0) {
        // Continue
    }

    // Attempt to encode a frame to trigger internal logic
    vpx_codec_encode(ctx, &img, 0, 1, 0, 0);

    // Cleanup
    vpx_img_free(&img);
    cleanup_encoder(ctx);

    return 0;
}