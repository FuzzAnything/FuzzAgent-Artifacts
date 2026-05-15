// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"
#include "aom_encoder.h"

static void init_image(aom_image_t *img, const uint8_t *data, size_t size) {
    memset(img, 0, sizeof(*img));
    
    static const aom_img_fmt_t formats[] = {
        AOM_IMG_FMT_I420,
        AOM_IMG_FMT_YV12,
        AOM_IMG_FMT_I422,
        AOM_IMG_FMT_I444,
        AOM_IMG_FMT_NV12,
        AOM_IMG_FMT_I42016,
        AOM_IMG_FMT_I42216,
        AOM_IMG_FMT_I44416
    };
    
    if (size < 16) return;
    
    img->fmt = formats[data[0] % (sizeof(formats)/sizeof(formats[0]))];
    img->w = 64 + (data[1] % 64);
    img->h = 64 + (data[2] % 64);
    img->d_w = img->w;
    img->d_h = img->h;
    img->r_w = img->w;
    img->r_h = img->h;
    img->bit_depth = (data[3] & 1) ? 10 : 8;
    img->range = (data[4] & 1) ? AOM_CR_FULL_RANGE : AOM_CR_STUDIO_RANGE;
    img->csp = (aom_chroma_sample_position_t)(data[5] % 4);
    img->monochrome = data[6] & 1;
    img->cp = (aom_color_primaries_t)(data[7] % 24);
    img->tc = (aom_transfer_characteristics_t)(data[8] % 20);
    img->mc = (aom_matrix_coefficients_t)(data[9] % 16);
    
    size_t plane_size = img->w * img->h;
    if (img->fmt == AOM_IMG_FMT_I420 || img->fmt == AOM_IMG_FMT_YV12 || 
        img->fmt == AOM_IMG_FMT_NV12 || img->fmt == AOM_IMG_FMT_I42016) {
        plane_size = plane_size * 3 / 2;
    } else if (img->fmt == AOM_IMG_FMT_I422 || img->fmt == AOM_IMG_FMT_I42216) {
        plane_size = plane_size * 2;
    } else if (img->fmt == AOM_IMG_FMT_I444 || img->fmt == AOM_IMG_FMT_I44416) {
        plane_size = plane_size * 3;
    }
    
    if (plane_size > 0 && plane_size <= 1024 * 1024) {
        img->img_data = malloc(plane_size);
        if (img->img_data) {
            img->img_data_owner = 1;
            img->self_allocd = 1;
            size_t copy_size = size - 16;
            if (copy_size > plane_size) copy_size = plane_size;
            memcpy(img->img_data, data + 16, copy_size);
            
            img->stride[0] = img->w;
            img->stride[1] = img->w / 2;
            img->stride[2] = img->w / 2;
            img->planes[0] = img->img_data;
            img->planes[1] = img->img_data + img->w * img->h;
            img->planes[2] = img->img_data + img->w * img->h * 5 / 4;
        }
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img && img->img_data_owner && img->img_data) {
        free(img->img_data);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
    if (size < 100) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64 + (data[0] % 192);
    cfg.g_h = 64 + (data[1] % 192);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (data[2] % 9000);
    cfg.g_threads = 1 + (data[3] % 4);
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = data[4] % 16;
    cfg.g_error_resilient = data[5] & 1;
    cfg.kf_min_dist = data[7] % 32;
    cfg.kf_max_dist = 32 + (data[8] % 96);
    cfg.rc_end_usage = (enum aom_rc_mode)(data[9] % 4);
    cfg.rc_min_quantizer = data[10] % 64;
    cfg.rc_max_quantizer = 10 + (data[11] % 54);
    cfg.g_bit_depth = (data[12] & 1) ? AOM_BITS_10 : AOM_BITS_8;
    cfg.g_profile = data[13] % 4;
    
    aom_codec_err_t init_res = aom_codec_enc_init_ver(&ctx, NULL, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (init_res != AOM_CODEC_OK) {
        return 0;
    }
    
    aom_image_t img;
    init_image(&img, data + 20, size - 20);
    
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    int num_frames = 1 + (data[14] % 5);
    for (int i = 0; i < num_frames; i++) {
        if (i == num_frames - 1) {
            aom_codec_encode(&ctx, NULL, pts, duration, flags);
        } else {
            aom_codec_encode(&ctx, &img, pts, duration, flags);
            pts += 1;
            flags = data[15 + i] & 3;
        }
        
        const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
        (void)preview;
    }
    
    int num_controls = data[16] % 10;
    for (int i = 0; i < num_controls; i++) {
        int ctrl_id = 1 + (data[17 + i * 2] % 100);
        int ctrl_val = data[18 + i * 2];
        
        aom_codec_control(&ctx, ctrl_id, ctrl_val);
    }
    
    aom_fixed_buf_t buf;
    unsigned char buffer_data[4096];
    buf.buf = buffer_data;
    buf.sz = sizeof(buffer_data);
    
    if (data[19] & 1) {
        aom_codec_set_cx_data_buf(&ctx, &buf, data[20] % 64, data[21] % 64);
    } else {
        aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
    }
    
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        (void)pkt;
    }
    
    cleanup_image(&img);
    aom_codec_destroy(&ctx);
    
    return 0;
}