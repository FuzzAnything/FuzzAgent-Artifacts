// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static int get_frame_buffer(void *cb_priv, unsigned long size,
                            struct vpx_codec_frame_buffer *buf) {
    (void)cb_priv;
    (void)size;
    (void)buf;
    return 0;
}

static int release_frame_buffer(void *cb_priv, struct vpx_codec_frame_buffer *buf) {
    (void)cb_priv;
    (void)buf;
    return 0;
}

static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Register frame callback
    vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, (void *)(uintptr_t)0x1234);

    // Set frame buffer functions
    err = vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer, release_frame_buffer, NULL);
    if (err != VPX_CODEC_OK) {
        // This is expected if the codec doesn't support external buffers, but we continue to test decode
    }

    // Decode data
    // Use the fuzzed data as input
    err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    
    // Check for errors and get details
    if (err != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail; // Use detail to avoid unused variable warning
    }

    // Iterate over decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame if needed
        (void)img;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}