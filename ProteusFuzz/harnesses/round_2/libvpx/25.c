// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_MIN_GF_INTERVAL__value at vp8cx.h:1052:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder = NULL;
    vpx_image_t *img = NULL;
    int ret;

    // Initialize encoder configuration
    encoder = vpx_codec_vp9_cx();
    if (!encoder) {
        return 0;
    }

    if (vpx_codec_enc_config_default(&cfg, encoder, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use input data to configure some parameters safely
    cfg.g_w = (Data[0] % 256) + 16; // Minimum width 16
    cfg.g_h = (Data[1] % 256) + 16; // Minimum height 16
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.rc_target_bitrate = (Data[5] % 1000) + 100;

    // Initialize the encoder
    ret = vpx_codec_enc_init(&codec, encoder, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Create a dummy image frame
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Fill image with fuzzed data
    if (Size > 0) {
        memset(img->planes[0], Data[0], img->d_w * img->d_h);
        memset(img->planes[1], Data[1], (img->d_w / 2) * (img->d_h / 2));
        memset(img->planes[2], Data[2], (img->d_w / 2) * (img->d_h / 2));
    }

    // Test VP9E_SET_LOSSLESS
    if (Size > 10) {
        int lossless = (Data[10] % 2);
        vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless);
    }

    // Test VP9E_SET_MAX_GF_INTERVAL
    if (Size > 11) {
        int max_gf_interval = (Data[11] % 32) + 1;
        vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
    }

    // Test VP9E_SET_POSTENCODE_DROP
    if (Size > 12) {
        int postencode_drop = (Data[12] % 2);
        vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, postencode_drop);
    }

    // Test VP9E_SET_FRAME_PERIODIC_BOOST
    if (Size > 13) {
        int periodic_boost = (Data[13] % 2);
        vpx_codec_control(&codec, VP9E_SET_FRAME_PERIODIC_BOOST, periodic_boost);
    }

    // Test VP9E_SET_MIN_GF_INTERVAL
    if (Size > 14) {
        int min_gf_interval = (Data[14] % 32) + 1;
        vpx_codec_control(&codec, VP9E_SET_MIN_GF_INTERVAL, min_gf_interval);
    }

    // Test VP9E_SET_TARGET_LEVEL
    if (Size > 15) {
        int target_level = (Data[15] % 4) + 1; // Valid levels 1-4
        vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, target_level);
    }

    // Encode a frame to exercise the encoder with the set options
    vpx_codec_encode(&codec, img, 0, 1, 0, 0);

    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);

    return 0;
}