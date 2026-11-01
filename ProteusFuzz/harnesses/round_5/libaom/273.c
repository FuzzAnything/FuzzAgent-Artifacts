// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void free_image(aom_image_t *img) {
    if (img == NULL) return;
    if (img->planes[0] != NULL) {
        aom_img_free(img);
    }
}

static int create_test_image(aom_image_t *img, unsigned int w, unsigned int h, unsigned int bit_depth, const uint8_t *data, size_t size) {
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
        return -1;
    }
    img->bit_depth = bit_depth;
    
    if (data != NULL && size > 0) {
        size_t plane_size = w * h;
        size_t uv_size = (w / 2) * (h / 2);
        
        size_t copy_size = (size < plane_size) ? size : plane_size;
        if (img->planes[0] != NULL) {
            memcpy(img->planes[0], data, copy_size);
        }
        
        if (size > plane_size) {
            size_t remaining = size - plane_size;
            size_t uv_copy = (remaining < uv_size) ? remaining : uv_size;
            if (img->planes[1] != NULL) {
                memcpy(img->planes[1], data + plane_size, uv_copy);
            }
            if (size > plane_size + uv_size) {
                remaining = size - plane_size - uv_size;
                uv_copy = (remaining < uv_size) ? remaining : uv_size;
                if (img->planes[2] != NULL) {
                    memcpy(img->planes[2], data + plane_size + uv_size, uv_copy);
                }
            }
        }
    }
    return 0;
}

int LLVMFuzzerTestOneInput_273(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t img;
    const aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t err;
    int frame_count = 0;
    int flush_count = 0;
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    aom_codec_iter_t iter;
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));
    
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }
    
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    if (Size > 10) {
        unsigned int w = (Data[0] % 128) + 16;
        unsigned int h = (Data[1] % 128) + 16;
        cfg.g_w = w;
        cfg.g_h = h;
        
        unsigned int bit_depth = 8;
        if (Data[2] % 3 == 1) bit_depth = 10;
        else if (Data[2] % 3 == 2) bit_depth = 12;
        cfg.g_bit_depth = (aom_bit_depth_t)bit_depth;
        cfg.g_input_bit_depth = bit_depth;
        
        cfg.g_threads = (Data[3] % 4) + 1;
        cfg.g_profile = (Data[4] % 4);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Data[5] % 100) + 30;
        
        cfg.rc_end_usage = (enum aom_rc_mode)(Data[6] % 4);
        if (cfg.rc_end_usage == AOM_VBR || cfg.rc_end_usage == AOM_CBR) {
            cfg.rc_target_bitrate = (Data[7] % 100) + 100;
        }
        
        cfg.rc_min_quantizer = (Data[8] % 63);
        cfg.rc_max_quantizer = (Data[9] % 63) + cfg.rc_min_quantizer;
        
        cfg.kf_mode = AOM_KF_DISABLED;
        cfg.kf_min_dist = 0;
        cfg.kf_max_dist = 0;
        
        cfg.g_limit = 0;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_bit_depth = AOM_BITS_8;
        cfg.g_input_bit_depth = 8;
        cfg.g_threads = 1;
        cfg.g_profile = 0;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_end_usage = AOM_VBR;
        cfg.rc_target_bitrate = 100;
        cfg.rc_min_quantizer = 0;
        cfg.rc_max_quantizer = 63;
        cfg.kf_mode = AOM_KF_DISABLED;
        cfg.kf_min_dist = 0;
        cfg.kf_max_dist = 0;
        cfg.g_limit = 0;
    }
    
    err = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    
    int num_frames = (Size > 10) ? (Data[10] % 5) + 1 : 1;
    
    for (int i = 0; i < num_frames; i++) {
        if (create_test_image(&img, cfg.g_w, cfg.g_h, cfg.g_input_bit_depth, Data, Size) != 0) {
            break;
        }
        
        if (i > 0) {
            pts += duration;
        }
        
        err = aom_codec_encode(&ctx, &img, pts, duration, flags);
        if (err != AOM_CODEC_OK) {
            const char *err_str = aom_codec_err_to_string(err);
            const char *ctx_err = aom_codec_error(&ctx);
            (void)err_str;
            (void)ctx_err;
        }
        
        iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                if (pkt->data.frame.sz > 0 && pkt->data.frame.buf != NULL) {
                    free(pkt->data.frame.buf);
                }
            }
        }
        
        free_image(&img);
        frame_count++;
    }
    
    while (flush_count < 5) {
        err = aom_codec_encode(&ctx, NULL, pts, duration, 0);
        if (err != AOM_CODEC_OK) {
            const char *err_str = aom_codec_err_to_string(err);
            const char *ctx_err = aom_codec_error(&ctx);
            (void)err_str;
            (void)ctx_err;
        }
        
        iter = NULL;
        int got_data = 0;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            got_data = 1;
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                if (pkt->data.frame.sz > 0 && pkt->data.frame.buf != NULL) {
                    free(pkt->data.frame.buf);
                }
            }
        }
        
        if (!got_data) break;
        flush_count++;
    }
    
    aom_codec_destroy(&ctx);
    
    return 0;
}