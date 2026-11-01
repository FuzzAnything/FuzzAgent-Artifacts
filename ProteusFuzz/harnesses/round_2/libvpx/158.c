// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required headers for libvpx */
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8.h>

/* Static helper to get the decoder interface */
static const vpx_codec_iface_t *get_dec_iface(void) {
    return vpx_codec_vp9_dx();
}

/* Dummy callback for put_frame matching the library signature:
 * void (*vpx_codec_put_frame_cb_fn_t)(void *user_priv, const vpx_image_t *frame)
 */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *frame) {
    (void)user_priv;
    (void)frame;
}

/* Dummy callback for put_slice matching the library signature:
 * void (*vpx_codec_put_slice_cb_fn_t)(void *user_priv, const vpx_image_t *frame,
 *                                     const vpx_image_rect_t *rect, const vpx_image_rect_t *rect)
 * Note: The exact signature may vary slightly by version, but typically involves user_priv and frame.
 * Based on the error log, the signature is:
 * void (*)(void *, const struct vpx_image *, const struct vpx_image_rect *, const struct vpx_image_rect *)
 */
static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *frame,
                               const vpx_image_rect_t *rect, const vpx_image_rect_t *rect2) {
    (void)user_priv;
    (void)frame;
    (void)rect;
    (void)rect2;
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;

    /* Guard against empty inputs */
    if (Size == 0) {
        return 0;
    }

    /* Initialize configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    /* Get decoder interface */
    iface = get_dec_iface();
    if (iface == NULL) {
        return 0;
    }

    /* Initialize decoder context */
    memset(&ctx, 0, sizeof(ctx));
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        /* Initialization failed, try to explore error paths with invalid context */
        vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void *)Data);
        vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void *)Data);
        vpx_codec_decode(&ctx, Data, Size, (void *)Data, 0);
        return 0;
    }

    /* Register frame callback */
    res = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void *)Data);
    (void)res;

    /* Register slice callback */
    res = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void *)Data);
    (void)res;

    /* Decode data */
    res = vpx_codec_decode(&ctx, Data, Size, (void *)Data, 0);
    (void)res;

    /* Flush decoder */
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}