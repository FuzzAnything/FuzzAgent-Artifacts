// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *codec = vpx_codec_vp8_cx();
    
    if (!codec) return 0;

    if (vpx_codec_enc_config_default(codec, &cfg, 0) < 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) < 0) {
        return 0;
    }

    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    if (Size >= cfg.g_w * cfg.g_h) {
        memcpy(img.planes[0], Data, cfg.g_w * cfg.g_h);
        memset(img.planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
        memset(img.planes[2], 128, (cfg.g_w * cfg.g_h) / 4);
    }

    int ctrl_id = Data[0] % 6;
    int value = Data[1];
    
    int ret = 0;
    
    switch (ctrl_id) {
        case 0:
            ret = vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, value);
            break;
        case 1:
            ret = vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, value);
            break;
        case 2:
            ret = vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, value);
            break;
        case 3:
            ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, value % 9);
            break;
        case 4:
            ret = vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, value);
            break;
        case 5:
            ret = vpx_codec_control(&ctx, VP8E_SET_ENABLEAUTOALTREF, value % 2);
            break;
    }

    vpx_codec_frame_flags_t flags = 0;
    vpx_codec_err_t err = vpx_codec_encode(&ctx, &img, 0, 1, flags, VPX_DL_REALTIME);
    (void)err;

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}