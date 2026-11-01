// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>

/* Callback implementations for external frame buffers */
static int get_frame_buffer_cb(void *cb_priv, unsigned long frame_id, vpx_codec_frame_buffer_t *buf) {
    (void)cb_priv;
    (void)frame_id;
    (void)buf;
    return 0;
}

static int release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *buf) {
    (void)cb_priv;
    (void)buf;
    return 0;
}

/* Custom callback for put_frame */
static int put_frame_cb(void *priv, const vpx_image_t *img) {
    (void)priv;
    (void)img;
    return 0;
}

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    const char *err_str;
    vpx_fixed_buf_t cx_buf;
    uint8_t *cx_data = NULL;

    /* Initialize configuration with fuzzed values, clamped to reasonable limits */
    /* Ensure we have enough data before accessing indices */
    if (Size > 0) {
        cfg.threads = (Data[0] % 16) + 1;
    } else {
        cfg.threads = 1;
    }

    if (Size > 2) {
        cfg.w = (Data[1] * 256 + Data[2]);
    } else {
        cfg.w = 16;
    }

    if (Size > 4) {
        cfg.h = (Data[3] * 256 + Data[4]);
    } else {
        cfg.h = 16;
    }

    /* Ensure minimum dimensions to avoid immediate failures in some codecs */
    if (cfg.w < 16) cfg.w = 16;
    if (cfg.h < 16) cfg.h = 16;

    /* Initialize decoder context */
    res = vpx_codec_dec_init_ver(&ctx, vpx_codec_vp8_dx(), &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        /* Initialization failed, try to get error and return */
        err_str = vpx_codec_error(&ctx);
        (void)err_str;
        return 0;
    }

    /* Set frame buffer functions */
    res = vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb, release_frame_buffer_cb, NULL);
    if (res != VPX_CODEC_OK) {
        /* If external buffers are not supported, that's fine, continue with internal */
        err_str = vpx_codec_error(&ctx);
        (void)err_str;
    }

    /* Attempt to decode the input data */
    /* We pass the raw data as coded data. Libvpx will handle parsing. */
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    
    /* Check for decode errors */
    if (res != VPX_CODEC_OK && res != VPX_CODEC_ERROR) {
        err_str = vpx_codec_error(&ctx);
        (void)err_str;
    }

    /* Flush the decoder to get remaining frames */
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    if (res != VPX_CODEC_OK) {
        err_str = vpx_codec_error(&ctx);
        (void)err_str;
    }

    /* Iterate over decoded frames */
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Process frame if needed, here we just ensure it's valid */
        (void)img;
    }

    /* Test vpx_codec_set_cx_data_buf (Encoder function, but we can call on decoder context to test error handling) */
    cx_data = malloc(1024);
    if (cx_data) {
        cx_buf.buf = cx_data;
        cx_buf.sz = 1024;
        res = vpx_codec_set_cx_data_buf(&ctx, &cx_buf, 0, 0);
        if (res != VPX_CODEC_OK) {
            err_str = vpx_codec_error(&ctx);
            (void)err_str;
        }
        free(cx_data);
    }

    /* Cleanup: Free the decoder context */
    vpx_codec_destroy(&ctx);

    return 0;
}