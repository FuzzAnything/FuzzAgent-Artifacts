// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>

/* Callback for frame output. 
 * Signature must match vpx_codec_put_frame_cb_fn_t: void (*)(void *, const vpx_image_t *)
 * The first argument is user_priv, second is the image.
 */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Helper to initialize a decoder context */
static int init_decoder(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    const vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t err;

    /* Select interface based on available data or default to vp9 */
    iface = vpx_codec_vp9_dx();
    if (iface == NULL) {
        iface = vpx_codec_vp8_dx();
        if (iface == NULL) {
            return -1;
        }
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    err = vpx_codec_dec_init_ver(ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    /* Register callback if supported */
    vpx_codec_register_put_frame_cb(ctx, dummy_put_frame_cb, NULL);

    return 0;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    vpx_codec_err_t err;
    int initialized = 0;

    if (Size == 0) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    /* Step 1: Initialize decoder */
    if (init_decoder(&ctx, Data, Size) == 0) {
        initialized = 1;
    } else {
        /* If initialization fails, we can't proceed with decode/get_frame */
        return 0;
    }

    /* Step 2: Decode data */
    /* Pass the fuzz input as coded data */
    err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    
    /* Handle decode errors gracefully */
    if (err != VPX_CODEC_OK && err != VPX_CODEC_ERROR) {
        /* Some errors might be recoverable, but we continue to try get_frame */
    }

    /* Step 3: Retrieve decoded frames */
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Process the image if needed (e.g., check dimensions, planes) */
        (void)img;
    }

    /* Step 4: Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

    /* Step 5: Retrieve any remaining frames after flush */
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    /* Cleanup */
    if (initialized) {
        vpx_codec_destroy(&ctx);
    }

    return 0;
}