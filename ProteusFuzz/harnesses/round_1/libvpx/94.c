// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx_image.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

static vpx_image_t *create_test_image(const uint8_t *data, size_t size, vpx_img_fmt_t fmt, unsigned int w, unsigned int h) {
    if (w == 0 || h == 0 || w > 0x08000000 || h > 0x08000000) {
        return NULL;
    }

    vpx_image_t *img = vpx_img_alloc(NULL, fmt, w, h, 32);
    if (!img) {
        return NULL;
    }

    // Fill image data with fuzz input if available
    if (data && size > 0) {
        size_t plane_size = (size_t)img->h * img->stride[0];
        size_t total_size = plane_size;
        if (fmt == VPX_IMG_FMT_I420 || fmt == VPX_IMG_FMT_YV12 || fmt == VPX_IMG_FMT_NV12) {
            total_size += 2 * ((size_t)img->h >> 1) * img->stride[1];
        } else if (fmt == VPX_IMG_FMT_I422 || fmt == VPX_IMG_FMT_I444) {
            total_size += 2 * (size_t)img->h * img->stride[1];
        }

        size_t copy_size = (size < total_size) ? size : total_size;
        if (img->planes[0]) {
            memcpy(img->planes[0], data, copy_size);
        }
    }

    return img;
}

static void cleanup_image(vpx_image_t *img) {
    if (img) {
        vpx_img_free(img);
    }
}

static vpx_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Fuzz configuration parameters slightly
    if (size > 3) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = (data[1] % 512) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (size > 4) ? ((unsigned int)data[4] % 10000) + 100 : 100;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 1;
        cfg.rc_target_bitrate = 100;
    }

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 14) {
        return 0;
    }

    // Extract parameters from fuzz data
    unsigned int w = (Data[0] % 512) + 16;
    unsigned int h = (Data[1] % 512) + 16;
    unsigned int stride_align = (Data[2] % 65536) + 1;
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(Data[3] % 10);

    // Ensure valid format
    if (fmt < VPX_IMG_FMT_NONE || fmt > VPX_IMG_FMT_I44016) {
        fmt = VPX_IMG_FMT_I420;
    }

    // Test vpx_img_alloc
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, w, h, stride_align);
    if (img) {
        // Test vpx_img_set_rect
        // Ensure we have enough data to read indices 4, 5, 6, 7
        unsigned int x = 0, y = 0, rect_w = w, rect_h = h;
        if (Size > 7) {
            x = (Data[4] % (w / 2 + 1));
            y = (Data[5] % (h / 2 + 1));
            rect_w = (Data[6] % (w - x));
            rect_h = (Data[7] % (h - y));
        }
        
        if (rect_w > 0 && rect_h > 0 && x < w && y < h) {
            vpx_img_set_rect(img, x, y, rect_w, rect_h);
        }

        // Test vpx_img_flip
        vpx_img_flip(img);

        // Test vpx_img_wrap with existing buffer
        vpx_image_t *wrapped_img = NULL;
        
        // Calculate buffer size safely
        vpx_image_t *temp_img = vpx_img_alloc(NULL, fmt, w, h, stride_align);
        size_t buffer_size = 0;
        if (temp_img) {
            buffer_size = (size_t)temp_img->h * temp_img->stride[0];
            if (fmt == VPX_IMG_FMT_I420 || fmt == VPX_IMG_FMT_YV12 || fmt == VPX_IMG_FMT_NV12) {
                buffer_size += 2 * ((size_t)temp_img->h >> 1) * temp_img->stride[1];
            } else if (fmt == VPX_IMG_FMT_I422 || fmt == VPX_IMG_FMT_I444) {
                buffer_size += 2 * (size_t)temp_img->h * temp_img->stride[1];
            }
            vpx_img_free(temp_img);
        } else {
            buffer_size = 1024; // Fallback
        }

        unsigned char *buffer = malloc(buffer_size);
        if (buffer) {
            wrapped_img = vpx_img_wrap(NULL, fmt, w, h, stride_align, buffer);
            if (wrapped_img) {
                vpx_img_flip(wrapped_img);
                vpx_img_set_rect(wrapped_img, 0, 0, w, h);
            }
            free(buffer);
        }
        cleanup_image(wrapped_img);

        cleanup_image(img);
    }

    // Test vpx_codec_encode
    vpx_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (ctx) {
        vpx_image_t *encode_img = create_test_image(Data, Size, fmt, w, h);
        if (encode_img) {
            vpx_codec_pts_t pts = (vpx_codec_pts_t)((unsigned int)Data[8] * 256 + Data[9]);
            unsigned long duration = (unsigned long)((unsigned int)Data[10] * 256 + Data[11]);
            vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(Data[12] % 4);
            vpx_enc_deadline_t deadline = (vpx_enc_deadline_t)((unsigned int)Data[13] * 1000);

            // Encode frame
            vpx_codec_encode(ctx, encode_img, pts, duration, flags, deadline);

            // Flush encoder
            vpx_codec_encode(ctx, NULL, -1, 0, 0, 0);

            cleanup_image(encode_img);
        } else {
            // Try to flush without image
            vpx_codec_encode(ctx, NULL, -1, 0, 0, 0);
        }

        cleanup_encoder_context(ctx);
    }

    return 0;
}