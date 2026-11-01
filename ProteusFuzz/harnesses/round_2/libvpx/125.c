// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

// Fuzz driver for libvpx encoder APIs
// Targets: vpx_codec_get_cx_data, vpx_codec_enc_config_set, vpx_codec_encode, vpx_codec_set_cx_data_buf

static vpx_codec_ctx_t codec_ctx;
static vpx_codec_enc_cfg_t cfg;
static vpx_image_t *img = NULL;
static uint8_t *custom_buf = NULL;
static size_t custom_buf_size = 0;

static void cleanup_resources(void) {
    if (custom_buf) {
        free(custom_buf);
        custom_buf = NULL;
        custom_buf_size = 0;
    }
    if (img) {
        vpx_img_free(img);
        img = NULL;
    }
    if (codec_ctx.priv) {
        vpx_codec_destroy(&codec_ctx);
    }
}

static int init_encoder(const uint8_t *data, size_t size) {
    int width = 0, height = 0;
    
    if (size >= 4) {
        width = (data[0] & 0x3F) | ((data[1] & 0x3F) << 6);
        height = (data[2] & 0x3F) | ((data[3] & 0x3F) << 6);
    }
    
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Initialize config manually since vpx_codec_enc_cfg_init is not available in this context
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 63;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = VPX_VBR;
    cfg.kf_mode = VPX_KF_DISABLED;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 0;

    if (vpx_codec_enc_init(&codec_ctx, vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }

    // Initialize image planes with fuzz data
    size_t plane_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    
    memset(img->planes[0], 0, plane_size);
    memset(img->planes[1], 128, uv_size);
    memset(img->planes[2], 128, uv_size);

    if (size > 0) {
        size_t copy_size = (size < plane_size) ? size : plane_size;
        memcpy(img->planes[0], data, copy_size);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    cleanup_resources();

    if (!init_encoder(Data, Size)) {
        return 0;
    }

    // Fuzz vpx_codec_enc_config_set
    if (Size > 0) {
        // Modify some config fields based on input
        cfg.rc_target_bitrate = (Data[0] % 10000) + 100;
        cfg.rc_min_quantizer = Data[1] % 64;
        cfg.rc_max_quantizer = (Data[2] % 60) + cfg.rc_min_quantizer;
        cfg.g_threads = (Data[3] % 8) + 1;
        
        vpx_codec_enc_config_set(&codec_ctx, &cfg);
    }

    // Fuzz vpx_codec_set_cx_data_buf
    if (Size > 4) {
        size_t buf_size = (Size > 1024) ? 1024 : Size;
        size_t pad_before = Data[4] % 100;
        size_t pad_after = Data[5] % 100;
        
        if (custom_buf) free(custom_buf);
        custom_buf_size = buf_size + pad_before + pad_after;
        custom_buf = (uint8_t *)malloc(custom_buf_size);
        
        if (custom_buf) {
            vpx_fixed_buf_t buf;
            buf.buf = custom_buf + pad_before;
            buf.sz = buf_size;
            vpx_codec_set_cx_data_buf(&codec_ctx, &buf, pad_before, pad_after);
        }
    }

    // Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;

    if (Size > 6) {
        pts = (Data[6] << 8) | Data[7];
        duration = (Data[8] << 8) | Data[9];
        flags = Data[10];
        deadline = (Data[11] << 8) | Data[12];
    }

    // Encode a frame
    vpx_codec_encode(&codec_ctx, img, pts, duration, flags, deadline);

    // Fuzz vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Access frame data
            const void *data = pkt->data.frame.buf;
            size_t sz = pkt->data.frame.sz;
            (void)data;
            (void)sz;
        } else if (pkt->kind == VPX_CODEC_STATS_PKT) {
            // Access stats data
            const void *data = pkt->data.twopass_stats.buf;
            size_t sz = pkt->data.twopass_stats.sz;
            (void)data;
            (void)sz;
        }
        // Ignore other packet types as per documentation
    }

    // Flush encoder
    vpx_codec_encode(&codec_ctx, NULL, pts + 1, duration, flags, deadline);
    
    // Get remaining packets after flush
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        // Process packets
    }

    cleanup_resources();
    return 0;
}