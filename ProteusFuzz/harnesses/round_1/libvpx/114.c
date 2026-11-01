// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_KEY_FRAME_FILTERING__value at vp8cx.h:1109:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int create_dummy_image(vpx_image_t *img, unsigned int w, unsigned int h, const uint8_t *data, size_t size) {
    if (!img || w == 0 || h == 0) return 0;

    img->fmt = VPX_IMG_FMT_I420;
    img->cs = VPX_CS_BT_601;
    img->range = VPX_CR_STUDIO_RANGE;
    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->r_w = w;
    img->r_h = h;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 0;
    img->user_priv = NULL;
    img->fb_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;

    size_t y_size = w * h;
    size_t uv_size = (w * h) / 4;
    size_t total_needed = y_size + 2 * uv_size;

    if (size < total_needed) {
        unsigned char *buf = malloc(total_needed);
        if (!buf) return 0;
        memset(buf, 0, total_needed);
        if (size > 0) memcpy(buf, data, size);
        
        img->planes[0] = buf;
        img->planes[1] = buf + y_size;
        img->planes[2] = buf + y_size + uv_size;
        img->planes[3] = NULL;
        img->stride[0] = w;
        img->stride[1] = w / 2;
        img->stride[2] = w / 2;
        img->stride[3] = 0;
        img->img_data = buf;
        img->img_data_owner = 1;
        img->self_allocd = 1;
    } else {
        img->planes[0] = (unsigned char *)data;
        img->planes[1] = (unsigned char *)data + y_size;
        img->planes[2] = (unsigned char *)data + y_size + uv_size;
        img->planes[3] = NULL;
        img->stride[0] = w;
        img->stride[1] = w / 2;
        img->stride[2] = w / 2;
        img->stride[3] = 0;
        img->img_data = (unsigned char *)data;
        img->img_data_owner = 0;
        img->self_allocd = 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    // Parse dimensions from input to vary test cases
    unsigned int w = (Data[0] % 128) + 16;
    unsigned int h = (Data[1] % 128) + 16;
    
    // Ensure dimensions are multiples of 2 for I420
    w = (w / 2) * 2;
    h = (h / 2) * 2;
    if (w < 16) w = 16;
    if (h < 16) h = 16;

    // Initialize configuration manually
    cfg.g_usage = 0;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Rate control settings
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 10;
    cfg.rc_max_quantizer = 50;
    cfg.rc_buf_sz = 1000;
    cfg.rc_buf_initial_sz = 500;
    cfg.rc_buf_optimal_sz = 750;
    
    // Keyframe settings
    cfg.kf_mode = VPX_KF_DISABLED;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 0;

    // Initialize encoder with VP8 as a fallback if VP9 is not available
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        iface = vpx_codec_vp8_cx();
    }
    
    if (!iface) return 0;

    vpx_codec_err_t err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t img;
    memset(&img, 0, sizeof(img));

    // Fuzz vpx_codec_enc_config_set
    // Reconfigure with slightly modified values based on input
    cfg.g_threads = (Data[2] % 16) + 1;
    cfg.rc_target_bitrate = (Data[3] % 500) + 500;
    vpx_codec_enc_config_set(&ctx, &cfg);

    // Fuzz vpx_codec_control_ with various control IDs
    // VP9E_SET_TILE_COLUMNS
    int tile_cols = (Data[4] % 16) + 1;
    vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);

    // VP9E_SET_SVC
    int svc_val = (Data[5] % 2);
    vpx_codec_control(&ctx, VP9E_SET_SVC, svc_val);

    // VP9E_SET_KEY_FRAME_FILTERING
    int kf_filter = (Data[6] % 2);
    vpx_codec_control(&ctx, VP9E_SET_KEY_FRAME_FILTERING, kf_filter);

    // Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = (Data[7] % 100) + 1;
    vpx_enc_frame_flags_t flags = (Data[8] % 4);
    vpx_enc_deadline_t deadline = (Data[9] % 3 == 0) ? 0 : (Data[10] % 10000) + 1000;

    // Create dummy image
    if (create_dummy_image(&img, w, h, Data, Size)) {
        // Encode a few frames
        for (int i = 0; i < 3; i++) {
            vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
            pts += duration;
            
            // Drain any output
            vpx_codec_cx_pkt_t pkt;
            while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
                if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Process frame data if needed
                }
            }
        }
        
        // Flush the encoder
        vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
        
        // Drain remaining data
        vpx_codec_cx_pkt_t pkt_flush;
        while (vpx_codec_get_cx_data(&ctx, &pkt_flush) == VPX_CODEC_OK) {
            // Drain remaining data
        }
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    
    // Free allocated image memory if owned
    if (img.img_data_owner && img.img_data) {
        free(img.img_data);
    }

    return 0;
}