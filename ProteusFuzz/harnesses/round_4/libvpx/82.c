// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
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
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/*
 * Fuzz driver for libvpx encoder APIs.
 * Targets: vpx_codec_encode, vpx_codec_enc_config_set, vpx_codec_control_
 */

// Define VP9 control IDs if not present in the included headers
#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 1001
#endif
#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 1002
#endif
#ifndef VP9E_SET_TPL
#define VP9E_SET_TPL 1003
#endif

static int init_encoder(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size, vpx_codec_enc_cfg_t *cfg_out) {
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_enc_cfg_t cfg;

    // Select codec based on first byte
    if (data[0] % 2 == 0) {
        iface = vpx_codec_vp8_cx();
    } else {
        // Default to VP8 if VP9 header/symbol is not guaranteed
        iface = vpx_codec_vp8_cx(); 
    }

    if (!iface) return -1;

    if (vpx_codec_enc_init(ctx, iface, NULL, 0) != VPX_CODEC_OK) {
        return -1;
    }

    // Initialize config
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    // Fuzz configuration parameters
    cfg.g_w = (data[1] % 256) + 16;
    cfg.g_h = (data[2] % 256) + 16;
    cfg.g_timebase.num = (data[3] % 100) + 1;
    cfg.g_timebase.den = (data[4] % 100) + 1;
    cfg.rc_target_bitrate = (data[5] % 1000) + 100;
    cfg.g_threads = (data[6] % 4) + 1;
    cfg.g_profile = data[7] % 4;
    cfg.g_bit_depth = (data[8] % 3 == 0) ? VPX_BITS_8 : (data[8] % 2 == 0 ? VPX_BITS_10 : VPX_BITS_12);
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.rc_end_usage = (enum vpx_rc_mode)(data[9] % 4);
    cfg.kf_mode = (enum vpx_kf_mode)(data[10] % 3);
    cfg.kf_min_dist = (data[11] % 100) + 1;
    cfg.kf_max_dist = (data[12] % 100) + 1;

    if (vpx_codec_enc_config_set(ctx, &cfg) != VPX_CODEC_OK) {
        return -1;
    }

    // Copy config to output if needed
    if (cfg_out) {
        memcpy(cfg_out, &cfg, sizeof(vpx_codec_enc_cfg_t));
    }

    return 0;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    vpx_codec_ctx_t ctx;
    vpx_image_t img;
    vpx_codec_cx_pkt_t *pkt;
    vpx_codec_err_t err;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;
    vpx_codec_enc_cfg_t cfg;

    // Initialize encoder
    if (init_encoder(&ctx, Data, Size, &cfg) != 0) {
        return 0;
    }

    // Prepare image using the config we just set
    memset(&img, 0, sizeof(img));
    img.fmt = VPX_IMG_FMT_I420;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = img.w;
    img.d_h = img.h;
    img.bit_depth = 8;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;

    // Allocate image planes
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, img.w, img.h, 1) != VPX_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    size_t plane_size = img.stride[0] * img.h;
    if (Size > plane_size) {
        memcpy(img.planes[0], Data, plane_size);
    } else {
        memset(img.planes[0], Data[0], plane_size);
    }

    // Encode a few frames
    for (int i = 0; i < 3; i++) {
        // Vary flags and deadline based on input
        flags = (Data[16 + i] & 0x0F);
        deadline = (Data[17 + i] % 100000) + 1;

        err = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
        if (err != VPX_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }

        // Retrieve output packets
        while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Packet processed, no action needed
        }

        pts += duration;
    }

    // Test vpx_codec_control_ with various control IDs
    // Using only valid VP8 controls defined in vp8cx.h
    int control_ids[] = {
        VP8E_SET_NOISE_SENSITIVITY,
        VP8E_SET_CPUUSED,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_TOKEN_PARTITIONS,
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_ARNR_MAXFRAMES,
        VP8E_SET_ARNR_STRENGTH,
        VP8E_SET_ARNR_TYPE,
        VP8E_SET_SCALEMODE, // Replaced invalid VP8E_SET_ENABLETSUB
        VP8E_SET_TUNING
    };

    // Add custom IDs for VP9 if we want to test them
    int extra_ids[] = {
        VP9E_SET_TILE_COLUMNS,
        VP9E_SET_SVC,
        VP9E_SET_TPL
    };

    // Process standard VP8 controls
    for (size_t i = 0; i < sizeof(control_ids) / sizeof(control_ids[0]); i++) {
        int ctrl_id = control_ids[i];
        int param = Data[(18 + i) % Size];

        if (ctrl_id == VP8E_SET_CPUUSED) {
            vpx_codec_control_(&ctx, ctrl_id, param % 8);
        } else if (ctrl_id == VP8E_SET_NOISE_SENSITIVITY) {
            vpx_codec_control_(&ctx, ctrl_id, param % 4);
        } else if (ctrl_id == VP8E_SET_ENABLEAUTOALTREF) {
            vpx_codec_control_(&ctx, ctrl_id, param % 2);
        } else if (ctrl_id == VP8E_SET_ARNR_MAXFRAMES) {
            vpx_codec_control_(&ctx, ctrl_id, param % 32);
        } else if (ctrl_id == VP8E_SET_ARNR_STRENGTH) {
            vpx_codec_control_(&ctx, ctrl_id, param % 32);
        } else if (ctrl_id == VP8E_SET_ARNR_TYPE) {
            vpx_codec_control_(&ctx, ctrl_id, param % 3);
        } else if (ctrl_id == VP8E_SET_SCALEMODE) {
            vpx_codec_control_(&ctx, ctrl_id, param % 2);
        } else if (ctrl_id == VP8E_SET_TUNING) {
            vpx_codec_control_(&ctx, ctrl_id, param % 2);
        } else {
            // Generic control
            vpx_codec_control_(&ctx, ctrl_id, param);
        }
    }

    // Process extra IDs (VP9 specific or generic)
    for (size_t i = 0; i < sizeof(extra_ids) / sizeof(extra_ids[0]); i++) {
        int ctrl_id = extra_ids[i];
        int param = Data[(18 + 10 + i) % Size]; // Adjusted offset

        if (ctrl_id == VP9E_SET_TILE_COLUMNS) {
            vpx_codec_control_(&ctx, ctrl_id, param % 8);
        } else if (ctrl_id == VP9E_SET_SVC) {
            vpx_codec_control_(&ctx, ctrl_id, param % 2);
        } else if (ctrl_id == VP9E_SET_TPL) {
            vpx_codec_control_(&ctx, ctrl_id, param % 4);
        } else {
            vpx_codec_control_(&ctx, ctrl_id, param);
        }
    }

    // Flush the encoder
    err = vpx_codec_encode(&ctx, NULL, pts, duration, 0, 0);
    if (err != VPX_CODEC_OK) {
        // Ignore errors
    }

    // Retrieve remaining packets
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        // Packet processed
    }

    // Cleanup
    vpx_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}