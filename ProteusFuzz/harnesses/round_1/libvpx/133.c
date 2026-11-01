// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
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

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const struct vpx_image_rect *slice_rect,
                               const struct vpx_image_rect *frame_rect) {
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)frame_rect;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 1920;
    cfg.h = 1080;

    vpx_codec_err_t init_res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_res != VPX_CODEC_OK) {
        return 0;
    }

    vpx_codec_err_t cb_res = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    (void)cb_res;

    cb_res = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    (void)cb_res;

    vpx_codec_iter_t iter = NULL;

    if (Size > 0) {
        vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    }

    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    vpx_codec_destroy(&ctx);

    return 0;
}