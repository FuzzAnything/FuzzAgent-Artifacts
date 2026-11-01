// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx decoder APIs.
 * Targets:
 * 1. vpx_codec_dec_init_ver
 * 2. vpx_codec_get_stream_info
 * 3. vpx_codec_decode
 * 4. vpx_codec_get_frame
 */

static vpx_codec_ctx_t *g_ctx = NULL;

static void cleanup_decoder(void) {
    if (g_ctx != NULL) {
        vpx_codec_destroy(&g_ctx);
        g_ctx = NULL;
    }
}

static int init_decoder(void) {
    if (g_ctx != NULL) {
        cleanup_decoder();
    }

    g_ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (g_ctx == NULL) {
        return -1;
    }
    memset(g_ctx, 0, sizeof(vpx_codec_ctx_t));

    /* Use the VP9 decoder interface directly from the header */
    vpx_codec_iface_t *iface = &vpx_codec_vp9_dx;
    
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    vpx_codec_err_t err = vpx_codec_dec_init_ver(
        g_ctx,
        iface,
        &cfg,
        0,
        VPX_DECODER_ABI_VERSION
    );

    if (err != VPX_CODEC_OK) {
        free(g_ctx);
        g_ctx = NULL;
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Attempt initialization if not already done */
    if (g_ctx == NULL) {
        if (init_decoder() != 0) {
            return 0;
        }
    }

    /* Test vpx_codec_get_stream_info */
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_err_t info_err = vpx_codec_get_stream_info(g_ctx, &si);
    (void)info_err;

    /* Test vpx_codec_decode with the input data */
    vpx_codec_err_t decode_err = vpx_codec_decode(
        g_ctx,
        Data,
        Size,
        NULL,
        0
    );
    (void)decode_err;

    /* Iterate over decoded frames */
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(g_ctx, &iter)) != NULL) {
        /* Frame retrieved, do nothing to avoid memory leaks or complex logic */
    }

    /* Flush the decoder by calling decode with NULL data */
    vpx_codec_decode(g_ctx, NULL, 0, NULL, 0);

    /* Iterate again to ensure all frames are consumed */
    iter = NULL;
    while ((img = vpx_codec_get_frame(g_ctx, &iter)) != NULL) {
        /* Consume frame */
    }

    /* Cleanup for this input to keep state clean for next input */
    cleanup_decoder();

    return 0;
}