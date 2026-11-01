// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec_ctx;
static vpx_codec_enc_cfg_t cfg;
static vpx_image_t img;
static uint8_t *custom_buf = NULL;
static size_t custom_buf_size = 0;

static void cleanup_resources(void) {
    if (custom_buf) {
        free(custom_buf);
        custom_buf = NULL;
        custom_buf_size = 0;
    }
    if (codec_ctx.priv) {
        vpx_codec_destroy(&codec_ctx);
    }
    if (img.planes[0]) {
        vpx_img_free(&img);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    cleanup_resources();

    // Initialize encoder configuration
    // vpx_codec_vp8_dx() returns a pointer, so we pass it directly, not its address
    if (vpx_codec_enc_config_default(vpx_codec_vp8_dx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (Size > 0) {
        cfg.g_w = Data[0] + 16;
        cfg.g_h = (Size > 1 ? Data[1] : 0) + 16;
        cfg.g_timebase.num = (Size > 2 ? Data[2] : 1);
        cfg.g_timebase.den = (Size > 3 ? Data[3] : 1);
        cfg.rc_target_bitrate = (Size > 4 ? Data[4] : 100);
        cfg.g_threads = (Size > 5 ? Data[5] % 8 : 1);
        cfg.g_profile = (Size > 6 ? Data[6] % 4 : 0);
        cfg.g_bit_depth = VPX_BITS_8;
        cfg.g_input_bit_depth = 8;
        cfg.rc_end_usage = (Size > 7 ? Data[7] % 4 : VPX_VBR);
        cfg.kf_mode = (Size > 8 ? Data[8] % 3 : VPX_KF_AUTO);
        cfg.kf_min_dist = (Size > 9 ? Data[9] : 12);
        cfg.kf_max_dist = (Size > 10 ? Data[10] : 120);
        cfg.rc_min_quantizer = (Size > 11 ? Data[11] % 64 : 2);
        cfg.rc_max_quantizer = (Size > 12 ? Data[12] % 64 : 63);
    }

    // Initialize encoder context
    // Pass the interface pointer directly
    if (vpx_codec_enc_init(&codec_ctx, vpx_codec_vp8_dx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate image buffer
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        cleanup_resources();
        return 0;
    }

    // Fill image data with fuzz input
    size_t img_size = img.planes[0] ? img.stride[0] * img.h : 0;
    if (img_size > 0) {
        size_t copy_len = Size < img_size ? Size : img_size;
        memcpy(img.planes[0], Data, copy_len);
        // Fill remaining planes if needed
        if (img.planes[1]) {
            size_t uv_size = img.stride[1] * img.h / 2;
            size_t offset = img_size;
            size_t remaining = (Size > offset) ? (Size - offset) : 0;
            size_t uv_copy = remaining < uv_size ? remaining : uv_size;
            if (uv_copy > 0) {
                memcpy(img.planes[1], Data + offset, uv_copy);
            }
        }
        if (img.planes[2]) {
            size_t uv_size = img.stride[2] * img.h / 2;
            size_t offset = img_size + (img.planes[1] ? img.stride[1] * img.h / 2 : 0);
            size_t remaining = (Size > offset) ? (Size - offset) : 0;
            size_t uv_copy = remaining < uv_size ? remaining : uv_size;
            if (uv_copy > 0) {
                memcpy(img.planes[2], Data + offset, uv_copy);
            }
        }
    }

    // Fuzz encoding parameters
    vpx_codec_pts_t pts = (Size > 13 ? (int64_t)Data[13] : 0);
    unsigned long duration = (Size > 14 ? (unsigned long)Data[14] : 1);
    vpx_enc_frame_flags_t flags = (Size > 15 ? (vpx_enc_frame_flags_t)Data[15] : 0);
    vpx_enc_deadline_t deadline = (Size > 16 ? (vpx_enc_deadline_t)Data[16] : VPX_DL_REALTIME);

    // Fuzz custom buffer
    if (Size > 17 && Data[17] > 0) {
        size_t buf_size = (Size > 18 ? (size_t)Data[18] * 1024 : 4096);
        custom_buf_size = buf_size;
        custom_buf = (uint8_t *)malloc(buf_size);
        if (custom_buf) {
            vpx_fixed_buf_t buf = { .buf = custom_buf, .sz = buf_size };
            unsigned int pad_before = (Size > 19 ? Data[19] : 0);
            unsigned int pad_after = (Size > 20 ? Data[20] : 0);
            vpx_codec_set_cx_data_buf(&codec_ctx, &buf, pad_before, pad_after);
        }
    }

    // Encode frame
    vpx_codec_encode(&codec_ctx, &img, pts, duration, flags, deadline);

    // Retrieve encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        // Process packet (ignore unsupported types)
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame data available
        } else if (pkt->kind == VPX_CODEC_STATS_PKT) {
            // Stats data available
        }
        // Other packet types are ignored as per documentation
    }

    // Flush encoder
    vpx_codec_encode(&codec_ctx, NULL, pts + duration, duration, flags, deadline);
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        // Process flush packets
    }

    // Reconfigure encoder with fuzzed config
    vpx_codec_enc_config_set(&codec_ctx, &cfg);

    cleanup_resources();
    return 0;
}