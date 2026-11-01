// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/* Callback for put_frame 
 * Signature: void (*)(void *ctx, const vpx_image_t *img)
 */
static void put_frame_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
}

/* Callback for put_slice 
 * Signature: void (*)(void *ctx, const vpx_image_t *img, 
 *                     const struct vpx_image_rect *rect, const struct vpx_image_rect *rect)
 */
static void put_slice_cb(void *ctx, const vpx_image_t *img, 
                         const struct vpx_image_rect *rect, const struct vpx_image_rect *rect2) {
    (void)ctx;
    (void)img;
    (void)rect;
    (void)rect2;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    /* Initialize the decoder */
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        /* Try VP9 if VP8 is not available */
        iface = vpx_codec_vp9_dx();
        if (!iface) {
            return 0;
        }
    }

    vpx_codec_err_t init_res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_res != VPX_CODEC_OK) {
        return 0;
    }

    /* Register callbacks */
    vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    vpx_codec_register_put_slice_cb(&ctx, put_slice_cb, NULL);

    /* Decode the fuzzed data */
    vpx_codec_decode(&ctx, Data, Size, NULL, 0);

    /* Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

    /* Iterate over available frames to ensure they are consumed */
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Frame is valid here. We do not free it manually as libvpx manages it. */
        (void)img;
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}