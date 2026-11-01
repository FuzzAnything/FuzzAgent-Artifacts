// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int init_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg) {
    const vpx_codec_iface_t *iface;
    vpx_codec_err_t res;

    iface = vpx_codec_vp8_cx();
    if (!iface) return -1;

    res = vpx_codec_enc_init(ctx, iface, cfg, 0);
    if (res != VPX_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = NULL;
    vpx_fixed_buf_t custom_buf;
    uint8_t *buf_memory = NULL;
    vpx_codec_err_t res;
    const vpx_codec_cx_pkt_t *pkt_ptr;
    int frame_count = 0;
    vpx_codec_pts_t pts = 0;
    
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    unsigned int width = (Data[0] % 256) + 16;
    unsigned int height = (Data[1] % 256) + 16;
    unsigned int threads = (Data[2] % 4) + 1;
    unsigned int bitrate = (Data[3] % 100) + 100;

    // Ensure alignment
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;

    // Initialize config
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.rc_target_bitrate = bitrate;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = VPX_CBR;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 10;
    cfg.kf_max_dist = 200;

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Create image
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        cleanup_encoder(&ctx);
        return 0;
    }
    
    // Fill image with fuzz data if available
    if (Size > 0) {
        size_t y_size = width * height;
        size_t uv_size = (width * height) / 4;
        size_t total_needed = y_size + 2 * uv_size;
        size_t copy_len = (Size < total_needed) ? Size : total_needed;
        
        memcpy(img->planes[VPX_PLANE_Y], Data, copy_len);
        if (copy_len > y_size) {
            size_t uv_copy = (copy_len - y_size < uv_size) ? (copy_len - y_size) : uv_size;
            memcpy(img->planes[VPX_PLANE_U], Data + y_size, uv_copy);
            if (copy_len > y_size + uv_size) {
                memcpy(img->planes[VPX_PLANE_V], Data + y_size + uv_size, uv_copy);
            }
        }
    }

    // Allocate custom buffer for vpx_codec_set_cx_data_buf
    size_t buf_size = (Size > 0) ? Size * 2 : 4096;
    buf_memory = (uint8_t *)malloc(buf_size);
    if (buf_memory) {
        custom_buf.buf = buf_memory;
        custom_buf.sz = buf_size;
        
        // Try setting custom buffer
        unsigned int pad_before = (Data[0] % 10);
        unsigned int pad_after = (Data[1] % 10);
        res = vpx_codec_set_cx_data_buf(&ctx, &custom_buf, pad_before, pad_after);
        // Ignore error, just testing the call
    }

    // Encode loop
    while (frame_count < 10) {
        // Vary flags and deadlines based on input
        vpx_enc_frame_flags_t flags = 0;
        if (frame_count % 3 == 0) flags = VPX_EFLAG_FORCE_KF;
        // VPX_EFLAG_NO_DELAY is not a standard flag in libvpx, removed to fix compilation
        
        vpx_enc_deadline_t deadline = (frame_count % 2 == 0) ? 0 : 100000; // 0 or 100ms

        res = vpx_codec_encode(&ctx, img, pts, 1, flags, deadline);
        
        if (res != VPX_CODEC_OK) {
            // Continue to try flushing or just break if critical
            if (res == VPX_CODEC_INVALID_PARAM) {
                // Might be due to bad params, try flushing
                res = vpx_codec_encode(&ctx, NULL, pts, 1, 0, 0);
            }
        }

        // Retrieve packets
        while ((pkt_ptr = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Process packet (do nothing for fuzzing)
        }

        pts += 1;
        frame_count++;
    }

    // Flush the encoder
    res = vpx_codec_encode(&ctx, NULL, pts, 1, 0, 0);
    while ((pkt_ptr = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        // Drain remaining data
    }

    // Cleanup
    vpx_img_free(img);
    free(buf_memory);
    cleanup_encoder(&ctx);

    return 0;
}