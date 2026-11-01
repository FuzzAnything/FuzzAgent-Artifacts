// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int run_encoder_fuzz(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = NULL;
    vpx_codec_err_t err;

    err = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    err = vpx_codec_enc_init(&codec, codec_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    if (size > 0) {
        size_t copy_size = size < (size_t)(img->d_w * img->d_h) ? size : (size_t)(img->d_w * img->d_h);
        memcpy(img->planes[0], data, copy_size);
    }

    vpx_codec_encode(&codec, img, 0, 1, 0, 0);

    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    return 1;
}

static int run_decoder_fuzz(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_dx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_err_t err;

    err = vpx_codec_dec_init(&codec, codec_iface, NULL, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Fixed: Added the missing 'deadline' argument (5th argument)
    vpx_codec_decode(&codec, data, size, NULL, 0);
    
    vpx_codec_destroy(&codec);
    return 1;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    run_encoder_fuzz(Data, Size);
    run_decoder_fuzz(Data, Size);

    return 0;
}