// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

/* Callback function for frame availability */
/* The signature must match vpx_codec_put_frame_cb_fn_t: void (*)(void *, const vpx_image_t *) */
static void put_frame_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
}

/* Helper to safely free the context */
static void cleanup_ctx(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_err_t err;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_iface_t *iface;

    /* Initialize configuration with some default values */
    cfg.threads = 1;
    cfg.w = 1920;
    cfg.h = 1080;

    /* Try to get the VP9 decoder interface */
    iface = vpx_codec_vp9_dx();
    if (!iface) {
        /* Fallback to VP8 if VP9 is not available */
        iface = vpx_codec_vp8_dx();
        if (!iface) {
            return 0;
        }
    }

    /* Initialize the decoder context */
    err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    /* Register the put_frame callback */
    err = vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    if (err != VPX_CODEC_OK) {
        cleanup_ctx(&ctx);
        return 0;
    }

    /* Attempt to decode the fuzzed data */
    /* We pass the fuzzed data as the coded data buffer */
    err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);

    /* Check for errors and retrieve details if necessary */
    if (err != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail; /* Suppress unused variable warning */
        /* Continue to flush to ensure proper cleanup */
    }

    /* Flush the decoder by passing NULL data */
    err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)err;

    /* Iterate over any available frames */
    while ((vpx_image_t *)vpx_codec_get_frame(&ctx, &iter) != NULL) {
        /* Frame processing would happen here */
    }

    /* Cleanup */
    cleanup_ctx(&ctx);

    return 0;
}