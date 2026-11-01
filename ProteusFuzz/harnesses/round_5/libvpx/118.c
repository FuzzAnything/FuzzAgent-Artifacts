// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP8D_GET_FRAME_CORRUPTED__value at vp8dx.h:192:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 0;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 64, 64, 1);
    memset(img.planes[0], data[0] % 256, 64 * 64);
    memset(img.planes[1], data[0] % 256, 32 * 32);
    memset(img.planes[2], data[0] % 256, 32 * 32);

    vpx_codec_control(&ctx, VP9E_SET_DELTA_Q_UV, data[0]);
    vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, data[0] % 2);
    vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, data[0] % 100);
    vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, data[0] % 7);
    vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, data[0] % 2);

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}

static int fuzz_vp8_decoder(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec = vpx_codec_vp8_dx();
    if (!codec) return 0;

    if (vpx_codec_dec_init(&ctx, codec, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    int corrupted = 0;
    vpx_codec_control(&ctx, VP8D_GET_FRAME_CORRUPTED, &corrupted);

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    if (Data[0] % 2 == 0) {
        fuzz_vp9_encoder(Data, Size);
    } else {
        fuzz_vp8_decoder(Data, Size);
    }

    return 0;
}