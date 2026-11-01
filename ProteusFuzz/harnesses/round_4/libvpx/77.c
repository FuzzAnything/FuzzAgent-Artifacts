// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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

/* Callback function for put_frame notification */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_iter_t iter = NULL;
    vpx_codec_err_t res;

    /* Initialize configuration */
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    /* Initialize decoder context using VP9 decoder interface */
    res = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp9_dx, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    /* Register put_frame callback */
    res = vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    /* Decode data */
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    if (res != VPX_CODEC_OK && res != VPX_CODEC_ERROR) {
        /* Some errors are expected during fuzzing */
    }

    /* Flush the decoder */
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    if (res != VPX_CODEC_OK) {
        /* Some errors are expected during fuzzing */
    }

    /* Iterate over decoded frames */
    while (vpx_codec_get_frame(&ctx, &iter) != NULL) {
        /* Do nothing with the frame */
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}