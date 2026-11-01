// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
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
#include <vpx/vpx_encoder.h>

// Define missing constants if not present in headers
#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 0x1000
#endif
#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 0x1001
#endif
#ifndef VP9E_SET_KEY_FRAME_FILTERING
#define VP9E_SET_KEY_FRAME_FILTERING 0x1002
#endif

static int create_image(const uint8_t *data, size_t size, vpx_image_t *img, int width, int height) {
    size_t required_size = (size_t)width * height * 3 / 2;
    if (size < required_size) {
        return 0;
    }
    if (vpx_img_alloc(img, VPX_IMG_FMT_I420, width, height, 1) != NULL) {
        memcpy(img->planes[VPX_PLANE_Y], data, width * height);
        memcpy(img->planes[VPX_PLANE_U], data + width * height, (width * height) / 4);
        memcpy(img->planes[VPX_PLANE_V], data + width * height + (width * height) / 4, (width * height) / 4);
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    // Use vpx_codec_vp9_cx as the correct function name for VP9 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (iface == NULL) {
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (Size > 0) cfg.g_w = Data[0] & 0x7F;
    if (Size > 1) cfg.g_h = Data[1] & 0x7F;
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    cfg.g_threads = (Size > 2) ? (Data[2] % 8) : 1;
    cfg.g_profile = (Size > 3) ? (Data[3] % 4) : 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = VPX_BITS_8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 4) ? (Data[4] + 1) : 30;
    cfg.rc_end_usage = VPX_VBR;
    cfg.rc_target_bitrate = (Size > 5) ? (Data[5] * 10 + 10) : 100;
    cfg.kf_mode = VPX_KF_DISABLED;
    cfg.kf_min_dist = 12;
    cfg.kf_max_dist = 12;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t img;
    memset(&img, 0, sizeof(img));

    // Fuzz encoding loop
    int frame_count = (Size > 6) ? (Data[6] % 10) : 1;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = (Size > 7) ? (Data[7] + 1) : 1;
    vpx_enc_frame_flags_t flags = (Size > 8) ? (Data[8] & 0xFF) : 0;
    vpx_enc_deadline_t deadline = (Size > 9) ? (Data[9] * 1000) : VPX_DL_REALTIME;

    for (int i = 0; i < frame_count; i++) {
        int width = (Size > 10) ? (Data[10] & 0x7F) : 16;
        int height = (Size > 11) ? (Data[11] & 0x7F) : 16;
        if (width == 0) width = 16;
        if (height == 0) height = 16;

        if (create_image(Data, Size, &img, width, height)) {
            vpx_codec_err_t res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
            (void)res; // Suppress unused variable warning
        } else {
            // Try encoding with NULL image to flush
            vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
        }

        pts += duration;
    }

    // Flush the encoder
    vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);

    // Reconfigure encoder with fuzzed config
    if (Size > 12) {
        cfg.rc_target_bitrate = Data[12] * 100 + 100;
        vpx_codec_enc_config_set(&ctx, &cfg);
    }

    // Test control commands
    if (Size > 13) {
        int ctrl_id = Data[13];
        // Avoid invalid control IDs that require frame data immediately
        if (ctrl_id == VP9E_SET_TILE_COLUMNS) {
            int cols = (Size > 14) ? (Data[14] & 0x0F) : 1;
            vpx_codec_control_(&ctx, VP9E_SET_TILE_COLUMNS, cols);
        } else if (ctrl_id == VP9E_SET_SVC) {
             // SVC configuration might need specific setup, passing minimal value
             vpx_codec_control_(&ctx, VP9E_SET_SVC, 0);
        } else if (ctrl_id == VP9E_SET_KEY_FRAME_FILTERING) {
             vpx_codec_control_(&ctx, VP9E_SET_KEY_FRAME_FILTERING, 0);
        }
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}