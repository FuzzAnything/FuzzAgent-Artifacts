// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"

/* Helper to get the encoder interface */
static const vpx_codec_iface_t *get_encoder_interface(void) {
    /* Use VP8 encoder interface */
    return vpx_codec_vp8_cx();
}

/* Helper to initialize the encoder context */
static int init_encoder_context(vpx_codec_ctx_t *ctx, const vpx_codec_enc_cfg_t *cfg) {
    const vpx_codec_iface_t *iface = get_encoder_interface();
    if (!iface) return -1;

    if (vpx_codec_enc_init(ctx, iface, cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }
    return 0;
}

/* Helper to create a test image */
static vpx_image_t *create_test_image(vpx_codec_enc_cfg_t *cfg) {
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (cfg->g_bit_depth == VPX_BITS_10 || cfg->g_bit_depth == VPX_BITS_12) {
        fmt = VPX_IMG_FMT_I42016;
    }
    
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, cfg->g_w, cfg->g_h, 1);
    if (!img) return NULL;

    /* Fill with pattern based on width/height to ensure non-zero data */
    for (int p = 0; p < 3; p++) {
        unsigned char *plane = img->planes[p];
        int stride = img->stride[p];
        int h = (p == 0) ? cfg->g_h : (cfg->g_h + 1) / 2;
        int w = (p == 0) ? cfg->g_w : (cfg->g_w + 1) / 2;
        
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                plane[y * stride + x] = (unsigned char)((x + y) & 0xFF);
            }
        }
    }
    return img;
}

/* Helper to cleanup encoder context */
static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    vpx_codec_ctx_t ctx = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_codec_err_t err;
    
    /* Extract parameters from fuzz input */
    unsigned int width = Data[0] % 256 + 16;
    unsigned int height = Data[1] % 256 + 16;
    unsigned int bit_depth_val = Data[2] % 3; // 0:8, 1:10, 2:12
    unsigned int threads = (Data[3] % 4) + 1;
    unsigned int bitrate = (Data[4] * 256 + Data[5]) % 10000 + 100;
    
    /* Configure encoder settings */
    const vpx_codec_iface_t *iface = get_encoder_interface();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.rc_target_bitrate = bitrate;
    
    if (bit_depth_val == 0) cfg.g_bit_depth = VPX_BITS_8;
    else if (bit_depth_val == 1) cfg.g_bit_depth = VPX_BITS_10;
    else cfg.g_bit_depth = VPX_BITS_12;
    
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_end_usage = VPX_VBR;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 120;
    cfg.kf_max_dist = 240;

    /* Initialize context */
    if (init_encoder_context(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Create image */
    vpx_image_t *img = create_test_image(&cfg);
    if (!img) {
        cleanup_encoder_context(&ctx);
        return 0;
    }

    /* Test vpx_codec_enc_config_set */
    err = vpx_codec_enc_config_set(&ctx, &cfg);
    (void)err;

    /* Prepare custom buffer for vpx_codec_set_cx_data_buf */
    size_t buf_size = Size > 1024 ? Size : 1024;
    unsigned char *custom_buf = malloc(buf_size);
    if (custom_buf) {
        memset(custom_buf, 0, buf_size);
        vpx_fixed_buf_t custom_fixed_buf;
        custom_fixed_buf.buf = custom_buf;
        custom_fixed_buf.sz = buf_size;

        unsigned int pad_before = Data[6] % 100;
        unsigned int pad_after = Data[7] % 100;

        err = vpx_codec_set_cx_data_buf(&ctx, &custom_fixed_buf, pad_before, pad_after);
        (void)err;

        /* Encode a frame */
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = 0; // Infinite

        err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
        (void)err;

        /* Get output data */
        vpx_codec_cx_pkt_t pkt;
        while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
            if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
                /* Process packet if needed, here just consume */
                (void)pkt.data.frame.sz;
            }
        }

        /* Reset buffer to NULL to restore default behavior */
        vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
        
        free(custom_buf);
    }

    /* Flush the encoder */
    vpx_codec_pts_t current_pts = 1;
    vpx_codec_encode(&ctx, NULL, current_pts, 1, 0, 0);
    
    /* Drain remaining packets */
    vpx_codec_cx_pkt_t pkt;
    while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
        /* Consume */
    }

    /* Cleanup */
    vpx_img_free(img);
    cleanup_encoder_context(&ctx);

    return 0;
}