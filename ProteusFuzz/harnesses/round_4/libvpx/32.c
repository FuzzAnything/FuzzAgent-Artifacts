// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_image_t *create_test_image(const uint8_t *data, size_t size, unsigned int w, unsigned int h) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;

    size_t plane_size = img->planes[0] ? (size_t)img->stride[0] * img->h : 0;
    if (plane_size > 0 && size > 0) {
        size_t copy_size = (size < plane_size) ? size : plane_size;
        memcpy(img->planes[0], data, copy_size);
        if (copy_size < plane_size) {
            memset(img->planes[0] + copy_size, 0, plane_size - copy_size);
        }
    }

    size_t uv_size = img->planes[1] ? (size_t)img->stride[1] * img->h : 0;
    if (uv_size > 0) {
        memset(img->planes[1], 128, uv_size);
        memset(img->planes[2], 128, uv_size);
    }

    return img;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    vpx_codec_iface_t *iface;

    // Initialize encoder context with VP8
    // vp8_cx() is defined in vpx/vp8cx.h
    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    err = vpx_codec_enc_init(&ctx, iface, NULL, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fuzz configuration parameters safely
    unsigned int w = 16;
    unsigned int h = 16;
    if (Size >= 2) {
        w = (Data[0] % 16) + 16;
        h = (Data[1] % 16) + 16;
        if (w > 2048) w = 2048;
        if (h > 2048) h = 2048;
    }
    cfg.g_w = w;
    cfg.g_h = h;
    
    if (Size > 2) cfg.g_threads = Data[2] % 8;
    if (Size > 3) cfg.rc_target_bitrate = (Data[3] % 100) + 100;
    if (Size > 4) cfg.g_profile = Data[4] % 4;
    if (Size > 5) cfg.g_bit_depth = (vpx_bit_depth_t)(Data[5] % 3); // 8, 10, 12
    if (Size > 6) cfg.rc_end_usage = (enum vpx_rc_mode)(Data[6] % 4);
    if (Size > 7) cfg.kf_mode = (enum vpx_kf_mode)(Data[7] % 3);
    if (Size > 8) cfg.kf_min_dist = Data[8] % 100;
    if (Size > 9) cfg.kf_max_dist = (Data[9] % 100) + 100;

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Set configuration
    err = vpx_codec_enc_config_set(&ctx, &cfg);
    if (err != VPX_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Create image
    vpx_image_t *img = create_test_image(Data, Size, w, h);
    if (!img) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fuzz encoding parameters
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;

    if (Size > 10) pts = (int64_t)(Data[10]);
    if (Size > 11) duration = Data[11];
    if (Size > 12) flags = (long)(Data[12]);
    if (Size > 13) deadline = (unsigned long)(Data[13]);

    // Test vpx_codec_encode with valid image
    err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    
    // Test vpx_codec_encode with NULL image (flush)
    err = vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);

    // Test vpx_codec_control with various control IDs
    // VP8E_SET_TILE_COLUMNS is typically 0x1001
    if (Size > 14) {
        int tile_cols = Data[14] % 16;
        vpx_codec_control_(&ctx, 0x1001, tile_cols);
    }
    
    // VP8E_SET_SVC is typically 0x1002
    if (Size > 15) {
        int svc = Data[15] % 2;
        vpx_codec_control_(&ctx, 0x1002, svc);
    }

    // VP8E_SET_KEY_FRAME_FILTERING is typically 0x1003
    if (Size > 16) {
        int kf_filter = Data[16] % 2;
        vpx_codec_control_(&ctx, 0x1003, kf_filter);
    }

    // Test with invalid control ID
    vpx_codec_control_(&ctx, -1, 0);

    // Cleanup
    vpx_img_free(img);
    cleanup_encoder(&ctx);

    return 0;
}