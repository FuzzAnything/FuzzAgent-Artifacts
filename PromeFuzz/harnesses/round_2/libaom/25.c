// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    static const unsigned int width = 64;
    static const unsigned int height = 64;
    static const aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->fmt = fmt;
    img->w = width;
    img->h = height;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width >> 1) * (height >> 1);
    
    if (size < y_plane_size + 2 * uv_plane_size) {
        aom_img_free(img);
        return NULL;
    }
    
    memcpy(img->planes[AOM_PLANE_Y], data, y_plane_size);
    memcpy(img->planes[AOM_PLANE_U], data + y_plane_size, uv_plane_size);
    memcpy(img->planes[AOM_PLANE_V], data + y_plane_size + uv_plane_size, uv_plane_size);
    
    return img;
}

static void init_default_config(aom_codec_enc_cfg_t *cfg) {
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = AOM_VBR;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_error_resilient = 0;
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->kf_mode = AOM_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
    if (size < 100) return 0;
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    memset(&cfg, 0, sizeof(cfg));
    init_default_config(&cfg);
    
    aom_codec_err_t res = aom_codec_enc_init_ver(&codec, NULL, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) return 0;
    
    aom_image_t *img = create_test_image(data, size);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }
    
    size_t data_offset = 64 * 64 * 3 / 2;
    if (size > data_offset + 100) {
        uint8_t *cx_buf = malloc(4096);
        if (cx_buf) {
            aom_fixed_buf_t buf;
            buf.buf = cx_buf;
            buf.sz = 4096;
            
            unsigned int pad_before = data[data_offset] % 256;
            unsigned int pad_after = data[data_offset + 1] % 256;
            
            aom_codec_set_cx_data_buf(&codec, &buf, pad_before, pad_after);
            
            free(cx_buf);
        }
    }
    
    if (size > data_offset + 10) {
        int ctrl_id = (data[data_offset + 2] % 10) + 1;
        int ctrl_value = data[data_offset + 3];
        
        aom_codec_control(&codec, ctrl_id, ctrl_value);
    }
    
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    res = aom_codec_encode(&codec, img, pts, duration, flags);
    
    if (res == AOM_CODEC_OK) {
        aom_codec_encode(&codec, NULL, pts, duration, flags);
    }
    
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    if (size > data_offset + 50) {
        cfg.rc_target_bitrate = (data[data_offset + 4] % 1000) + 100;
        cfg.rc_min_quantizer = data[data_offset + 5] % 64;
        cfg.rc_max_quantizer = data[data_offset + 6] % 64;
        
        aom_codec_enc_config_set(&codec, &cfg);
    }
    
    aom_img_free(img);
    aom_codec_destroy(&codec);
    
    return 0;
}