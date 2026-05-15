// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

static void free_global_headers(aom_fixed_buf_t *buf) {
    if (buf) {
        free(buf->buf);
        free(buf);
    }
}

static aom_image_t *create_dummy_image(const uint8_t *data, size_t size) {
    static const unsigned int width = 16;
    static const unsigned int height = 16;
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
    
    size_t plane_size = width * height;
    size_t uv_plane_size = (width / 2) * (height / 2);
    
    if (size > 0) {
        size_t copy_size = size < plane_size ? size : plane_size;
        memcpy(img->planes[0], data, copy_size);
        
        if (size > plane_size) {
            copy_size = (size - plane_size) < uv_plane_size ? (size - plane_size) : uv_plane_size;
            memcpy(img->planes[1], data + plane_size, copy_size);
        }
        
        if (size > plane_size + uv_plane_size) {
            copy_size = (size - plane_size - uv_plane_size) < uv_plane_size ? 
                       (size - plane_size - uv_plane_size) : uv_plane_size;
            memcpy(img->planes[2], data + plane_size + uv_plane_size, copy_size);
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    unsigned int flags = 0;
    if (size % 2 == 0) {
        flags |= AOM_CODEC_USE_HIGHBITDEPTH;
    }
    
    aom_codec_err_t init_result = aom_codec_enc_init_ver(&ctx, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (init_result != AOM_CODEC_OK) {
        return 0;
    }
    
    aom_fixed_buf_t output_buf;
    unsigned char buffer[4096];
    output_buf.buf = buffer;
    output_buf.sz = sizeof(buffer);
    
    unsigned int pad_before = data[0] % 16;
    unsigned int pad_after = (data[0] >> 4) % 16;
    
    aom_codec_set_cx_data_buf(&ctx, &output_buf, pad_before, pad_after);
    
    aom_image_t *img = create_dummy_image(data, size);
    if (img) {
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t frame_flags = 0;
        
        if (size % 3 == 0) {
            frame_flags |= AOM_EFLAG_FORCE_KF;
        }
        
        aom_codec_encode(&ctx, img, pts, duration, frame_flags);
        
        pts += duration;
        if (size % 5 == 0) {
            aom_codec_encode(&ctx, NULL, pts, duration, 0);
        }
        
        aom_img_free(img);
    }
    
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        free_global_headers(global_headers);
    }
    
    if (size % 7 == 0) {
        cfg.rc_target_bitrate = 500 + (data[0] % 1500);
        aom_codec_enc_config_set(&ctx, &cfg);
    }
    
    if (size > 1) {
        int ctrl_id = AOME_SET_CPUUSED;
        int ctrl_value = 8;
        
        if (data[1] % 2 == 0) {
            ctrl_id = AOME_SET_ENABLEAUTOALTREF;
            ctrl_value = data[1] % 2;
        } else if (data[1] % 3 == 0) {
            ctrl_id = AOME_SET_CQ_LEVEL;
            ctrl_value = data[1] % 63;
        }
        
        aom_codec_control(&ctx, ctrl_id, ctrl_value);
    }
    
    aom_codec_destroy(&ctx);
    return 0;
}