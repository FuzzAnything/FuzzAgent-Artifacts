// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>

/* Callback function for decoded frames */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Callback function for decoded slices */
static void put_slice_cb(void *user_priv, const vpx_image_t *frame,
                         const vpx_image_rect_t *rect,
                         const vpx_image_rect_t *dst_rect) {
    (void)user_priv;
    (void)frame;
    (void)rect;
    (void)dst_rect;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Initialize decoder configuration */
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    /* Get VP9 decoder interface */
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) {
        return 0;
    }

    /* Initialize the decoder */
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    /* Register put frame callback */
    err = vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    /* Register put slice callback if supported */
    err = vpx_codec_register_put_slice_cb(&ctx, put_slice_cb, NULL);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    /* Decode the input data */
    err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    if (err != VPX_CODEC_OK && err != VPX_CODEC_ERROR && err != VPX_CODEC_CORRUPT_FRAME) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    /* Iterate over decoded frames */
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Do nothing with the frame, just iterate */
        (void)img;
    }

    /* Flush the decoder */
    err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    if (err != VPX_CODEC_OK && err != VPX_CODEC_ERROR && err != VPX_CODEC_CORRUPT_FRAME) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    /* Iterate over flushed frames */
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    /* Check for detailed error information */
    const char *detail = vpx_codec_error_detail(&ctx);
    (void)detail;

    /* Destroy the decoder context */
    vpx_codec_destroy(&ctx);

    return 0;
}