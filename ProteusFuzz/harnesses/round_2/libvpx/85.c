// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_ROI_MAP__value at vp8cx.h:978:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static void setup_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg, vpx_codec_iface_t *iface, int width, int height) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->g_w = width > 0 ? width : 128;
    cfg->g_h = height > 0 ? height : 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 1;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->g_lag_in_frames = 1;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    
    vpx_codec_enc_config_default(iface, cfg, 0);
    vpx_codec_enc_init(ctx, iface, cfg, 0);
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    int width = Data[0];
    int height = Data[1];
    if (width == 0 || height == 0) {
        width = 128;
        height = 128;
    }
    if (width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    
    if (!codec_iface) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    setup_encoder(&ctx, &cfg, codec_iface, width, height);

    // Test VP8E_SET_ENABLEAUTOALTREF__value
    int auto_altref_val = (Size > 2) ? Data[2] % 2 : 0;
    vpx_codec_control(&ctx, VP8E_SET_ENABLEAUTOALTREF, auto_altref_val);

    // Test VP8E_SET_ROI_MAP__value
    if (Size > 4) {
        vpx_roi_map_t roi_map;
        memset(&roi_map, 0, sizeof(roi_map));
        roi_map.enabled = (Data[3] % 2);
        roi_map.rows = (Data[4] % 16) + 1;
        roi_map.cols = (Data[5 % Size] % 16) + 1;
        
        size_t roi_map_size = roi_map.rows * roi_map.cols;
        if (roi_map_size > 0 && roi_map_size < 1024) {
            roi_map.roi_map = (unsigned char *)malloc(roi_map_size);
            if (roi_map.roi_map) {
                for (size_t i = 0; i < roi_map_size; i++) {
                    roi_map.roi_map[i] = Data[(6 + i) % Size] % 8;
                }
                for (int i = 0; i < 8; i++) {
                    roi_map.delta_q[i] = ((int)Data[(6 + i) % Size] % 127) - 63;
                    roi_map.delta_lf[i] = ((int)Data[(6 + i + 8) % Size] % 127) - 63;
                    roi_map.skip[i] = (Data[(6 + i + 16) % Size] % 2);
                    roi_map.ref_frame[i] = (Data[(6 + i + 24) % Size] % 4);
                }
                // VP8 specific fields
                for (int i = 0; i < 4; i++) {
                    roi_map.static_threshold[i] = (Data[(6 + i + 32) % Size] % 256);
                }
                vpx_codec_control(&ctx, VP8E_SET_ROI_MAP, &roi_map);
                free(roi_map.roi_map);
            }
        }
    }

    // Test VP8E_SET_STATIC_THRESHOLD__value
    if (Size > 8) {
        unsigned int static_thresh = Data[8] % 256;
        vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, static_thresh);
    }

    // Test VP8E_SET_GF_CBR_BOOST_PCT__value
    if (Size > 9) {
        int gf_boost_pct_8 = Data[9] % 101;
        vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, gf_boost_pct_8);
    }

    // Test VP8E_SET_CPUUSED__value
    int cpu_used = (Size > 10) ? Data[10] % 9 : 0;
    vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);

    // Encode a dummy frame
    vpx_image_t frame;
    vpx_img_alloc(&frame, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    vpx_codec_encode(&ctx, &frame, 0, 1, 0, 0);

    vpx_img_free(&frame);
    cleanup_encoder(&ctx);

    return 0;
}