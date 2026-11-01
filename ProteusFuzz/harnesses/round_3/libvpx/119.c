// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_GET_LEVEL__value at vp8cx.h:1068:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9_INVERT_TILE_DECODE_ORDER__value at vp8dx.h:208:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vpx_codec.h>

static int handle_vp9_encoder_fuzz(const uint8_t *data, size_t size) {
    if (size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    int ret;

    iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    // Fuzz VP9E_SET_TUNE_CONTENT
    int tune_content = (data[0] % 3); 
    ret = vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, tune_content);
    (void)ret;

    // Fuzz VP9E_SET_DELTA_Q_UV
    int delta_q_uv = (int)(data[1] - 128);
    ret = vpx_codec_control(&ctx, VP9E_SET_DELTA_Q_UV, delta_q_uv);
    (void)ret;

    // Fuzz VP9E_SET_TARGET_LEVEL
    int target_level = (int)(data[2] % 10);
    ret = vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, target_level);
    (void)ret;

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    int quantizer = (int)(data[3] * 2);
    ret = vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, quantizer);
    (void)ret;

    // Attempt to get level (VP9E_GET_LEVEL)
    int current_level = 0;
    ret = vpx_codec_control(&ctx, VP9E_GET_LEVEL, &current_level);
    (void)ret;

    // Create a dummy frame to encode
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], 128, (cfg.g_w * cfg.g_h) / 4);

    vpx_codec_frame_flags_t flags = 0;
    ret = vpx_codec_encode(&ctx, &img, 0, 1, flags, VPX_DL_REALTIME);
    (void)ret;

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}

static int handle_vp9_decoder_fuzz(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    int ret;

    iface = vpx_codec_vp9_dx();
    if (!iface) return 0;

    ret = vpx_codec_dec_init(&ctx, iface, NULL, 0);
    if (ret != VPX_CODEC_OK) return 0;

    // Fuzz VP9_INVERT_TILE_DECODE_ORDER
    int invert_order = (int)(data[0] % 2);
    ret = vpx_codec_control(&ctx, VP9_INVERT_TILE_DECODE_ORDER, invert_order);
    (void)ret;

    // Create a dummy bitstream
    size_t stream_size = size > 100 ? 100 : size;
    uint8_t *stream = malloc(stream_size);
    if (!stream) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    memcpy(stream, data, stream_size);

    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *frame;
    ret = vpx_codec_decode(&ctx, stream, stream_size, NULL, 0);
    (void)ret;

    while ((frame = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)frame;
    }

    free(stream);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    if (Data[0] % 2 == 0) {
        handle_vp9_encoder_fuzz(Data, Size);
    } else {
        handle_vp9_decoder_fuzz(Data, Size);
    }

    return 0;
}