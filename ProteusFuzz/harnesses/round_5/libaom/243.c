// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

/* Helper to initialize an encoder context for testing */
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    aom_image_t img;
    unsigned int frame_cnt = 0;
    unsigned int width, height;
    int i;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Determine dimensions from input data if possible, otherwise default */
    if (size >= 4) {
        width = (data[0] % 16 + 1) * 16;
        height = (data[1] % 16 + 1) * 16;
        frame_cnt = (data[2] % 10) + 1;
    } else {
        width = 128;
        height = 128;
        frame_cnt = 1;
    }

    /* Ensure dimensions are valid */
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Encode a few frames to ensure context is fully initialized */
    for (i = 0; i < frame_cnt; i++) {
        aom_codec_cx_pkt_t pkt;
        memset(&img, 0, sizeof(img));
        aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
        
        /* Fill with fuzz data safely */
        /* Calculate the size of the Y plane strictly based on stride and height */
        size_t y_plane_size = (size_t)img.stride[0] * (size_t)img.h;
        size_t uv_plane_size = (size_t)img.stride[1] * (size_t)(img.h / 2);
        
        /* Copy Y plane data safely */
        if (y_plane_size > 0) {
            size_t copy_len = (size < y_plane_size) ? size : y_plane_size;
            memcpy(img.planes[0], data, copy_len);
            /* Zero out the rest if input is smaller than plane */
            if (copy_len < y_plane_size) {
                memset(img.planes[0] + copy_len, 0, y_plane_size - copy_len);
            }
        }

        /* Copy U and V planes safely */
        if (img.planes[1] && img.planes[2] && uv_plane_size > 0) {
            size_t copy_len_uv = (size < uv_plane_size) ? size : uv_plane_size;
            memcpy(img.planes[1], data, copy_len_uv);
            memcpy(img.planes[2], data, copy_len_uv);
            
            if (copy_len_uv < uv_plane_size) {
                memset(img.planes[1] + copy_len_uv, 0, uv_plane_size - copy_len_uv);
                memset(img.planes[2] + copy_len_uv, 0, uv_plane_size - copy_len_uv);
            }
        }

        res = aom_codec_encode(ctx, &img, i, 1, 0);
        if (res != AOM_CODEC_OK) {
            /* Continue to test error paths */
        }

        aom_img_free(&img);
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_243(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    /* Test 1: aom_codec_get_global_headers */
    /* This is the primary target that has full documentation */
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(ctx);
    if (headers) {
        /* Access data to ensure it's valid */
        if (headers->buf && headers->sz > 0) {
            volatile uint8_t dummy = *(uint8_t *)headers->buf;
            (void)dummy;
        }
        /* Free the buffer as per documentation */
        if (headers->buf) {
            free(headers->buf);
        }
        free(headers);
    }

    /* Test 2: aom_codec_error */
    /* Test with valid context */
    const char *err_msg = aom_codec_error(ctx);
    if (err_msg) {
        volatile char dummy = err_msg[0];
        (void)dummy;
    }

    /* Test 3: aom_codec_error with NULL context */
    const char *null_err_msg = aom_codec_error(NULL);
    if (null_err_msg) {
        volatile char dummy2 = null_err_msg[0];
        (void)dummy2;
    }

    /* Cleanup */
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}