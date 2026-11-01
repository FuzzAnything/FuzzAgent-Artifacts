// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

/* 
 * Fuzz driver for libvpx encoder functions.
 * Targets: vpx_codec_enc_config_set, vpx_codec_encode, vpx_codec_set_cx_data_buf.
 */

static void init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!cfg) return;
    
    // Initialize config struct manually since vpx_codec_enc_cfg_init might not be available
    // or requires specific headers not guaranteed in this environment.
    // We zero it out and set defaults.
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Set default values
    cfg->g_w = 16;
    cfg->g_h = 16;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 120;
    cfg->kf_max_dist = 240;
    cfg->g_pass = VPX_RC_ONE_PASS;

    if (!data || size < 20) return;

    // Fuzz generic settings
    cfg->g_w = (data[0] % 32 + 1) * 16; // Ensure multiple of 16
    cfg->g_h = (data[1] % 32 + 1) * 16;
    
    cfg->g_threads = (data[2] % 8) + 1;
    cfg->g_profile = data[3] % 4;
    
    // Bit depth: 8, 10, or 12
    int bd_idx = data[4] % 3;
    if (bd_idx == 0) cfg->g_bit_depth = VPX_BITS_8;
    else if (bd_idx == 1) cfg->g_bit_depth = VPX_BITS_10;
    else cfg->g_bit_depth = VPX_BITS_12;

    // Timebase
    cfg->g_timebase.num = (data[5] % 100) + 1;
    cfg->g_timebase.den = (data[6] % 100) + 1;

    // Rate control
    cfg->rc_end_usage = (enum vpx_rc_mode)(data[7] % 4);
    cfg->rc_target_bitrate = (data[8] * 10) + 10;
    cfg->rc_min_quantizer = data[9] % 64;
    cfg->rc_max_quantizer = data[10] % 64;
    if (cfg->rc_max_quantizer < cfg->rc_min_quantizer) cfg->rc_max_quantizer = cfg->rc_min_quantizer;

    // Keyframe
    cfg->kf_mode = (enum vpx_kf_mode)(data[11] % 3);
    cfg->kf_min_dist = (data[12] * 5) + 1;
    cfg->kf_max_dist = (data[13] * 5) + 1;
    if (cfg->kf_max_dist < cfg->kf_min_dist) cfg->kf_max_dist = cfg->kf_min_dist;

    // Pass
    cfg->g_pass = (enum vpx_enc_pass)(data[14] % 3);
}

static vpx_image_t *create_fuzz_image(const uint8_t *data, size_t size, unsigned int w, unsigned int h, vpx_bit_depth_t depth) {
    if (!data || size == 0) return NULL;
    
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 16);
    if (!img) return NULL;

    // Fill planes with fuzz data
    size_t plane_size = 0;
    int bit_depth_val = (depth == VPX_BITS_10 || depth == VPX_BITS_12) ? 2 : 1;
    
    // Y plane
    plane_size = w * h * bit_depth_val;
    if (plane_size > 0 && plane_size <= size) {
        memcpy(img->planes[VPX_PLANE_Y], data, plane_size);
    } else if (plane_size > 0) {
        memset(img->planes[VPX_PLANE_Y], 0, plane_size);
    }

    // U and V planes
    int w_uv = (w + 1) / 2;
    int h_uv = (h + 1) / 2;
    plane_size = w_uv * h_uv * bit_depth_val;
    
    size_t offset = plane_size;
    if (offset + plane_size <= size) {
        memcpy(img->planes[VPX_PLANE_U], data + offset, plane_size);
        offset += plane_size;
    } else {
        memset(img->planes[VPX_PLANE_U], 0, plane_size);
    }

    if (offset + plane_size <= size) {
        memcpy(img->planes[VPX_PLANE_V], data + offset, plane_size);
    } else {
        memset(img->planes[VPX_PLANE_V], 0, plane_size);
    }

    return img;
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    
    // Get VP8 encoder interface
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    // Initialize configuration
    init_config(&cfg, Data, Size);

    // Initialize encoder
    err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Extract dimensions for image creation
    unsigned int w = cfg.g_w;
    unsigned int h = cfg.g_h;
    vpx_bit_depth_t depth = cfg.g_bit_depth;

    // Create a custom buffer for vpx_codec_set_cx_data_buf
    uint8_t *custom_buf = NULL;
    vpx_fixed_buf_t custom_buf_struct = {0};
    
    // Fuzz buffer allocation based on input
    if (Size > 20) {
        size_t buf_size = (Data[20] % 10) * 1024 + 1024;
        if (buf_size > 0 && buf_size < 1024 * 1024) {
            custom_buf = (uint8_t *)malloc(buf_size);
            if (custom_buf) {
                memset(custom_buf, 0, buf_size);
                custom_buf_struct.buf = custom_buf;
                custom_buf_struct.sz = buf_size;
            }
        }
    }

    // Test vpx_codec_set_cx_data_buf
    if (custom_buf) {
        unsigned int pad_before = (Size > 21) ? Data[21] % 100 : 0;
        unsigned int pad_after = (Size > 22) ? Data[22] % 100 : 0;
        vpx_codec_set_cx_data_buf(&ctx, &custom_buf_struct, pad_before, pad_after);
    }

    // Create image and encode
    vpx_image_t *img = create_fuzz_image(Data, Size, w, h, depth);
    if (!img) {
        // Try to encode NULL to flush
        vpx_codec_encode(&ctx, NULL, 0, 1, 0, 0);
        goto cleanup;
    }

    // Fuzz encoding parameters
    vpx_codec_pts_t pts = 0;
    unsigned long duration = (Size > 23) ? (Data[23] % 100) + 1 : 1;
    vpx_enc_frame_flags_t flags = (Size > 24) ? Data[24] : 0;
    vpx_enc_deadline_t deadline = (Size > 25) ? (Data[25] * 1000) : 0;

    // Encode a few frames to explore states
    for (int i = 0; i < 3; i++) {
        if (pts < 0) break; // Prevent overflow
        err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
        if (err != VPX_CODEC_OK) {
            // Continue to flush
            vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
            break;
        }
        pts += duration;
    }

    // Flush encoder
    vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);

cleanup:
    if (img) vpx_img_free(img);
    if (custom_buf) free(custom_buf);
    vpx_codec_destroy(&ctx);

    return 0;
}