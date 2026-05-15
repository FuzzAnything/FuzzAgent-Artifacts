// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) {
        memcpy(cfg, data, size);
    } else {
        memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));
    }
    
    cfg->g_w = cfg->g_w % 4096 + 1;
    cfg->g_h = cfg->g_h % 4096 + 1;
    cfg->g_threads = cfg->g_threads % 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = cfg->rc_target_bitrate % 10000 + 1;
    cfg->rc_min_quantizer = cfg->rc_min_quantizer % 64;
    cfg->rc_max_quantizer = cfg->rc_max_quantizer % 64;
    if (cfg->rc_min_quantizer > cfg->rc_max_quantizer) {
        unsigned int tmp = cfg->rc_min_quantizer;
        cfg->rc_min_quantizer = cfg->rc_max_quantizer;
        cfg->rc_max_quantizer = tmp;
    }
    cfg->g_error_resilient = cfg->g_error_resilient & 0x3;
    cfg->g_lag_in_frames = cfg->g_lag_in_frames % 16;
    cfg->kf_min_dist = cfg->kf_min_dist % 100;
    cfg->kf_max_dist = cfg->kf_max_dist % 100;
    if (cfg->kf_min_dist > cfg->kf_max_dist) {
        unsigned int tmp = cfg->kf_min_dist;
        cfg->kf_min_dist = cfg->kf_max_dist;
        cfg->kf_max_dist = tmp;
    }
    cfg->g_pass = (cfg->g_pass % 3);
}

static void fuzz_control(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    uint32_t ctrl_id = *(const uint32_t*)data;
    uint32_t value = *((const uint32_t*)(data + 4));
    
    switch (ctrl_id % 20) {
        case 0:
            vpx_codec_control_(ctx, VP8E_SET_CPUUSED, (int)value);
            break;
        case 1:
            vpx_codec_control_(ctx, VP8E_SET_ENABLEAUTOALTREF, (int)value);
            break;
        case 2:
            vpx_codec_control_(ctx, VP8E_SET_NOISE_SENSITIVITY, (int)value);
            break;
        case 3:
            vpx_codec_control_(ctx, VP8E_SET_SHARPNESS, (int)value);
            break;
        case 4:
            vpx_codec_control_(ctx, VP8E_SET_STATIC_THRESHOLD, (int)value);
            break;
        case 5:
            vpx_codec_control_(ctx, VP8E_SET_TOKEN_PARTITIONS, (int)value);
            break;
        case 6:
            vpx_codec_control_(ctx, VP8E_SET_ARNR_MAXFRAMES, (int)value);
            break;
        case 7:
            vpx_codec_control_(ctx, VP8E_SET_ARNR_STRENGTH, (int)value);
            break;
        case 8:
            vpx_codec_control_(ctx, VP8E_SET_ARNR_TYPE, (int)value);
            break;
        case 9:
            vpx_codec_control_(ctx, VP9E_SET_TILE_COLUMNS, (int)value);
            break;
        case 10:
            vpx_codec_control_(ctx, VP9E_SET_TILE_ROWS, (int)value);
            break;
        case 11:
            vpx_codec_control_(ctx, VP9E_SET_FRAME_PARALLEL_DECODING, (int)value);
            break;
        case 12:
            vpx_codec_control_(ctx, VP9E_SET_AQ_MODE, (int)value);
            break;
        case 13:
            vpx_codec_control_(ctx, VP9E_SET_FRAME_PERIODIC_BOOST, (int)value);
            break;
        case 14:
            vpx_codec_control_(ctx, VP9E_SET_NOISE_SENSITIVITY, (int)value);
            break;
        case 15:
            vpx_codec_control_(ctx, VP9E_SET_SVC, (int)value);
            break;
        case 16:
            vpx_codec_control_(ctx, VP9E_SET_SVC_PARAMETERS, (int)value);
            break;
        case 17:
            vpx_codec_control_(ctx, VP9E_SET_COLOR_SPACE, (int)value);
            break;
        case 18:
            vpx_codec_control_(ctx, VP9E_SET_TARGET_LEVEL, (int)value);
            break;
        case 19:
            vpx_codec_control_(ctx, VP9E_SET_ROW_MT, (int)value);
            break;
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size) {
    if (size < 100) return 0;
    
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));
    
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;
    
    fuzz_config(&cfg, data, size);
    size_t consumed = sizeof(vpx_codec_enc_cfg_t) < size ? sizeof(vpx_codec_enc_cfg_t) : size;
    
    res = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) return 0;
    
    size_t offset = consumed;
    while (offset + 8 <= size) {
        fuzz_control(&codec, data + offset, size - offset);
        offset += 8;
    }
    
    vpx_codec_enc_cfg_t new_cfg = cfg;
    if (size - offset >= sizeof(vpx_codec_enc_cfg_t)) {
        fuzz_config(&new_cfg, data + offset, size - offset);
        vpx_codec_enc_config_set(&codec, &new_cfg);
    }
    
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&codec);
    (void)headers;
    
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        (void)pkt;
    }
    
    vpx_codec_destroy(&codec);
    return 0;
}