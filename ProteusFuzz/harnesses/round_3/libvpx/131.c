// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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
#include <vpx/vp8dx.h>

// Correct signature for put_frame callback
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *frame) {
    (void)user_priv;
    (void)frame;
}

// Correct signature for put_slice callback using struct tag for vpx_image_rect
static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *frame, 
                               const struct vpx_image_rect *rect, const struct vpx_image_rect *sub_rect) {
    (void)user_priv;
    (void)frame;
    (void)rect;
    (void)sub_rect;
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_err_t res;

    // Initialize configuration with safe defaults
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    // Get the VP8 decoder interface
    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }

    // Initialize decoder
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Register callbacks with correct signatures
    res = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    res = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Decode the fuzzed data
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)res;

    // Flush the decoder
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}