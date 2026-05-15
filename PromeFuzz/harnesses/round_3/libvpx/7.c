// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void init_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) {
        memset(cfg, 0, sizeof(*cfg));
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 256;
        cfg->g_error_resilient = 0;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->kf_mode = VPX_KF_AUTO;
        cfg->kf_min_dist = 0;
        cfg->kf_max_dist = 9999;
        cfg->rc_end_usage = VPX_VBR;
        cfg->rc_min_quantizer = 2;
        cfg->rc_max_quantizer = 56;
        cfg->rc_undershoot_pct = 100;
        cfg->rc_overshoot_pct = 15;
        cfg->rc_buf_sz = 1000;
        cfg->rc_buf_initial_sz = 500;
        cfg->rc_buf_optimal_sz = 600;
        cfg->rc_2pass_vbr_bias_pct = 50;
        cfg->rc_2pass_vbr_minsection_pct = 0;
        cfg->rc_2pass_vbr_maxsection_pct = 2000;
        cfg->ss_number_layers = 1;
        cfg->ts_number_layers = 1;
        cfg->ts_rate_decimator[0] = 1;
        cfg->ts_periodicity = 1;
        cfg->ts_layer_id[0] = 0;
        cfg->g_bit_depth = VPX_BITS_8;
        cfg->g_input_bit_depth = 8;
        cfg->g_profile = 0;
        cfg->g_threads = 1;
    } else {
        memcpy(cfg, data, sizeof(*cfg));
        cfg->g_w = cfg->g_w & 0xFFF;
        cfg->g_h = cfg->g_h & 0xFFF;
        if (cfg->g_w == 0) cfg->g_w = 64;
        if (cfg->g_h == 0) cfg->g_h = 64;
        cfg->g_lag_in_frames = cfg->g_lag_in_frames & 0xF;
        cfg->g_threads = cfg->g_threads & 0xF;
        cfg->rc_target_bitrate = cfg->rc_target_bitrate & 0xFFFF;
        cfg->rc_min_quantizer = cfg->rc_min_quantizer & 0x3F;
        cfg->rc_max_quantizer = cfg->rc_max_quantizer & 0x3F;
        if (cfg->rc_max_quantizer < cfg->rc_min_quantizer) {
            cfg->rc_max_quantizer = cfg->rc_min_quantizer;
        }
        cfg->ss_number_layers = cfg->ss_number_layers & 0x3;
        cfg->ts_number_layers = cfg->ts_number_layers & 0x3;
        cfg->ts_periodicity = cfg->ts_periodicity & 0xF;
    }
}

static vpx_image_t *create_test_image(const uint8_t *data, size_t size, unsigned int width, unsigned int height) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;
    
    unsigned int y_plane_size = width * height;
    unsigned int uv_plane_size = ((width + 1) / 2) * ((height + 1) / 2);
    
    if (size >= y_plane_size + 2 * uv_plane_size) {
        memcpy(img->planes[VPX_PLANE_Y], data, y_plane_size);
        memcpy(img->planes[VPX_PLANE_U], data + y_plane_size, uv_plane_size);
        memcpy(img->planes[VPX_PLANE_V], data + y_plane_size + uv_plane_size, uv_plane_size);
    } else {
        memset(img->planes[VPX_PLANE_Y], 128, y_plane_size);
        memset(img->planes[VPX_PLANE_U], 128, uv_plane_size);
        memset(img->planes[VPX_PLANE_V], 128, uv_plane_size);
    }
    
    img->stride[VPX_PLANE_Y] = width;
    img->stride[VPX_PLANE_U] = (width + 1) / 2;
    img->stride[VPX_PLANE_V] = (width + 1) / 2;
    
    return img;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    vpx_codec_ctx_t encoder;
    vpx_codec_ctx_t decoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    
    const vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    const vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    if (!encoder_iface || !decoder_iface) return 0;
    
    init_encoder_config(&cfg, data, size);
    data += sizeof(cfg) < size ? sizeof(cfg) : size;
    size -= sizeof(cfg) < size ? sizeof(cfg) : size;
    
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    res = vpx_codec_dec_init(&decoder, decoder_iface, NULL, 0);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    if (size > 0) {
        vpx_image_t *img = create_test_image(data, size, cfg.g_w, cfg.g_h);
        if (img) {
            vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
            vpx_img_free(img);
        }
    }
    
    vpx_codec_encode(&encoder, NULL, 0, 1, 0, VPX_DL_REALTIME);
    
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            vpx_codec_decode(&decoder, (const uint8_t*)pkt->data.frame.buf, 
                           pkt->data.frame.sz, NULL, 0);
        }
    }
    
    if (size > 0) {
        vpx_codec_decode(&decoder, data, size > 4096 ? 4096 : size, NULL, 0);
    }
    
    vpx_codec_decode(&decoder, NULL, 0, NULL, 0);
    
    uint8_t control_byte = size > 0 ? data[0] : 0;
    int ctrl_id = (control_byte % 20) + 1;
    int value = control_byte;
    
    vpx_codec_control_(&encoder, ctrl_id, value);
    
    int ref_updates = 0;
    vpx_codec_control_(&decoder, 1, &ref_updates);
    
    vpx_codec_enc_cfg_t new_cfg = cfg;
    new_cfg.rc_target_bitrate = (control_byte * 100) % 10000;
    vpx_codec_enc_config_set(&encoder, &new_cfg);
    
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}