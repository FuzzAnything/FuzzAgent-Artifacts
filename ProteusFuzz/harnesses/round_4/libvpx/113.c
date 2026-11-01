// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static int create_test_image(vpx_image_t *img, unsigned int w, unsigned int h, vpx_img_fmt_t fmt) {
    if (img == NULL) return 0;
    vpx_img_alloc(img, fmt, w, h, 16);
    if (img->planes[0] == NULL) return 0;
    // Fill with simple gradient pattern to ensure valid data
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            img->planes[0][y * img->stride[0] + x] = (unsigned char)((x + y) & 0xFF);
        }
    }
    // Fill U and V planes if present
    if (img->planes[1] != NULL) {
        int u_w = w >> img->x_chroma_shift;
        int u_h = h >> img->y_chroma_shift;
        for (int y = 0; y < u_h; y++) {
            for (int x = 0; x < u_w; x++) {
                img->planes[1][y * img->stride[1] + x] = 128;
            }
        }
    }
    if (img->planes[2] != NULL) {
        int v_w = w >> img->x_chroma_shift;
        int v_h = h >> img->y_chroma_shift;
        for (int y = 0; y < v_h; y++) {
            for (int x = 0; x < v_w; x++) {
                img->planes[2][y * img->stride[2] + x] = 128;
            }
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Parse input to determine test parameters
    uint32_t seed = *(uint32_t *)Data;
    uint32_t width = (seed % 256) + 16;
    uint32_t height = ((seed >> 8) % 256) + 16;
    uint32_t mode = (seed >> 16) % 4;
    uint32_t flags = (seed >> 24) % 8;

    // Ensure dimensions are multiples of 16 for VP8/VP9
    width = (width / 16) * 16;
    height = (height / 16) * 16;
    if (width < 16 || height < 16) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface = NULL;

    // Select codec based on mode. 
    // Note: vp9cx.h is not available in the include path, so we only test VP8.
    // If the build environment had vp9cx.h, we would use vpx_codec_vp9_cx() here.
    // For now, we force VP8 to ensure compilation.
    iface = vpx_codec_vp8_cx();

    if (iface == NULL) return 0;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    // Configure encoder
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = VPX_VBR;
    cfg.kf_mode = VPX_KF_DISABLED;

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    vpx_image_t img;
    vpx_img_fmt_t fmt = (mode % 2 == 0) ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_NV12;
    
    if (!create_test_image(&img, width, height, fmt)) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz vpx_codec_set_cx_data_buf
    uint8_t custom_buf[4096];
    vpx_fixed_buf_t buf = { .buf = custom_buf, .sz = sizeof(custom_buf) };
    unsigned int pad_before = (seed % 100);
    unsigned int pad_after = ((seed >> 8) % 100);
    
    res = vpx_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
    // Ignore error, might fail if buffer too small or logic invalid, but safe to call

    // Fuzz vpx_codec_enc_config_set with modified config
    vpx_codec_enc_cfg_t new_cfg = cfg;
    new_cfg.rc_target_bitrate = (seed % 10000) + 1;
    new_cfg.kf_min_dist = (seed % 100);
    res = vpx_codec_enc_config_set(&ctx, &new_cfg);
    // Ignore error, config changes might be rejected mid-stream

    // Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t enc_flags = flags;
    vpx_enc_deadline_t deadline = (seed % 1000000);

    // Encode a few frames
    for (int i = 0; i < 3; i++) {
        res = vpx_codec_encode(&ctx, &img, pts, duration, enc_flags, deadline);
        if (res == VPX_CODEC_INVALID_PARAM) {
            // Try with NULL img to flush if encoding failed due to param
            break;
        }
        pts += duration;
    }

    // Flush the encoder
    res = vpx_codec_encode(&ctx, NULL, pts, duration, 0, 0);

    // Drain output
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        // Consume packets
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Do nothing with data, just consume
        }
    }

    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}