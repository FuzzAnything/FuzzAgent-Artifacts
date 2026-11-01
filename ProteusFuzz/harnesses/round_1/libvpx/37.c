// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void create_dummy_image(vpx_image_t *img, unsigned int w, unsigned int h, const uint8_t *data, size_t size) {
    if (!img || w == 0 || h == 0) return;

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
    img->bps = 8;
    img->user_priv = NULL;
    img->fb_priv = NULL;

    size_t y_size = w * h;
    size_t uv_size = (w / 2) * (h / 2);
    size_t total_needed = y_size + 2 * uv_size;

    if (size < total_needed) {
        size_t alloc_size = total_needed;
        unsigned char *buf = (unsigned char *)malloc(alloc_size);
        if (!buf) return;
        memset(buf, 0, alloc_size);
        img->img_data = buf;
        img->img_data_owner = 1;
        img->self_allocd = 1;
    } else {
        img->img_data = (unsigned char *)malloc(total_needed);
        if (!img->img_data) return;
        memcpy(img->img_data, data, total_needed);
        img->img_data_owner = 1;
        img->self_allocd = 1;
    }

    img->planes[0] = img->img_data;
    img->planes[1] = img->img_data + y_size;
    img->planes[2] = img->img_data + y_size + uv_size;
    img->planes[3] = NULL;

    img->stride[0] = w;
    img->stride[1] = w / 2;
    img->stride[2] = w / 2;
    img->stride[3] = 0;
}

static void cleanup_image(vpx_image_t *img) {
    if (img && img->img_data_owner && img->img_data) {
        free(img->img_data);
        img->img_data = NULL;
        img->img_data_owner = 0;
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 2 ? Data[2] : 30) + 1;
    cfg.rc_target_bitrate = (Size > 3 ? Data[3] : 100) + 10;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_usage = 0;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.rc_end_usage = VPX_VBR;
    cfg.kf_mode = VPX_KF_DISABLED;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 0;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.rc_buf_sz = 1000;
    cfg.rc_buf_initial_sz = 500;
    cfg.rc_buf_optimal_sz = 750;
    cfg.rc_dropframe_thresh = 0;
    cfg.rc_resize_allowed = 0;
    cfg.rc_scaled_width = cfg.g_w;
    cfg.rc_scaled_height = cfg.g_h;
    cfg.rc_resize_up_thresh = 0;
    cfg.rc_resize_down_thresh = 0;
    cfg.rc_undershoot_pct = 0;
    cfg.rc_overshoot_pct = 0;
    cfg.rc_2pass_vbr_bias_pct = 0;
    cfg.rc_2pass_vbr_minsection_pct = 0;
    cfg.rc_2pass_vbr_maxsection_pct = 0;
    cfg.rc_2pass_vbr_corpus_complexity = 0;
    cfg.ss_number_layers = 1;
    cfg.ts_number_layers = 1;
    cfg.ts_periodicity = 1;
    cfg.temporal_layering_mode = 0;
    cfg.use_vizier_rc_params = 0;

    if (vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }

    int ctrl_ids[] = {
        VP8E_SET_CPUUSED,
        VP8E_SET_TOKEN_PARTITIONS,
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_NOISE_SENSITIVITY,
        VP8E_SET_SHARPNESS,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_ARNR_MAXFRAMES,
        VP8E_SET_ARNR_STRENGTH,
        VP8E_SET_ARNR_TYPE,
        -1,
        9999
    };

    for (int i = 0; i < sizeof(ctrl_ids) / sizeof(ctrl_ids[0]); i++) {
        int id = ctrl_ids[i];
        vpx_codec_err_t res = vpx_codec_control_(&ctx, id, NULL);
        (void)res;
    }

    vpx_image_t img;
    memset(&img, 0, sizeof(img));
    create_dummy_image(&img, cfg.g_w, cfg.g_h, Data, Size);

    if (img.planes[0]) {
        vpx_codec_pts_t pts = 0;
        unsigned long duration = (Size > 4 ? Data[4] : 1) + 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = VPX_DL_REALTIME;

        for (int i = 0; i < 3; i++) {
            vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
            pts += duration;
        }

        vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
    }

    cleanup_image(&img);

    if (vpx_codec_enc_config_set(&ctx, &cfg) != VPX_CODEC_OK) {
        // Expected to fail sometimes
    }

    vpx_codec_destroy(&ctx);

    return 0;
}