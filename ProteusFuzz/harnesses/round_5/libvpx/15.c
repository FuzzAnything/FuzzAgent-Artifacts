// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_ROI_MAP__value at vp8cx.h:978:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"

static int fuzz_vp8e_set_roi_map(vpx_codec_ctx_t *enc, const uint8_t *data, size_t size) {
    if (!enc || !data || size < sizeof(vpx_roi_map_t)) {
        return 0;
    }

    vpx_roi_map_t roi_map;
    memset(&roi_map, 0, sizeof(roi_map));
    memcpy(&roi_map, data, sizeof(roi_map));

    // Validate and sanitize roi_map fields
    roi_map.enabled = (roi_map.enabled > 1) ? 1 : roi_map.enabled;
    
    if (roi_map.rows == 0 || roi_map.cols == 0) {
        roi_map.rows = 1;
        roi_map.cols = 1;
    }
    
    // Ensure roi_map is not NULL and has valid size
    if (!roi_map.roi_map) {
        size_t map_size = roi_map.rows * roi_map.cols;
        if (map_size > 1000000) return 0; // Prevent huge allocations
        roi_map.roi_map = (unsigned char *)malloc(map_size);
        if (!roi_map.roi_map) {
            return 0;
        }
        memset(roi_map.roi_map, 0, map_size);
    }

    // Clamp delta_q and delta_lf to valid range [-63, 63]
    for (int i = 0; i < 8; i++) {
        if (roi_map.delta_q[i] < -63) roi_map.delta_q[i] = -63;
        if (roi_map.delta_q[i] > 63) roi_map.delta_q[i] = 63;
        if (roi_map.delta_lf[i] < -63) roi_map.delta_lf[i] = -63;
        if (roi_map.delta_lf[i] > 63) roi_map.delta_lf[i] = 63;
    }

    int ret = vpx_codec_control(enc, VP8E_SET_ROI_MAP, &roi_map);
    
    if (roi_map.roi_map) {
        free(roi_map.roi_map);
    }
    
    return ret;
}

static int fuzz_vp8e_set_static_threshold(vpx_codec_ctx_t *enc, const uint8_t *data, size_t size) {
    if (!enc || !data || size < sizeof(unsigned int)) {
        return 0;
    }
    
    unsigned int threshold;
    memcpy(&threshold, data, sizeof(unsigned int));
    
    // Ensure threshold is within reasonable bounds
    if (threshold > 0xFFFF) {
        threshold = 0xFFFF;
    }
    
    return vpx_codec_control(enc, VP8E_SET_STATIC_THRESHOLD, threshold);
}

static int fuzz_vp8e_set_max_intra_bitrate_pct(vpx_codec_ctx_t *enc, const uint8_t *data, size_t size) {
    if (!enc || !data || size < sizeof(int)) {
        return 0;
    }
    
    int pct;
    memcpy(&pct, data, sizeof(int));
    
    // Clamp percentage to [0, 100]
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    
    return vpx_codec_control(enc, VP8E_SET_MAX_INTRA_BITRATE_PCT, pct);
}

static int fuzz_vp8e_set_frame_flags(vpx_codec_ctx_t *enc, const uint8_t *data, size_t size) {
    if (!enc || !data || size < sizeof(int)) {
        return 0;
    }
    
    int flags;
    memcpy(&flags, data, sizeof(int));
    
    return vpx_codec_control(enc, VP8E_SET_FRAME_FLAGS, flags);
}

static int fuzz_vp8e_set_enableautoaltref(vpx_codec_ctx_t *enc, const uint8_t *data, size_t size) {
    if (!enc || !data || size < sizeof(int)) {
        return 0;
    }
    
    int enable;
    memcpy(&enable, data, sizeof(int));
    
    // Clamp to 0 or 1
    enable = (enable != 0) ? 1 : 0;
    
    return vpx_codec_control(enc, VP8E_SET_ENABLEAUTOALTREF, enable);
}

static int fuzz_vp8e_set_cpuused(vpx_codec_ctx_t *enc, const uint8_t *data, size_t size) {
    if (!enc || !data || size < sizeof(int)) {
        return 0;
    }
    
    int cpu_used;
    memcpy(&cpu_used, data, sizeof(int));
    
    // Clamp to valid range [0, 8]
    if (cpu_used < 0) cpu_used = 0;
    if (cpu_used > 8) cpu_used = 8;
    
    return vpx_codec_control(enc, VP8E_SET_CPUUSED, cpu_used);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy encoder instance
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize encoder
    if (vpx_codec_enc_init(&enc, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use the first byte to decide which function to fuzz
    int choice = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    int ret = 0;
    switch (choice) {
        case 0:
            ret = fuzz_vp8e_set_roi_map(&enc, payload, payload_size);
            break;
        case 1:
            ret = fuzz_vp8e_set_static_threshold(&enc, payload, payload_size);
            break;
        case 2:
            ret = fuzz_vp8e_set_max_intra_bitrate_pct(&enc, payload, payload_size);
            break;
        case 3:
            ret = fuzz_vp8e_set_frame_flags(&enc, payload, payload_size);
            break;
        case 4:
            ret = fuzz_vp8e_set_enableautoaltref(&enc, payload, payload_size);
            break;
        case 5:
            ret = fuzz_vp8e_set_cpuused(&enc, payload, payload_size);
            break;
    }

    // Cleanup
    vpx_codec_destroy(&enc);
    
    return 0;
}