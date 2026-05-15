// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

/* Declare the codec interface functions that are missing */
extern vpx_codec_iface_t *vpx_codec_vp8_cx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_cx(void);
extern vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_dx(void);

static void init_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) {
        memset(cfg, 0, sizeof(*cfg));
        cfg->g_w = 320;
        cfg->g_h = 240;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 1000;
        cfg->g_threads = 1;
        cfg->g_profile = 0;
        cfg->rc_min_quantizer = 0;
        cfg->rc_max_quantizer = 63;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->kf_mode = VPX_KF_AUTO;
        cfg->kf_min_dist = 0;
        cfg->kf_max_dist = 10;
        cfg->rc_end_usage = VPX_VBR;
        return;
    }
    
    memcpy(cfg, data, sizeof(*cfg));
    cfg->g_w = cfg->g_w % 2048 + 1;
    cfg->g_h = cfg->g_h % 2048 + 1;
    cfg->g_threads = cfg->g_threads % 8 + 1;
    cfg->rc_target_bitrate = cfg->rc_target_bitrate % 100000 + 100;
    cfg->rc_min_quantizer = cfg->rc_min_quantizer % 64;
    cfg->rc_max_quantizer = cfg->rc_max_quantizer % 64;
    if (cfg->rc_min_quantizer > cfg->rc_max_quantizer) {
        unsigned int tmp = cfg->rc_min_quantizer;
        cfg->rc_min_quantizer = cfg->rc_max_quantizer;
        cfg->rc_max_quantizer = tmp;
    }
    cfg->g_lag_in_frames = cfg->g_lag_in_frames % 10;
    cfg->kf_min_dist = cfg->kf_min_dist % 100;
    cfg->kf_max_dist = cfg->kf_max_dist % 100;
    if (cfg->kf_min_dist > cfg->kf_max_dist) {
        unsigned int tmp = cfg->kf_min_dist;
        cfg->kf_min_dist = cfg->kf_max_dist;
        cfg->kf_max_dist = tmp;
    }
}

static void init_decoder_config(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_dec_cfg_t)) {
        memset(cfg, 0, sizeof(*cfg));
        cfg->w = 320;
        cfg->h = 240;
        cfg->threads = 1;
        return;
    }
    
    memcpy(cfg, data, sizeof(*cfg));
    cfg->w = cfg->w % 2048 + 1;
    cfg->h = cfg->h % 2048 + 1;
    cfg->threads = cfg->threads % 8 + 1;
}

static vpx_codec_iface_t* get_codec_iface(int codec_type) {
    switch (codec_type) {
        case 0: return vpx_codec_vp8_cx();
        case 1: return vpx_codec_vp9_cx();
        case 2: return vpx_codec_vp8_dx();
        case 3: return vpx_codec_vp9_dx();
        default: return vpx_codec_vp8_cx();
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    int codec_type = data[0] % 4;
    data++;
    size--;
    
    vpx_codec_iface_t *iface = get_codec_iface(codec_type);
    
    if (codec_type == 0 || codec_type == 1) {
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t cfg;
        
        init_encoder_config(&cfg, data, size);
        data += sizeof(cfg);
        size -= size < sizeof(cfg) ? size : sizeof(cfg);
        
        vpx_codec_err_t err = vpx_codec_enc_init_ver(&encoder, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
            (void)preview;
            
            if (size > 0) {
                int ctrl_id = data[0];
                data++;
                size--;
                
                int ctrl_value = 0;
                if (size >= sizeof(int)) {
                    memcpy(&ctrl_value, data, sizeof(int));
                    data += sizeof(int);
                    size -= sizeof(int);
                }
                
                vpx_codec_control_(&encoder, ctrl_id, ctrl_value);
            }
            
            vpx_codec_destroy(&encoder);
        }
    } else {
        vpx_codec_ctx_t decoder;
        vpx_codec_dec_cfg_t cfg;
        
        init_decoder_config(&cfg, data, size);
        data += sizeof(cfg);
        size -= size < sizeof(cfg) ? size : sizeof(cfg);
        
        vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *frame = vpx_codec_get_frame(&decoder, &iter);
            (void)frame;
            
            vpx_codec_destroy(&decoder);
        }
    }
    
    return 0;
}