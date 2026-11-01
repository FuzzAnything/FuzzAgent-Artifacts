// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *frame) {
    (void)user_priv;
    (void)frame;
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
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

    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) {
        return 0;
    }

    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    err = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)err;

    vpx_codec_iter_t iter = NULL;
    while (vpx_codec_get_frame(&ctx, &iter) != NULL) {
        // Frame processed
    }

    err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)err;

    const char *detail = vpx_codec_error_detail(&ctx);
    (void)detail;

    vpx_codec_destroy(&ctx);

    return 0;
}