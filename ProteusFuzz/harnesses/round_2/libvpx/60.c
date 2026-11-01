// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP9E_SET_TPL__value at vp8cx.h:1087:1 in vp8cx.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
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

/*
 * Fuzz driver for libvpx encoder APIs.
 * Targets:
 * - vpx_codec_enc_config_set
 * - vpx_codec_encode
 * - vpx_codec_control_ (via vpx_codec_control macro for VP9E_SET_SVC, VP9E_SET_TILE_COLUMNS, VP9E_SET_TPL)
 */

/* Define VP9 control IDs if not present in included headers */
#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 0
#endif
#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 1
#endif
#ifndef VP9E_SET_TPL
#define VP9E_SET_TPL 2
#endif

static int fuzz_encode(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    /* Use vpx_codec_vp9_cx as the correct interface function for VP9 encoder */
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (!iface) {
        return 0;
    }

    // Initialize configuration with defaults
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters safely
    // Ensure we have enough data before accessing indices
    if (Size >= 2) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1] % 1024) + 16;
    }
    if (Size > 2) cfg.g_threads = (Data[2] % 8) + 1;
    if (Size > 3) cfg.g_profile = Data[3] % 4;
    if (Size > 4) cfg.g_bit_depth = (Data[4] % 3) == 0 ? VPX_BITS_8 : (Data[4] % 2 == 0 ? VPX_BITS_10 : VPX_BITS_12);
    if (Size > 5) cfg.g_input_bit_depth = cfg.g_bit_depth;
    
    // Timebase
    if (Size > 6) {
        cfg.g_timebase.num = (Data[6] % 100) + 1;
    } else {
        cfg.g_timebase.num = 1;
    }
    if (Size > 7) {
        cfg.g_timebase.den = (Data[7] % 100) + 1;
    } else {
        cfg.g_timebase.den = 1;
    }

    // Rate control
    if (Size > 8) cfg.rc_target_bitrate = (Data[8] % 1000) + 1;
    if (Size > 9) cfg.rc_min_quantizer = Data[9] % 64;
    if (Size > 10) cfg.rc_max_quantizer = (Data[10] % 64) + 1;
    if (Size > 11) cfg.rc_end_usage = Data[11] % 4;
    if (Size > 12) cfg.kf_mode = Data[12] % 3;
    if (Size > 13) cfg.kf_min_dist = (Data[13] % 100) + 1;
    if (Size > 14) cfg.kf_max_dist = (Data[14] % 100) + 1;

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Create a dummy image
    vpx_image_t img;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size > 15) fmt = (Data[15] % 4 == 0) ? VPX_IMG_FMT_I420 : (Data[15] % 4 == 1 ? VPX_IMG_FMT_YV12 : (Data[15] % 4 == 2 ? VPX_IMG_FMT_NV12 : VPX_IMG_FMT_I420));

    // vpx_img_alloc returns 0 on success, non-zero on error
    if (vpx_img_alloc(&img, fmt, cfg.g_w, cfg.g_h, 16) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data
    size_t total_size = 0;
    for (int i = 0; i < 3; i++) {
        if (img.planes[i]) {
            int stride = img.stride[i];
            int h = (i == 0) ? cfg.g_h : (cfg.g_h + 1) / 2;
            total_size += stride * h;
        }
    }

    if (Size > total_size) Size = total_size;
    
    size_t offset = 0;
    for (int i = 0; i < 3; i++) {
        if (img.planes[i]) {
            int stride = img.stride[i];
            int h = (i == 0) ? cfg.g_h : (cfg.g_h + 1) / 2;
            size_t plane_size = stride * h;
            if (offset + plane_size <= Size) {
                memcpy(img.planes[i], Data + offset, plane_size);
            } else {
                memset(img.planes[i], 0, plane_size);
            }
            offset += plane_size;
        }
    }

    // Fuzz control commands (VP9E_SET_SVC, VP9E_SET_TILE_COLUMNS, VP9E_SET_TPL)
    if (Size > 17) {
        int ctrl_id = Data[16] % 3;
        int val = 0;
        if (ctrl_id == 0) {
            // VP9E_SET_SVC (simulated as int value)
            val = Data[17] % 4;
            vpx_codec_control(&ctx, VP9E_SET_SVC, val);
        } else if (ctrl_id == 1) {
            // VP9E_SET_TILE_COLUMNS
            val = (Data[17] % 8) + 1;
            vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, val);
        } else if (ctrl_id == 2) {
            // VP9E_SET_TPL
            val = Data[17] % 4;
            vpx_codec_control(&ctx, VP9E_SET_TPL, val);
        }
    }

    // Reconfigure with fuzzed config
    if (Size > 18) {
        vpx_codec_enc_config_set(&ctx, &cfg);
    }

    // Encode frames
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;

    // Encode a few frames based on data length
    int num_frames = (Size > 20) ? (Data[20] % 10) + 1 : 3;
    
    for (int i = 0; i < num_frames; i++) {
        if (Size > 21 + i) {
            flags = Data[21 + i] & 0xFF;
        }
        
        res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
        if (res != VPX_CODEC_OK) {
            break;
        }

        // Get encoded data
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Process packet if needed
        }

        pts += duration;
    }

    // Flush encoder
    res = vpx_codec_encode(&ctx, NULL, pts, duration, 0, deadline);
    if (res == VPX_CODEC_OK) {
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Process packet
        }
    }

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_encode(Data, Size);
    return 0;
}