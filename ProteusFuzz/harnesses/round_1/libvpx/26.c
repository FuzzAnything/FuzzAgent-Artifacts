// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

// Use a local context in the main function to avoid global state issues
// and potential double-free or invalid access across iterations.

static void cleanup_image(vpx_image_t *img) {
    if (img && img->planes[0]) {
        vpx_img_free(img);
        memset(img, 0, sizeof(vpx_image_t));
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_image_t img;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;

    // Initialize structures
    memset(&ctx, 0, sizeof(ctx));
    memset(&img, 0, sizeof(img));
    memset(&cfg, 0, sizeof(cfg));

    if (Size == 0) return 0;

    // Select interface: VP8
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data with strict bounds checking
    // Ensure dimensions are valid (min 16x16 for VP8)
    cfg.g_w = (Size > 0) ? ((Data[0] % 256) + 16) : 64;
    cfg.g_h = (Size > 1) ? ((Data[1] % 256) + 16) : 64;
    
    if (Size > 2) cfg.g_threads = (Data[2] % 8) + 1;
    if (Size > 3) cfg.rc_target_bitrate = (Data[3] % 1000) + 10;
    if (Size > 4) {
        int bd = Data[4] % 3;
        if (bd == 0) cfg.g_bit_depth = VPX_BITS_8;
        else if (bd == 1) cfg.g_bit_depth = VPX_BITS_10;
        else cfg.g_bit_depth = VPX_BITS_12;
    } else {
        cfg.g_bit_depth = VPX_BITS_8;
    }
    cfg.g_input_bit_depth = cfg.g_bit_depth;

    if (Size > 5) cfg.kf_mode = (Data[5] % 3);
    if (Size > 6) cfg.kf_min_dist = (Data[6] % 100) + 1;
    if (Size > 7) cfg.kf_max_dist = (Data[7] % 100) + cfg.kf_min_dist;
    if (Size > 8) cfg.rc_end_usage = (Data[8] % 4);

    // Initialize timebase
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Create context
    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate image
    vpx_image_t *img_ptr = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img_ptr) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    img = *img_ptr;

    // Fill image planes with fuzzing data to avoid uninitialized memory reads
    if (img.planes[0]) {
        size_t y_size = img.h * img.stride[0];
        if (Size >= y_size) {
            memcpy(img.planes[0], Data, y_size);
        } else {
            memset(img.planes[0], 0, y_size);
            if (Size > 0) memcpy(img.planes[0], Data, Size);
        }
    }
    if (img.planes[1]) {
        size_t u_size = (img.h >> img.y_chroma_shift) * img.stride[1];
        memset(img.planes[1], 128, u_size);
    }
    if (img.planes[2]) {
        size_t v_size = (img.h >> img.y_chroma_shift) * img.stride[2];
        memset(img.planes[2], 128, v_size);
    }

    // 1. Test vpx_codec_enc_config_set
    if (Size > 9) {
        vpx_codec_enc_cfg_t new_cfg = cfg;
        new_cfg.g_w = (Data[9] % 256) + 16;
        new_cfg.g_h = (Data[10] % 256) + 16;
        if (Size > 11) new_cfg.rc_target_bitrate = (Data[11] % 1000) + 10;
        vpx_codec_enc_config_set(&ctx, &new_cfg);
    }

    // 2. Test vpx_codec_control_ via vpx_codec_control macro
    if (Size > 12) {
        int ctrl_id = Data[12] % 1000;
        int param_val = (Size > 13) ? (Data[13] % 100) : 0;
        vpx_codec_control_(&ctx, ctrl_id, param_val);
    }

    // 3. Test vpx_codec_encode
    vpx_codec_pts_t pts = (Size > 14) ? (int64_t)Data[14] : 0;
    unsigned long duration = (Size > 15) ? Data[15] : 1;
    vpx_enc_frame_flags_t flags = (Size > 16) ? Data[16] : 0;
    vpx_enc_deadline_t deadline = (Size > 17) ? (Data[17] % 1000000) : 0;

    // Encode a frame
    vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);

    // Flush the encoder (NULL image)
    vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);

    // Drain the encoder until no more data
    while (1) {
        vpx_codec_cx_pkt_t pkt;
        if (vpx_codec_get_cx_data(&ctx, &pkt) != VPX_CODEC_OK) {
            break;
        }
        if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process packet if needed, currently just draining
        }
    }

    // Cleanup
    cleanup_image(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}