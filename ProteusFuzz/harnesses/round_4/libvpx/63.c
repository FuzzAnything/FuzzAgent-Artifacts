// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Dummy callback for put_frame to satisfy registration requirements
// Signature must match vpx_codec_put_frame_cb_fn_t: void (*)(void *, const vpx_image_t *)
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

// Helper to safely destroy context if needed
static void cleanup_ctx(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;

    // Initialize configuration
    cfg.threads = 1;
    cfg.w = 16;
    cfg.h = 16;

    // Get VP8 or VP9 decoder interface
    iface = vpx_codec_vp8_dx();
    if (!iface) {
        iface = vpx_codec_vp9_dx();
    }
    if (!iface) {
        return 0; // No decoder available
    }

    // Initialize decoder context
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Register put frame callback
    res = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    // Ignore error as some codecs might not support it (VPX_CODEC_INCAPABLE)

    // Attempt to decode the fuzzed data
    // We pass the data directly. The decoder will handle invalid data gracefully.
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);

    // If decoding succeeded or partially succeeded, try to retrieve frames
    if (res == VPX_CODEC_OK || res == VPX_CODEC_ERROR) {
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            // Frame retrieved successfully, do nothing or minimal processing
            (void)img;
        }
    }

    // Flush the decoder to ensure all frames are processed
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

    // Retrieve any remaining frames
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    // Cleanup
    cleanup_ctx(&ctx);

    return 0;
}