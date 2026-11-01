// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

/* Callback for frame buffer allocation */
static int get_frame_buffer_cb(void *cb_priv, unsigned long frame_size,
                               vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)frame_size;
    (void)fb;
    /* Fuzzing: occasionally return -1 to test error handling */
    if (cb_priv && *(uint8_t *)cb_priv == 0xFF) {
        return -1;
    }
    /* Return 0 to indicate success */
    return 0;
}

/* Callback for frame buffer release */
static int release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

/* Callback for put frame notification */
static void put_frame_cb(void *user_priv, const vpx_image_t *frame) {
    (void)user_priv;
    (void)frame;
    /* Do nothing, just consume the callback */
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t res;
    uint8_t *cb_priv_data = NULL;
    size_t cb_priv_size = 1;

    /* Initialize configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    /* Select interface based on input data to explore different code paths */
    if (Size > 0 && Data[0] < 2) {
        iface = vpx_codec_vp8_dx();
    } else {
        iface = vpx_codec_vp9_dx();
    }

    if (!iface) {
        return 0;
    }

    /* Initialize decoder */
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    /* Register put frame callback */
    res = vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    /* Ignore error as some codecs might not support it */

    /* Setup external frame buffer functions if data suggests it */
    if (Size > 1 && Data[1] & 0x01) {
        cb_priv_data = malloc(cb_priv_size);
        if (cb_priv_data) {
            cb_priv_data[0] = Data[1];
            res = vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb,
                                                       release_frame_buffer_cb,
                                                       cb_priv_data);
            /* Ignore error, might be INCAPABLE for some codecs */
        }
    }

    /* Decode loop */
    /* Use the input data as coded data */
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    
    /* Check for errors and retrieve details */
    if (res != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail; /* Use detail to avoid unused warning */
    }

    /* Iterate over decoded frames */
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Process frame (do nothing in fuzzing) */
        (void)img;
    }

    /* Flush the decoder */
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    if (res != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail;
    }

    /* Iterate again after flush */
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);
    free(cb_priv_data);

    return 0;
}