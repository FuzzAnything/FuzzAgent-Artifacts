// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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

#define MAX_ENCODERS 4
#define MAX_WIDTH 4096
#define MAX_HEIGHT 4096

static void init_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(vpx_codec_enc_cfg_t) > size) {
        *offset = size;
        return;
    }
    
    memcpy(cfg, data + *offset, sizeof(vpx_codec_enc_cfg_t));
    *offset += sizeof(vpx_codec_enc_cfg_t);
    
    cfg->g_w = cfg->g_w % MAX_WIDTH + 1;
    cfg->g_h = cfg->g_h % MAX_HEIGHT + 1;
    cfg->g_threads = cfg->g_threads % 8 + 1;
    cfg->rc_target_bitrate = cfg->rc_target_bitrate % 10000 + 1;
    cfg->rc_min_quantizer = cfg->rc_min_quantizer % 64;
    cfg->rc_max_quantizer = cfg->rc_max_quantizer % 64;
    if (cfg->rc_min_quantizer > cfg->rc_max_quantizer) {
        unsigned int temp = cfg->rc_min_quantizer;
        cfg->rc_min_quantizer = cfg->rc_max_quantizer;
        cfg->rc_max_quantizer = temp;
    }
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 100;
    cfg->rc_end_usage = VPX_VBR;
}

static void init_rational(vpx_rational_t *rat, const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(vpx_rational_t) > size) {
        *offset = size;
        return;
    }
    
    memcpy(rat, data + *offset, sizeof(vpx_rational_t));
    *offset += sizeof(vpx_rational_t);
    
    if (rat->den == 0) rat->den = 1;
    if (rat->num < 0) rat->num = -rat->num;
    if (rat->den < 0) rat->den = -rat->den;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(vpx_codec_enc_cfg_t) * 2 + sizeof(vpx_rational_t) * 2) {
        return 0;
    }
    
    size_t offset = 0;
    
    vpx_codec_ctx_t ctx[MAX_ENCODERS];
    vpx_codec_enc_cfg_t cfg[MAX_ENCODERS];
    vpx_rational_t dsf[MAX_ENCODERS];
    
    int num_enc = (Data[offset++] % MAX_ENCODERS) + 1;
    vpx_codec_flags_t flags = Data[offset++] % 4;
    
    for (int i = 0; i < num_enc; i++) {
        init_encoder_config(&cfg[i], Data, Size, &offset);
        init_rational(&dsf[i], Data, Size, &offset);
    }
    
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    vpx_codec_err_t err = vpx_codec_enc_init_multi_ver(ctx, iface, cfg, num_enc, flags, dsf, VPX_ENCODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        for (int i = 0; i < num_enc; i++) {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&ctx[i], &iter)) != NULL) {
            }
            
            int ctrl_id = Data[offset % Size] % 256;
            offset++;
            vpx_codec_control_(&ctx[i], ctrl_id, Data[offset % Size]);
            offset++;
            
            vpx_codec_enc_config_set(&ctx[i], &cfg[(i + 1) % num_enc]);
        }
        
        for (int i = 0; i < num_enc; i++) {
            vpx_codec_destroy(&ctx[i]);
        }
    }
    
    vpx_codec_ctx_t single_ctx;
    vpx_codec_enc_cfg_t single_cfg;
    
    init_encoder_config(&single_cfg, Data, Size, &offset);
    
    err = vpx_codec_enc_init_ver(&single_ctx, iface, &single_cfg, flags, VPX_ENCODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&single_ctx, &iter)) != NULL) {
        }
        
        for (int i = 0; i < 5; i++) {
            int ctrl_id = Data[offset % Size] % 256;
            offset++;
            vpx_codec_control_(&single_ctx, ctrl_id, Data[offset % Size]);
            offset++;
        }
        
        vpx_codec_enc_config_set(&single_ctx, &single_cfg);
        
        vpx_codec_destroy(&single_ctx);
    }
    
    return 0;
}