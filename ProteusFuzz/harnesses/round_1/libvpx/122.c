// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_ROI_MAP__value at vp8cx.h:978:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_roi_map_t roi_map;
    const vpx_codec_iface_t *iface;
    int ret;

    // Get the VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != 0) {
        return 0;
    }

    // Initialize ROI map with safe defaults
    memset(&roi_map, 0, sizeof(roi_map));
    roi_map.enabled = 1;
    roi_map.rows = 2;
    roi_map.cols = 2;
    roi_map.roi_map = (unsigned char *)calloc(roi_map.rows * roi_map.cols, sizeof(unsigned char));
    if (!roi_map.roi_map) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Parse input data to set ROI map values
    size_t idx = 0;
    if (idx < size) roi_map.enabled = data[idx++];
    if (idx < size) roi_map.rows = data[idx++];
    if (idx < size) roi_map.cols = data[idx++];
    if (idx < size) roi_map.static_threshold[0] = data[idx++];
    if (idx < size) roi_map.static_threshold[1] = data[idx++];
    if (idx < size) roi_map.static_threshold[2] = data[idx++];
    if (idx < size) roi_map.static_threshold[3] = data[idx++];

    // Set CPU usage (0-8)
    int cpu_used = 0;
    if (idx < size) {
        cpu_used = data[idx++] & 0x07; // Ensure 0-7 range
    }

    // Set Auto Alt Ref
    int enable_auto_alt_ref = 0;
    if (idx < size) {
        enable_auto_alt_ref = data[idx++] & 0x01;
    }

    // Set GF CBR Boost Pct
    int gf_cbr_boost_pct = 0;
    if (idx < size) {
        gf_cbr_boost_pct = data[idx++] & 0x3F; // 0-63
    }

    // Apply controls
    vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
    vpx_codec_control(&ctx, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);

    // Apply ROI map if enabled
    if (roi_map.enabled) {
        vpx_codec_control(&ctx, VP8E_SET_ROI_MAP, &roi_map);
    }

    // Apply static threshold if available
    if (idx < size) {
        unsigned int static_thresh = data[idx++];
        vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, static_thresh);
    }

    // Encode a dummy frame
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, 0);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
    memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    // Cleanup
    free(roi_map.roi_map);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_vp8_encoder(Data, Size);
}