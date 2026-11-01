// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t res;
    const char *err_detail;
    const vpx_codec_iface_t *iface;

    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    // Get the VP8 decoder interface using the public API
    iface = vpx_codec_vp8_dx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize decoder context
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Initialization failed, try to get error detail
        err_detail = vpx_codec_error_detail(&ctx);
        (void)err_detail; // Suppress unused warning
        return 0;
    }

    // Try to get stream info before decoding (might fail, which is expected)
    si.sz = sizeof(si);
    res = vpx_codec_get_stream_info(&ctx, &si);
    (void)res;

    // Decode the fuzzed data
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)res;

    // Check for errors after decode
    if (res != VPX_CODEC_OK) {
        err_detail = vpx_codec_error_detail(&ctx);
        (void)err_detail;
    }

    // Iterate over decoded frames
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Frame is valid here. We don't do much with it to avoid complex cleanup.
        // Just ensure we don't dereference NULL.
        if (img != NULL) {
            // Access some fields to ensure they are reachable
            (void)img->w;
            (void)img->h;
            (void)img->fmt;
        }
    }

    // Flush the decoder
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    // Iterate again to ensure flush worked
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    // Try peeking stream info with the raw data (doesn't require context)
    si.sz = sizeof(si);
    res = vpx_codec_peek_stream_info(iface, Data, Size, &si);
    (void)res;

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}