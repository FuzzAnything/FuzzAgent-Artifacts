// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    int ret;
    int cpuused = 0;
    int noise_sensitivity = 0;
    int static_threshold = 0;
    int gf_cbr_boost_pct = 0;
    // int postencode_drop = 0; // Skipped as VP9E_SET_POSTENCODE_DROP is not available in vp8cx.h

    if (size > 0) {
        cpuused = data[0] % 9; // 0-8
    }
    if (size > 1) {
        noise_sensitivity = data[1] % 2; // 0 or 1
    }
    if (size > 2) {
        static_threshold = data[2] % 100; // 0-99
    }
    if (size > 3) {
        gf_cbr_boost_pct = data[3] % 101; // 0-100
    }

    // Initialize encoder config
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    // Initialize codec
    if (vpx_codec_enc_init(&codec, vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Apply controls in specified order
    ret = vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpuused);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    ret = vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    ret = vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_threshold);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    ret = vpx_codec_control(&codec, VP8E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Note: VP9E_SET_POSTENCODE_DROP is skipped as it requires VP9 headers and context
    // which are not included per the requirement to use only vp8dx.h and vp8cx.h.

    // Encode a dummy frame to exercise the state
    vpx_image_t img;
    unsigned char *img_data = NULL;
    
    // Allocate buffer for a 32x32 I420 image
    size_t img_size = 32 * 32 * 3 / 2;
    if (size < img_size) {
        img_data = malloc(img_size);
        if (!img_data) {
            vpx_codec_destroy(&codec);
            return 0;
        }
        memset(img_data, 128, img_size); // Fill with gray
    } else {
        img_data = (unsigned char *)data;
    }

    if (vpx_img_wrap(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img_data) != 0) {
        if (img_data != data) free(img_data);
        vpx_codec_destroy(&codec);
        return 0;
    }

    ret = vpx_codec_encode(&codec, &img, 0, 1, 0, 0);
    // We don't strictly check ret here as encoding might fail for various reasons in fuzzing
    // but we proceed to cleanup.

    if (img_data != data) free(img_data);
    vpx_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_encoder(Data, Size);
    return 0;
}