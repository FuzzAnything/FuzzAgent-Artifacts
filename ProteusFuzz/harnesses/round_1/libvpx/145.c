// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static void cleanup_ctx(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

static void cleanup_images(vpx_image_t *img, int count) {
    for (int i = 0; i < count; i++) {
        if (img[i].img_data) {
            vpx_img_free(&img[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    uint32_t seed = 0;
    if (Size >= 4) {
        memcpy(&seed, Data, 4);
    }

    // Test interface retrieval and capabilities
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    
    if (vp8_iface) {
        vpx_codec_get_caps(vp8_iface);
        vpx_codec_iface_name(vp8_iface);
    }

    // Test with NULL interface
    vpx_codec_get_caps(NULL);
    vpx_codec_iface_name(NULL);

    // Prepare configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *target_iface = vp8_iface;
    
    if (!target_iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(target_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    cfg.g_w = (seed & 0x3FF) + 16; // 16 to 1023
    cfg.g_h = ((seed >> 10) & 0x3FF) + 16;
    cfg.g_threads = (seed >> 20) & 0xFF;
    cfg.g_timebase.num = (seed >> 28) & 0xFF;
    cfg.g_timebase.den = ((seed >> 28) & 0xFF) + 1;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.rc_target_bitrate = ((seed >> 16) & 0xFFFF) + 100;
    cfg.kf_min_dist = ((seed >> 4) & 0xFF) + 1;
    cfg.kf_max_dist = ((seed >> 4) & 0xFF) + 10;

    // Allocate context and image
    vpx_codec_ctx_t ctx;
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img.planes[0]) {
        return 0;
    }

    // Initialize encoder
    vpx_codec_err_t err;
    err = vpx_codec_enc_init(&ctx, target_iface, &cfg, 0);

    if (err != VPX_CODEC_OK) {
        cleanup_images(&img, 1);
        return 0;
    }

    // Encode a frame
    vpx_codec_pts_t pts = (vpx_codec_pts_t)((seed >> 8) & 0xFFFF);
    unsigned long duration = ((seed >> 24) & 0xFF) + 1;
    vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(seed & 0xFF);
    vpx_enc_deadline_t deadline = ((seed >> 12) & 0xFFFF) * 1000; // microseconds

    err = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
    (void)err;

    // Flush encoder
    err = vpx_codec_encode(&ctx, NULL, pts + duration, duration, 0, 0);
    (void)err;

    // Cleanup
    cleanup_ctx(&ctx);
    cleanup_images(&img, 1);

    return 0;
}