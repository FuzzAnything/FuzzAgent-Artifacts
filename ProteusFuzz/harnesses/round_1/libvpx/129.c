// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
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
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void dummy_frame_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
}

static void dummy_slice_cb(void *ctx, const vpx_image_t *img,
                           const struct vpx_image_rect *rect, const struct vpx_image_rect *macroblock) {
    (void)ctx;
    (void)img;
    (void)rect;
    (void)macroblock;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    // Initialize the decoder
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) {
        // Fallback to VP8 if VP9 is not available
        iface = vpx_codec_vp8_dx();
        if (!iface) {
            return 0;
        }
    }

    vpx_codec_err_t res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Register callbacks
    vpx_codec_register_put_frame_cb(&ctx, dummy_frame_cb, NULL);
    vpx_codec_register_put_slice_cb(&ctx, dummy_slice_cb, NULL);

    // Decode data
    // We pass the fuzz input as the encoded data
    // Note: This data is likely not valid VP8/VP9 bitstream, but the decoder
    // should handle invalid input gracefully without crashing.
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)res;

    // Iterate over frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Do nothing with the frame, just ensure we can iterate
        // The image is valid until the next decode call or codec destroy
    }

    // Flush the decoder
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    // Iterate again after flush
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Do nothing
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}