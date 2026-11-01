// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
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
#include <vpx/vp8.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>

/* Callback implementation for put_frame */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    /* Do nothing */
}

/* Callback implementation for put_slice */
static void put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const struct vpx_image_rect *rect, const struct vpx_image_rect *sub_rect) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)sub_rect;
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t err;

    /* Initialize configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    /* Initialize context structure */
    memset(&ctx, 0, sizeof(ctx));

    /* Try to get VP8 or VP9 decoder interface */
    iface = vpx_codec_vp8_dx();
    if (iface) {
        err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            /* Try VP9 if VP8 failed */
            iface = vpx_codec_vp9_dx();
            if (iface) {
                err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
            }
        }
    } else {
        /* VP8 not available, try VP9 directly */
        iface = vpx_codec_vp9_dx();
        if (iface) {
            err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        }
    }

    if (err != VPX_CODEC_OK) {
        /* Initialization failed, nothing to fuzz */
        return 0;
    }

    /* Register callbacks if supported */
    vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    vpx_codec_register_put_slice_cb(&ctx, put_slice_cb, NULL);

    /* Decode the input data */
    vpx_codec_decode(&ctx, Data, Size, NULL, 0);

    /* Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}