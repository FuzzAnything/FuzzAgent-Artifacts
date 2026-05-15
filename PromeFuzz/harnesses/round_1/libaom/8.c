// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"

static aom_codec_ctx_t *create_and_init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
    }
    if (size > 1) {
        cfg.g_h = (data[1] % 128) + 16;
    }
    
    res = aom_codec_enc_init_ver(ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    static int width = 64;
    static int height = 64;
    
    if (size > 2) {
        width = (data[2] % 128) + 16;
    }
    if (size > 3) {
        height = (data[3] % 128) + 16;
    }
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;
    
    if (size > 0) {
        size_t first_plane_size = img->w * img->h;
        size_t bytes_to_copy = size < first_plane_size ? size : first_plane_size;
        if (bytes_to_copy > 0) {
            memcpy(img->planes[0], data, bytes_to_copy);
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    aom_codec_ctx_t *ctx = create_and_init_encoder(data, size);
    if (!ctx) return 0;
    
    aom_image_t *img = create_test_image(data, size);
    
    size_t offset = 0;
    
    if (size > offset + 1) {
        int ctrl_id = data[offset] % 256;
        offset++;
        
        if (size > offset + 4) {
            int ctrl_value;
            memcpy(&ctrl_value, data + offset, 4);
            offset += 4;
            aom_codec_control(ctx, ctrl_id, ctrl_value);
        }
    }
    
    if (img) {
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        if (size > offset + 8) {
            memcpy(&pts, data + offset, 8);
            offset += 8;
        }
        
        if (size > offset + 4) {
            memcpy(&duration, data + offset, 4);
            offset += 4;
        }
        
        if (size > offset + 4) {
            memcpy(&flags, data + offset, 4);
            offset += 4;
        }
        
        aom_codec_encode(ctx, img, pts, duration, flags);
        
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
        }
        
        if (size > offset + 8) {
            static uint8_t buffer[4096];
            aom_fixed_buf_t buf;
            buf.buf = buffer;
            buf.sz = sizeof(buffer);
            
            unsigned int pad_before, pad_after;
            memcpy(&pad_before, data + offset, 4);
            offset += 4;
            memcpy(&pad_after, data + offset, 4);
            offset += 4;
            
            aom_codec_set_cx_data_buf(ctx, &buf, pad_before, pad_after);
        }
        
        aom_img_free(img);
    }
    
    aom_codec_encode(ctx, NULL, 0, 0, 0);
    
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
    }
    
    aom_codec_destroy(ctx);
    free(ctx);
    
    return 0;
}