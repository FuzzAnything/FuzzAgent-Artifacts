// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
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

static void my_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    // Callback body intentionally empty for fuzzing
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (iface == NULL) {
        return 0;
    }

    vpx_codec_err_t res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Register callback
    res = vpx_codec_register_put_frame_cb(&ctx, my_put_frame_cb, NULL);
    (void)res;

    // Decode data
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)res;

    // Flush decoder
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    // Iterate frames
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Frame processing
        (void)img;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}