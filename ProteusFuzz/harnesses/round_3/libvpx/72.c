// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *get_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    vpx_codec_err_t res;

    if (!iface) return NULL;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return NULL;

    // Fuzz configuration parameters
    if (size > 0) cfg.g_w = (unsigned int)data[0] + 16;
    if (size > 1) cfg.g_h = (unsigned int)data[1] + 16;
    if (size > 2) cfg.g_timebase.num = (unsigned int)data[2] + 1;
    if (size > 3) cfg.g_timebase.den = (unsigned int)data[3] + 1;
    if (size > 4) cfg.rc_target_bitrate = (unsigned int)data[4] * 1000;
    if (size > 5) cfg.rc_dropframe_thresh = (unsigned int)data[5];
    if (size > 6) cfg.g_error_resilient = (unsigned int)data[6];
    if (size > 7) cfg.g_lag_in_frames = (unsigned int)data[7];

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return NULL;

    // Allocate on heap to return a valid pointer
    vpx_codec_ctx_t *heap_ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!heap_ctx) return NULL;
    memcpy(heap_ctx, &ctx, sizeof(vpx_codec_ctx_t));
    return heap_ctx;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t *ctx = get_encoder_context(Data, Size);
    if (!ctx) return 0;

    vpx_image_t *img = NULL;
    vpx_img_fmt_t fmt;
    unsigned int w, h, align;
    unsigned char *img_data = NULL;
    size_t img_size = 0;

    // Extract fuzz parameters
    fmt = (vpx_img_fmt_t)(Data[0] % 11); // 0 to 10, covering some formats
    w = (unsigned int)(Data[1] + 16);
    if (Size > 2) {
        h = (unsigned int)(Data[2] + 16);
    } else {
        h = 16;
    }
    if (Size > 3) {
        align = (unsigned int)(Data[3] % 16 + 1);
    } else {
        align = 1;
    }

    // Limit dimensions to prevent OOM
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;

    // Calculate required buffer size for vpx_img_wrap
    // Using a simplified estimation for safety
    img_size = (size_t)w * h * 3; 
    img_data = malloc(img_size);
    if (!img_data) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return 0;
    }

    // Initialize image data with fuzz data
    memset(img_data, 0, img_size);
    size_t copy_len = (Size > img_size) ? img_size : Size;
    memcpy(img_data, Data, copy_len);

    // Test vpx_img_alloc
    vpx_image_t img_alloc;
    vpx_image_t *img_alloc_ret = vpx_img_alloc(&img_alloc, fmt, w, h, align);
    if (img_alloc_ret) {
        // Test vpx_img_flip
        vpx_img_flip(img_alloc_ret);
        
        // Test vpx_img_set_rect
        unsigned int x = (Size > 4) ? Data[4] % w : 0;
        unsigned int y = (Size > 5) ? Data[5] % h : 0;
        unsigned int rw = (Size > 6) ? Data[6] % (w - x) : w - x;
        unsigned int rh = (Size > 7) ? Data[7] % (h - y) : h - y;
        vpx_img_set_rect(img_alloc_ret, x, y, rw, rh);
        
        vpx_img_free(img_alloc_ret);
    }

    // Test vpx_img_wrap
    vpx_image_t img_wrap;
    vpx_image_t *img_wrap_ret = vpx_img_wrap(&img_wrap, fmt, w, h, align, img_data);
    if (img_wrap_ret) {
        img = img_wrap_ret;
    }

    // Test vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = (Size > 8) ? Data[8] + 1 : 1;
    vpx_enc_frame_flags_t flags = (Size > 9) ? Data[9] : 0;
    vpx_enc_deadline_t deadline = (Size > 10) ? Data[10] * 1000 : 0;

    // Encode a few frames with increasing PTS
    for (int i = 0; i < 3 && img; i++) {
        vpx_codec_encode(ctx, img, pts, duration, flags, deadline);
        pts += duration;
    }

    // Flush the encoder
    vpx_codec_encode(ctx, NULL, 0, 0, 0, 0);

    // Cleanup
    if (img) {
        vpx_img_free(img);
    }
    free(img_data);
    vpx_codec_destroy(ctx);
    free(ctx);

    return 0;
}