// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_image_t *create_test_image(vpx_codec_enc_cfg_t *cfg) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg->g_w, cfg->g_h, 1);
    if (!img) return NULL;

    // Fill with random-like data based on a simple pattern to ensure variety
    for (int i = 0; i < img->planes[0][0]; i++) {
        img->planes[VPX_PLANE_Y][i] = (unsigned char)(i % 256);
    }
    for (int i = 0; i < img->planes[1][0]; i++) {
        img->planes[VPX_PLANE_U][i] = (unsigned char)(i % 256);
    }
    for (int i = 0; i < img->planes[2][0]; i++) {
        img->planes[VPX_PLANE_V][i] = (unsigned char)(i % 256);
    }
    return img;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx, vpx_image_t *img, void *buf) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
    if (img) {
        vpx_img_free(img);
    }
    if (buf) {
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    vpx_codec_ctx_t ctx = {0};
    vpx_codec_enc_cfg_t cfg;
    // Use the correct function name for VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    // Initialize configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Safely read parameters from input data
    uint32_t p0 = 0, p1 = 0, p2 = 0, p3 = 0;
    if (Size >= 4) p0 = *(const uint32_t*)(Data);
    if (Size >= 8) p1 = *(const uint32_t*)(Data + 4);
    if (Size >= 12) p2 = *(const uint32_t*)(Data + 8);
    if (Size >= 16) p3 = *(const uint32_t*)(Data + 12);

    // Use input data to fuzz configuration parameters
    cfg.g_w = (p0 % 1024) + 16; // Ensure min size
    cfg.g_h = (p1 % 1024) + 16;
    cfg.g_threads = (p2 % 8) + 1;
    cfg.g_profile = p3 % 4;
    cfg.rc_target_bitrate = (p0 % 10000) + 100;
    cfg.rc_min_quantizer = p1 % 64;
    cfg.rc_max_quantizer = (p2 % 64) + cfg.rc_min_quantizer;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (p3 % 60) + 1;
    // Use enum values directly or cast from int
    cfg.rc_end_usage = (enum vpx_rc_mode)(p0 % 4);
    cfg.kf_mode = (enum vpx_kf_mode)(p1 % 3);

    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t *img = create_test_image(&cfg);
    if (!img) {
        cleanup_encoder(&ctx, NULL, NULL);
        return 0;
    }

    // Custom buffer for vpx_codec_set_cx_data_buf
    size_t buf_size = (Size % 65536) + 1024;
    void *custom_buf = malloc(buf_size);
    if (!custom_buf) {
        cleanup_encoder(&ctx, img, NULL);
        return 0;
    }

    vpx_fixed_buf_t fx_buf = { .buf = custom_buf, .sz = buf_size };
    unsigned int pad_before = p0 % 100;
    unsigned int pad_after = p1 % 100;

    // Fuzz vpx_codec_set_cx_data_buf
    vpx_codec_set_cx_data_buf(&ctx, &fx_buf, pad_before, pad_after);

    // Fuzz vpx_codec_enc_config_set with modified config
    cfg.g_w = (p2 % 1024) + 16;
    cfg.g_h = (p3 % 1024) + 16;
    vpx_codec_enc_config_set(&ctx, &cfg);

    // Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = (p0 % 1000) + 1;
    vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(p1 & 0xFFFF);
    vpx_enc_deadline_t deadline = (p2 % 1000000);

    // Encode a few frames
    for (int i = 0; i < 5; i++) {
        vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
        pts += duration;

        // Drain output
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Process packet if needed
            }
        }
    }

    // Flush encoder
    vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process packet if needed
        }
    }

    // Reset buffer to default
    vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);

    cleanup_encoder(&ctx, img, custom_buf);
    return 0;
}