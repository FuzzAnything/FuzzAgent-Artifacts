// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static void init_config(vpx_codec_enc_cfg_t *cfg, unsigned int w, unsigned int h) {
    if (!cfg) return;
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    // Manually initialize fields as vpx_codec_enc_cfg_init is not available in this context
    cfg->g_w = w;
    cfg->g_h = h;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    cfg->g_error_resilient = 0; // Default to 0 as VPX_ER_DEFAULT is not defined
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 750;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 120;
    cfg->kf_max_dist = 120;
    cfg->g_usage = 0;
    cfg->rc_dropframe_thresh = 0;
    cfg->rc_resize_allowed = 0;
    cfg->rc_scaled_width = 0;
    cfg->rc_scaled_height = 0;
    cfg->rc_resize_up_thresh = 0;
    cfg->rc_resize_down_thresh = 0;
    cfg->rc_twopass_stats_in.buf = NULL;
    cfg->rc_twopass_stats_in.sz = 0;
    cfg->rc_firstpass_mb_stats_in.buf = NULL;
    cfg->rc_firstpass_mb_stats_in.sz = 0;
    cfg->rc_undershoot_pct = 100;
    cfg->rc_overshoot_pct = 100;
    cfg->rc_2pass_vbr_bias_pct = 0;
    cfg->rc_2pass_vbr_minsection_pct = 0;
    cfg->rc_2pass_vbr_maxsection_pct = 0;
    cfg->rc_2pass_vbr_corpus_complexity = 0;
    cfg->ss_number_layers = 1;
    cfg->ts_number_layers = 1;
    cfg->ts_periodicity = 1;
    cfg->temporal_layering_mode = 0;
    cfg->use_vizier_rc_params = 0;
}

static vpx_image_t *create_test_image(unsigned int w, unsigned int h) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;

    // Fill with random-like data based on coordinates to ensure non-zero content
    for (int i = 0; i < img->h; i++) {
        for (int j = 0; j < img->w; j++) {
            img->planes[VPX_PLANE_Y][i * img->stride[VPX_PLANE_Y] + j] = (uint8_t)((i + j) % 256);
        }
    }
    for (int i = 0; i < img->h >> img->y_chroma_shift; i++) {
        for (int j = 0; j < img->w >> img->x_chroma_shift; j++) {
            img->planes[VPX_PLANE_U][i * img->stride[VPX_PLANE_U] + j] = 128;
            img->planes[VPX_PLANE_V][i * img->stride[VPX_PLANE_V] + j] = 128;
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    unsigned int w = Data[0] + 16;
    unsigned int h = Data[1] + 16;
    unsigned int num_enc = (Data[2] % 4) + 1; // 1 to 4 encoders for multi-test
    int use_multi = (Data[3] % 2);

    vpx_codec_ctx_t ctx;
    vpx_codec_ctx_t *ctx_arr[4];
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_cfg_t cfg_arr[4];
    vpx_rational_t dsf[4];
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    if (!iface) return 0;

    // Initialize config
    init_config(&cfg, w, h);

    // Create image
    vpx_image_t *img = create_test_image(w, h);
    if (!img) return 0;

    int result = 0;

    if (use_multi && num_enc > 1) {
        // Multi-encoder path
        vpx_codec_ctx_t *dynamic_ctxs = (vpx_codec_ctx_t *)malloc(num_enc * sizeof(vpx_codec_ctx_t));
        if (dynamic_ctxs) {
            for (int i = 0; i < num_enc; i++) {
                ctx_arr[i] = &dynamic_ctxs[i];
                init_config(&cfg_arr[i], w, h);
                dsf[i].num = 1;
                dsf[i].den = 1;
            }
            
            vpx_codec_err_t init_res = vpx_codec_enc_init_multi_ver(
                dynamic_ctxs, iface, cfg_arr, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION
            );
            
            if (init_res == VPX_CODEC_OK) {
                // Encode one frame per encoder
                for (int i = 0; i < num_enc; i++) {
                    vpx_codec_encode(ctx_arr[i], img, i, 1, 0, 0);
                    vpx_codec_cx_pkt_t pkt;
                    while (vpx_codec_get_cx_data(ctx_arr[i], &pkt) == VPX_CODEC_OK) {
                        // consume data
                    }
                }
                // Flush
                for (int i = 0; i < num_enc; i++) {
                    vpx_codec_encode(ctx_arr[i], NULL, 0, 0, 0, 0);
                    vpx_codec_cx_pkt_t pkt;
                    while (vpx_codec_get_cx_data(ctx_arr[i], &pkt) == VPX_CODEC_OK) {
                        // consume data
                    }
                    vpx_codec_destroy(ctx_arr[i]);
                }
            } else {
                // Error handling for multi-init
                vpx_codec_err_to_string(init_res);
            }
            free(dynamic_ctxs);
        }
    } else {
        // Single encoder path
        vpx_codec_err_t init_res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        if (init_res == VPX_CODEC_OK) {
            // Test vpx_codec_enc_config_set with a modified config
            vpx_codec_enc_cfg_t new_cfg = cfg;
            new_cfg.rc_target_bitrate = 200;
            vpx_codec_err_t set_res = vpx_codec_enc_config_set(&ctx, &new_cfg);
            if (set_res != VPX_CODEC_OK) {
                vpx_codec_err_to_string(set_res);
            }

            // Encode a frame
            vpx_codec_err_t enc_res = vpx_codec_encode(&ctx, img, 1, 1, 0, 0);
            if (enc_res != VPX_CODEC_OK) {
                vpx_codec_err_to_string(enc_res);
            }

            // Get output
            vpx_codec_cx_pkt_t pkt;
            while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
                // Consume packet
            }

            // Flush encoder
            vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
            while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
                // Consume packet
            }

            vpx_codec_destroy(&ctx);
        } else {
            // Handle init error
            vpx_codec_err_to_string(init_res);
        }
    }

    vpx_img_free(img);
    return 0;
}