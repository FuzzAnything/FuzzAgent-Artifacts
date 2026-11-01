// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_ROI_MAP__value at vp8cx.h:978:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_roi_map_t roi_map;
    int ret;
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) {
        return 0;
    }

    // Initialize ROI map with safe defaults
    memset(&roi_map, 0, sizeof(roi_map));
    roi_map.enabled = 0;
    roi_map.rows = 0;
    roi_map.cols = 0;
    roi_map.roi_map = NULL;

    // Initialize encoder config
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;

    // Initialize with VP8 codec
    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize codec context
    ret = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz input for parameters
    uint8_t val1 = Data[0];
    uint8_t val2 = Data[1];
    uint8_t val3 = Size > 2 ? Data[2] : 0;
    uint8_t val4 = Size > 3 ? Data[3] : 0;
    uint8_t val5 = Size > 4 ? Data[4] : 0;
    uint8_t val6 = Size > 5 ? Data[5] : 0;

    // Test VP8E_SET_ROI_MAP__value
    roi_map.enabled = val1 & 0x01;
    if (Size > 10) {
        size_t map_size = (Size - 10) / 2;
        if (map_size > 0 && map_size < 10000) {
            roi_map.rows = 1;
            roi_map.cols = (unsigned int)map_size;
            roi_map.roi_map = (unsigned char *)malloc(map_size);
            if (roi_map.roi_map) {
                memcpy(roi_map.roi_map, Data + 10, map_size);
                ret = vpx_codec_control(&codec, VP8E_SET_ROI_MAP, &roi_map);
                (void)ret;
                free(roi_map.roi_map);
                roi_map.roi_map = NULL;
            }
        }
    }

    // Test VP8E_SET_STATIC_THRESHOLD__value
    // The API expects a single unsigned int, not an array.
    // We use the first value derived from input.
    unsigned int threshold_val = (unsigned int)(val2 % 1000);
    ret = vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, threshold_val);
    (void)ret;

    // Test VP8E_SET_ENABLEAUTOALTREF__value
    int enable_auto_altref = val4 % 2;
    ret = vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_altref);
    (void)ret;

    // Test VP8E_SET_GF_CBR_BOOST_PCT__value
    int gf_boost_pct_vp8 = val5 % 101;
    ret = vpx_codec_control(&codec, VP8E_SET_GF_CBR_BOOST_PCT, gf_boost_pct_vp8);
    (void)ret;

    // Test VP8E_SET_CPUUSED__value
    int cpu_used = val6 % 9;
    ret = vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    (void)ret;

    // Cleanup
    vpx_codec_destroy(&codec);

    return 0;
}