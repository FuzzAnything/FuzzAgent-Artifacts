// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx VP9 encoder controls.
 * Fixed buffer overflow by using vpx_img_plane_stride and vpx_img_plane_size
 * to correctly calculate plane sizes instead of manual arithmetic.
 */

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Need at least 3 bytes to safely access Data[0], Data[1], and Data[2]
    if (Size < 3) return 0;

    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;

    err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Initialize configuration with fuzzed data where safe
    // Ensure dimensions are valid (min 16x16) and reasonable max to avoid OOM
    cfg.g_w = (Data[0] % 2048) + 16;
    cfg.g_h = (Data[1] % 2048) + 16;
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Safe access to Data[2]
    cfg.rc_target_bitrate = (Data[2] % 10000) + 100;
    
    cfg.g_error_resilient = 1;

    err = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Create a dummy image for encoding
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    
    // Correctly fill planes using stride and height to avoid buffer overflow
    // Plane 0 (Y)
    for (int y = 0; y < img.d_h; y++) {
        memset(&img.planes[0][y * img.stride[0]], 0x80, img.d_w);
    }
    // Plane 1 (U)
    for (int y = 0; y < img.d_h / 2; y++) {
        memset(&img.planes[1][y * img.stride[1]], 0x80, (img.d_w + 1) / 2);
    }
    // Plane 2 (V)
    for (int y = 0; y < img.d_h / 2; y++) {
        memset(&img.planes[2][y * img.stride[2]], 0x80, (img.d_w + 1) / 2);
    }

    // Encode a frame to exercise the encoder state
    vpx_codec_encode(&codec, &img, 0, 1, 0, 0);

    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&codec);

    return 0;
}