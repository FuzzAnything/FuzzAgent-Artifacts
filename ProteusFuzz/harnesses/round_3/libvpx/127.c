// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
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
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8.h>
#include <vpx/vp8dx.h>

/* Callbacks for frame and slice notifications */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* 
 * The signature for vpx_codec_put_slice_cb_fn_t is:
 * void (*)(void *user_priv, const vpx_image_t *img,
 *          const struct vpx_image_rect *rect, const struct vpx_image_rect *ref_rect)
 */
static void put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const struct vpx_image_rect *rect, const struct vpx_image_rect *ref_rect) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)ref_rect;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface = NULL;

    /* Initialize configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 1920;
    cfg.h = 1080;

    /* Try to get the VP8 decoder interface */
    iface = vpx_codec_vp8_dx();
    if (iface == NULL) {
        /* Fallback to VP9 if VP8 is not available */
        iface = vpx_codec_vp9_dx();
        if (iface == NULL) {
            return 0;
        }
    }

    /* Initialize the decoder context */
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    /* Register callbacks */
    res = vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    (void)res; /* Ignore result for fuzzing exploration */

    res = vpx_codec_register_put_slice_cb(&ctx, put_slice_cb, NULL);
    (void)res; /* Ignore result for fuzzing exploration */

    /* Decode the input data */
    /* Pass data as is, even if it's not valid video data, to trigger error handling */
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)res;

    /* Flush the decoder */
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}