// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

static void free_global_headers(aom_fixed_buf_t *buf) {
    if (buf) {
        free(buf->buf);
        free(buf);
    }
}

static aom_image_t *create_test_image(unsigned int width, unsigned int height, 
                                      aom_img_fmt_t fmt, unsigned int bit_depth) {
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    // Fill with some data
    for (int plane = 0; plane < 3; ++plane) {
        if (img->planes[plane]) {
            size_t plane_size = img->stride[plane] * (height >> (plane ? img->y_chroma_shift : 0));
            memset(img->planes[plane], plane % 256, plane_size);
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
    if (size < 16) return 0;
    
    // Initialize with default configuration
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use fuzz data to modify some configuration parameters
    cfg.g_w = 64 + (data[0] % 192);  // 64-256
    cfg.g_h = 64 + (data[1] % 192);  // 64-256
    cfg.g_bit_depth = (data[2] % 2) ? AOM_BITS_8 : AOM_BITS_10;
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100 + (data[3] % 9900);  // 100-10000 kbps
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = data[4] % 20;
    cfg.g_error_resilient = data[5] % 4;
    cfg.g_threads = 1 + (data[6] % 4);
    cfg.kf_mode = data[7] % 3;
    cfg.kf_min_dist = data[8] % 100;
    cfg.kf_max_dist = 100 + (data[9] % 900);
    cfg.rc_end_usage = data[10] % 4;
    cfg.rc_min_quantizer = data[11] % 64;
    cfg.rc_max_quantizer = 10 + (data[12] % 54);
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Try some codec controls with fuzz data
    int ctrl_id = AOME_SET_CPUUSED;
    int ctrl_value = 6 + (data[13] % 4);  // 6-9
    aom_codec_control(&ctx, ctrl_id, ctrl_value);
    
    ctrl_id = AOME_SET_ENABLEAUTOALTREF;
    ctrl_value = data[14] % 2;
    aom_codec_control(&ctx, ctrl_id, ctrl_value);
    
    ctrl_id = AOME_SET_CQ_LEVEL;
    ctrl_value = 10 + (data[15] % 50);  // 10-60
    aom_codec_control(&ctx, ctrl_id, ctrl_value);
    
    // Create test image
    aom_img_fmt_t fmt = cfg.g_bit_depth == AOM_BITS_8 ? AOM_IMG_FMT_I420 : AOM_IMG_FMT_I42016;
    aom_image_t *img = create_test_image(cfg.g_w, cfg.g_h, fmt, cfg.g_bit_depth);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Encode some frames
    for (int frame_idx = 0; frame_idx < 3; ++frame_idx) {
        aom_codec_pts_t pts = frame_idx * 1000;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        if (frame_idx == 2) {
            // Try reconfiguring mid-stream
            cfg.rc_target_bitrate = 500 + (data[frame_idx % 16] % 9500);
            aom_codec_enc_config_set(&ctx, &cfg);
        }
        
        aom_codec_encode(&ctx, img, pts, duration, flags);
        
        // Try getting global headers at different times
        if (frame_idx == 1) {
            aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
            free_global_headers(global_headers);
        }
    }
    
    // Flush encoder
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    
    // Get final global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    free_global_headers(global_headers);
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&ctx);
    
    return 0;
}