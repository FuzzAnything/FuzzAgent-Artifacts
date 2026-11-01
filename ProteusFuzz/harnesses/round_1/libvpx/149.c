// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/* Helper to initialize encoder context */
static int init_encoder(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    const vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    /* Determine interface based on input data to vary fuzzing targets */
    if (size > 0 && data[0] & 0x01) {
        iface = vpx_codec_vp8_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }

    if (!iface) {
        return -1;
    }

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return -1;
    }

    /* Fuzz some configuration parameters safely with bounds checking */
    if (size > 0) {
        cfg.g_w = (data[0] % 16 + 1) * 16;
        cfg.g_h = (size > 1 ? (data[1] % 16 + 1) * 16 : 16);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (size > 2 ? (data[2] % 100) + 1 : 1);
        cfg.rc_target_bitrate = (size > 3 ? (data[3] % 1000) + 100 : 100);
        cfg.g_lag_in_frames = (size > 4 ? (data[4] % 20) : 0);
    }

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

/* Helper to clean up encoder context */
static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

/* Helper to create a dummy image frame */
static vpx_image_t *create_dummy_frame(const uint8_t *data, size_t size, int width, int height) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        return NULL;
    }

    /* Fill planes with fuzz data safely */
    size_t plane_size = (size_t)width * height;
    size_t u_plane_size = plane_size / 4;
    size_t v_plane_size = plane_size / 4;
    
    /* Calculate total bytes needed for Y, U, V planes */
    size_t total_needed = plane_size + u_plane_size + v_plane_size;
    
    /* Only copy what we have available, never exceed input buffer */
    size_t copy_size = (size < total_needed) ? size : total_needed;

    if (copy_size > 0) {
        /* Copy Y plane */
        size_t y_copy = (plane_size < copy_size) ? plane_size : copy_size;
        memcpy(img->planes[0], data, y_copy);
        
        /* Copy U plane if data remains */
        if (copy_size > y_copy) {
            size_t u_copy = (u_plane_size < (copy_size - y_copy)) ? u_plane_size : (copy_size - y_copy);
            memcpy(img->planes[1], data + y_copy, u_copy);
            
            /* Copy V plane if data remains */
            if (copy_size > y_copy + u_copy) {
                size_t v_copy = (v_plane_size < (copy_size - y_copy - u_copy)) ? v_plane_size : (copy_size - y_copy - u_copy);
                memcpy(img->planes[2], data + y_copy + u_copy, v_copy);
            }
        }
    }

    return img;
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    int width = 16, height = 16;
    int ctrl_id;
    int value;
    vpx_svc_ref_frame_config_t svc_config;
    vpx_image_t *img = NULL;

    if (Size < 2) {
        return 0;
    }

    /* Initialize context */
    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    /* Create a dummy frame for encoding */
    if (Size > 0) {
        width = (Data[0] % 32) + 16;
        height = (Data[1] % 32) + 16;
        img = create_dummy_frame(Data, Size, width, height);
    }

    /* Fuzz vpx_codec_control_ with various control IDs */
    if (Size > 2) {
        ctrl_id = Data[2];
        value = (Size > 3 && Data[3] % 2) ? 1 : 0;

        /* Try different control IDs based on input */
        switch (ctrl_id % 6) {
            case 0:
                /* VP8E_SET_SCREEN_CONTENT_MODE */
                res = vpx_codec_control_(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, value);
                break;
            case 1:
                /* VP9E_SET_DISABLE_LOOPFILTER */
                res = vpx_codec_control_(&ctx, VP9E_SET_DISABLE_LOOPFILTER, value);
                break;
            case 2:
                /* VP9E_SET_NOISE_SENSITIVITY */
                res = vpx_codec_control_(&ctx, VP9E_SET_NOISE_SENSITIVITY, value);
                break;
            case 3:
                /* VP9E_SET_FRAME_PARALLEL_DECODING */
                res = vpx_codec_control_(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, value);
                break;
            case 4:
                /* VP9E_GET_SVC_REF_FRAME_CONFIG */
                memset(&svc_config, 0, sizeof(svc_config));
                res = vpx_codec_control_(&ctx, VP9E_GET_SVC_REF_FRAME_CONFIG, &svc_config);
                break;
            case 5:
                /* VP9E_SET_SVC_REF_FRAME_CONFIG */
                memset(&svc_config, 0, sizeof(svc_config));
                /* Populate with fuzz data */
                if (Size > 10) {
                    for (int i = 0; i < 3; i++) {
                        svc_config.lst_fb_idx[i] = Data[4 + i] % 32;
                        svc_config.gld_fb_idx[i] = Data[7 + i] % 32;
                        svc_config.alt_fb_idx[i] = Data[10 + i] % 32;
                    }
                }
                res = vpx_codec_control_(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &svc_config);
                break;
            default:
                /* Invalid control ID to test error handling */
                res = vpx_codec_control_(&ctx, -1, 0);
                break;
        }

        /* Check return value for potential issues */
        if (res != VPX_CODEC_OK && res != VPX_CODEC_INCAPABLE && res != VPX_CODEC_ERROR) {
            /* Unexpected error, but continue fuzzing */
        }
    }

    /* Encode a frame if image was created */
    if (img) {
        res = vpx_codec_encode(&ctx, img, 0, 1, 0, 0);
        if (res == VPX_CODEC_OK) {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                /* Process packet if needed */
            }
        }
    }

    /* Cleanup */
    if (img) {
        vpx_img_free(img);
    }
    cleanup_encoder(&ctx);

    return 0;
}