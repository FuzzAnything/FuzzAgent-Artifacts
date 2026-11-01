// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

/*
 * Helper to initialize a vpx_image_t with fuzzed data.
 * Returns a pointer to the image, or NULL on failure.
 * The caller must free the image using vpx_img_free().
 */
static vpx_image_t *create_fuzzed_image(const uint8_t *data, size_t size, unsigned int w, unsigned int h) {
    if (w == 0 || h == 0) return NULL;
    
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;

    /* Fill planes with fuzzed data if available, otherwise zeros */
    size_t plane_size = img->stride[0] * h;
    size_t uv_size = img->stride[1] * (h / 2);
    
    size_t total_needed = plane_size + uv_size + uv_size;
    
    if (size >= total_needed) {
        memcpy(img->planes[0], data, plane_size);
        memcpy(img->planes[1], data + plane_size, uv_size);
        memcpy(img->planes[2], data + plane_size + uv_size, uv_size);
    } else if (size > 0) {
        memset(img->planes[0], 0, plane_size);
        memset(img->planes[1], 0, uv_size);
        memset(img->planes[2], 0, uv_size);
        
        /* Fill with available data up to the limit */
        size_t y_avail = (size < plane_size) ? size : plane_size;
        if (y_avail > 0) memcpy(img->planes[0], data, y_avail);
        
        size_t remaining = size - y_avail;
        if (remaining > 0) {
            size_t u_avail = (remaining < uv_size) ? remaining : uv_size;
            memcpy(img->planes[1], data + y_avail, u_avail);
            
            remaining -= u_avail;
            if (remaining > 0) {
                size_t v_avail = (remaining < uv_size) ? remaining : uv_size;
                memcpy(img->planes[2], data + y_avail + u_avail, v_avail);
            }
        }
    }

    return img;
}

static uint8_t safe_read_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data for basic checks */
    if (Size < 4) return 0;

    /* Extract parameters from input data safely */
    uint32_t width = safe_read_byte(Data, Size, 0) | (safe_read_byte(Data, Size, 1) << 8);
    uint32_t height = safe_read_byte(Data, Size, 2) | (safe_read_byte(Data, Size, 3) << 8);
    
    /* Sanity checks to prevent huge allocations or invalid dimensions */
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    /* Use vpx_codec_vp9_cx as the correct interface function for VP9 encoder */
    const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (!iface) return 0;

    /* Initialize configuration */
    err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    /* Apply fuzzed configuration values safely */
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = (safe_read_byte(Data, Size, 4) % 8) + 1;
    cfg.g_profile = (safe_read_byte(Data, Size, 5) % 4);
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = VPX_BITS_8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (safe_read_byte(Data, Size, 6) % 60) + 1;
    cfg.g_error_resilient = (safe_read_byte(Data, Size, 7) % 3);
    cfg.g_pass = (safe_read_byte(Data, Size, 8) % 3);
    cfg.g_lag_in_frames = (safe_read_byte(Data, Size, 9) % 10);
    
    cfg.rc_end_usage = (safe_read_byte(Data, Size, 10) % 4);
    cfg.rc_target_bitrate = (safe_read_byte(Data, Size, 11) % 5000) + 100;
    cfg.rc_min_quantizer = (safe_read_byte(Data, Size, 12) % 63);
    cfg.rc_max_quantizer = (safe_read_byte(Data, Size, 13) % 63) + 1;
    
    cfg.kf_mode = (safe_read_byte(Data, Size, 14) % 3);
    cfg.kf_min_dist = (safe_read_byte(Data, Size, 15) % 100);
    cfg.kf_max_dist = (safe_read_byte(Data, Size, 16) % 200) + 10;

    /* Initialize encoder */
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        /* Initialization failed, nothing to clean up per docs */
        return 0;
    }

    /* Fuzz VP9E_SET_SVC if available (via control) */
    int ctrl_id = (safe_read_byte(Data, Size, 17) % 1000); 
    vpx_codec_err_t ctrl_err = vpx_codec_control_(&ctx, ctrl_id, NULL);
    (void)ctrl_err;

    /* Fuzz VP9E_SET_ROI_MAP if available */
    int roi_ctrl = (safe_read_byte(Data, Size, 18) % 1000);
    ctrl_err = vpx_codec_control_(&ctx, roi_ctrl, NULL);
    (void)ctrl_err;

    /* Create a fuzzed image */
    vpx_image_t *img = create_fuzzed_image(Data, Size, width, height);
    
    if (img) {
        /* Encode a frame */
        vpx_codec_pts_t pts = (vpx_codec_pts_t)(
            safe_read_byte(Data, Size, 19) | 
            (safe_read_byte(Data, Size, 20) << 8) | 
            (safe_read_byte(Data, Size, 21) << 16) | 
            (safe_read_byte(Data, Size, 22) << 24)
        );
        unsigned long duration = (safe_read_byte(Data, Size, 23) % 100) + 1;
        vpx_enc_frame_flags_t flags = (safe_read_byte(Data, Size, 24) % 8);
        vpx_enc_deadline_t deadline = (safe_read_byte(Data, Size, 25) % 1000000);

        err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
        
        if (err == VPX_CODEC_OK) {
            /* Retrieve encoded data */
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                /* Process packet to ensure it's read, avoiding unused warnings */
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    /* Access data to trigger any potential issues in the packet structure */
                    (void)pkt->data.frame.buf;
                    (void)pkt->data.frame.sz;
                }
            }
        }
        
        vpx_img_free(img);
    }

    /* Flush the encoder */
    err = vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
    if (err == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            /* Consume flush packets */
        }
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}