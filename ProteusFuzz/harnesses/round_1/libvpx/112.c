// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
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

static void static_put_frame_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
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

    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }

    vpx_codec_err_t init_res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_res != VPX_CODEC_OK) {
        return 0;
    }

    vpx_codec_err_t cb_res = vpx_codec_register_put_frame_cb(&ctx, static_put_frame_cb, NULL);
    (void)cb_res;

    if (Size > 0) {
        vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    }

    vpx_codec_iter_t iter = NULL;
    while (1) {
        vpx_image_t *img = vpx_codec_get_frame(&ctx, &iter);
        if (img == NULL) {
            break;
        }
        (void)img;
    }

    vpx_codec_destroy(&ctx);

    return 0;
}