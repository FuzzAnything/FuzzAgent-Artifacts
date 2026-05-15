// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

static aom_image_t *create_dummy_image(const uint8_t *data, size_t size) {
    static const unsigned int width = 64;
    static const unsigned int height = 64;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->bit_depth = 8;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width / 2) * (height / 2);
    
    if (size > 0) {
        size_t copy_size = size;
        if (copy_size > y_plane_size) copy_size = y_plane_size;
        memcpy(img->planes[AOM_PLANE_Y], data, copy_size);
        
        if (size > y_plane_size) {
            copy_size = size - y_plane_size;
            if (copy_size > uv_plane_size) copy_size = uv_plane_size;
            memcpy(img->planes[AOM_PLANE_U], data + y_plane_size, copy_size);
        }
        
        if (size > y_plane_size + uv_plane_size) {
            copy_size = size - y_plane_size - uv_plane_size;
            if (copy_size > uv_plane_size) copy_size = uv_plane_size;
            memcpy(img->planes[AOM_PLANE_V], data + y_plane_size + uv_plane_size, copy_size);
        }
    }
    
    return img;
}

static void setup_encoder_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_config_default(iface, cfg, 0);
    
    if (size >= sizeof(unsigned int)) {
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->rc_target_bitrate = 256;
        cfg->g_threads = 1;
        cfg->g_pass = AOM_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->rc_end_usage = AOM_VBR;
        
        if (size >= 2 * sizeof(unsigned int)) {
            cfg->rc_min_quantizer = data[0] % 64;
            cfg->rc_max_quantizer = data[1] % 64;
        }
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    setup_encoder_config(&cfg, data, size);
    
    aom_codec_err_t res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    aom_image_t *img = create_dummy_image(data, size);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    size_t data_offset = 0;
    if (size > 100) data_offset = 100;
    
    unsigned int frame_count = 2;
    for (unsigned int i = 0; i < frame_count; i++) {
        aom_codec_pts_t pts = i * 1000;
        unsigned long duration = 1000;
        aom_enc_frame_flags_t flags = 0;
        
        if (i == frame_count - 1) {
            res = aom_codec_encode(&ctx, NULL, pts, duration, flags);
        } else {
            res = aom_codec_encode(&ctx, img, pts, duration, flags);
        }
        
        if (res != AOM_CODEC_OK) {
            break;
        }
        
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process frame packet
            } else if (pkt->kind == AOM_CODEC_STATS_PKT) {
                // Process stats packet
            }
        }
        
        if (i == 0 && size > data_offset + 8) {
            uint8_t buffer[1024];
            aom_fixed_buf_t buf;
            buf.buf = buffer;
            buf.sz = sizeof(buffer);
            
            unsigned int pad_before = data[data_offset] % 32;
            unsigned int pad_after = data[data_offset + 1] % 32;
            aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
        }
        
        if (i == 0 && size > data_offset + 16) {
            aom_codec_enc_cfg_t new_cfg = cfg;
            new_cfg.rc_target_bitrate = data[data_offset + 2] * 100 + 100;
            aom_codec_enc_config_set(&ctx, &new_cfg);
        }
        
        if (i == 0 && size > data_offset + 20) {
            int ctrl_id = AOME_SET_CPUUSED;
            int value = data[data_offset + 3] % 9;
            aom_codec_control(&ctx, ctrl_id, value);
            
            ctrl_id = AOME_SET_ENABLEAUTOALTREF;
            value = data[data_offset + 4] % 2;
            aom_codec_control(&ctx, ctrl_id, value);
        }
    }
    
    aom_img_free(img);
    aom_codec_destroy(&ctx);
    
    return 0;
}