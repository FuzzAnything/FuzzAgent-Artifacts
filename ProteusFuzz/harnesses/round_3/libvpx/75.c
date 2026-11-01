// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_ROI_MAP__value at vp8cx.h:978:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

// Forward declaration for VP9 control constants if not available in included headers
// These are standard VP9 control IDs, typically defined in vp9cx.h or vpx_encoder.h
#ifndef VP9E_SET_NOISE_SENSITIVITY
#define VP9E_SET_NOISE_SENSITIVITY 2001
#endif

static int test_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_roi_map_t roi_map;
    int ret;
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 63;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != 0) {
        return 0;
    }

    // Fuzz VP8E_SET_ARNR_STRENGTH
    if (size >= 1) {
        int arnr_strength = data[0] % 4;
        ret = vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
        (void)ret;
    }

    // Fuzz VP8E_SET_NOISE_SENSITIVITY
    if (size >= 2) {
        int noise_sens = (int)(data[1] & 0x3F); // 0-63
        ret = vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, noise_sens);
        (void)ret;
    }

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (size >= 3) {
        int max_intra_pct = (int)(data[2] & 0xFF); // 0-255
        ret = vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);
        (void)ret;
    }

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    if (size >= 4) {
        int screen_mode = (data[3] & 0x01); // 0 or 1
        ret = vpx_codec_control(&codec, VP8E_SET_SCREEN_CONTENT_MODE, screen_mode);
        (void)ret;
    }

    // Fuzz VP8E_SET_ROI_MAP
    if (size >= 5) {
        memset(&roi_map, 0, sizeof(roi_map));
        roi_map.enabled = (data[4] & 0x01);
        roi_map.rows = (data[4] >> 1) & 0x0F; // 0-15
        roi_map.cols = (data[4] >> 5) & 0x03; // 0-3
        if (roi_map.rows > 0 && roi_map.cols > 0 && size >= 6) {
            size_t map_size = roi_map.rows * roi_map.cols;
            if (size - 5 >= map_size) {
                roi_map.roi_map = (unsigned char *)malloc(map_size);
                if (roi_map.roi_map) {
                    memcpy(roi_map.roi_map, data + 5, map_size);
                    // Ensure values are within 0-3 for VP8
                    for (size_t i = 0; i < map_size; i++) {
                        roi_map.roi_map[i] &= 0x03;
                    }
                    ret = vpx_codec_control(&codec, VP8E_SET_ROI_MAP, &roi_map);
                    (void)ret;
                    free(roi_map.roi_map);
                }
            }
        }
    }

    vpx_codec_destroy(&codec);
    return 0;
}

static int test_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    int ret;
    const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 63;

    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != 0) {
        return 0;
    }

    // Fuzz VP9E_SET_NOISE_SENSITIVITY
    if (size >= 1) {
        int noise_sens = (int)(data[0] & 0x3F); // 0-63
        ret = vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sens);
        (void)ret;
    }

    vpx_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run both VP8 and VP9 tests to maximize coverage
    test_vp8_encoder(Data, Size);
    test_vp9_encoder(Data, Size);

    return 0;
}