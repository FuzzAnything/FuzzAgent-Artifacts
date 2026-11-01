// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/* 
 * Fuzz driver for libaom targeting:
 * - aom_codec_get_cx_data
 * - aom_codec_encode
 * Note: Functions with empty signatures in the prompt are skipped.
 * Decoder functions like aom_codec_get_frame are skipped as the context is an encoder.
 */

static aom_codec_ctx_t *g_ctx = NULL;
static aom_image_t g_img;

static void cleanup(void) {
    if (g_ctx) {
        aom_codec_destroy(g_ctx);
        g_ctx = NULL;
    }
    /* Free the image if it was allocated */
    if (g_img.img_data) {
        aom_img_free(&g_img);
    }
}

static int init_encoder(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    unsigned int width, height;

    if (!g_ctx) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ret = aom_codec_enc_config_default(iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        /* Use fuzz input to determine dimensions, clamped to safe values */
        if (size >= 4) {
            width = (data[0] & 0x3F) + 16;
            height = (data[1] & 0x3F) + 16;
        } else {
            width = 64;
            height = 64;
        }

        /* Ensure dimensions are multiples of 8 (common requirement) */
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;

        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 500;
        /* Use 0 for error resilience as AOM_ERRRESIL_NONE is not defined in all versions */
        cfg.g_error_resilient = 0; 
        cfg.g_lag_in_frames = 0;
        cfg.g_threads = 1;

        ret = aom_codec_enc_init(&g_ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            g_ctx = NULL;
            return 0;
        }

        /* Allocate image buffer */
        g_img.w = width;
        g_img.h = height;
        g_img.d_w = width;
        g_img.d_h = height;
        g_img.r_w = width;
        g_img.r_h = height;
        g_img.bit_depth = 8;
        g_img.fmt = AOM_IMG_FMT_I420;
        g_img.planes[0] = NULL;
        g_img.planes[1] = NULL;
        g_img.planes[2] = NULL;
        g_img.stride[0] = 0;
        g_img.stride[1] = 0;
        g_img.stride[2] = 0;
        g_img.sz = 0;
        g_img.bps = 0;
        g_img.temporal_id = 0;
        g_img.spatial_id = 0;
        g_img.user_priv = NULL;
        g_img.img_data = NULL;
        g_img.img_data_owner = 0;
        g_img.self_allocd = 0;
        g_img.metadata = NULL;
        g_img.fb_priv = NULL;
        g_img.cp = AOM_CICP_CP_BT_709;
        g_img.tc = AOM_CICP_TC_BT_709;
        g_img.mc = AOM_CICP_MC_BT_709;
        g_img.monochrome = 0;
        g_img.csp = AOM_CSP_UNKNOWN;
        g_img.range = AOM_CR_STUDIO_RANGE;
        g_img.x_chroma_shift = 1;
        g_img.y_chroma_shift = 1;

        if (!aom_img_alloc(&g_img, AOM_IMG_FMT_I420, width, height, 1)) {
            cleanup();
            return 0;
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize encoder if not already done */
    if (!g_ctx) {
        if (!init_encoder(Data, Size)) {
            return 0;
        }
    }

    /* Prepare image data from fuzz input */
    if (g_img.planes[0]) {
        size_t plane_size = (size_t)g_img.stride[0] * g_img.h;
        size_t u_plane_size = (size_t)g_img.stride[1] * (g_img.h / 2);
        size_t v_plane_size = (size_t)g_img.stride[2] * (g_img.h / 2);
        size_t total_needed = plane_size + u_plane_size + v_plane_size;
        
        size_t copy_size = (Size < total_needed) ? Size : total_needed;
        
        /* Fill Y plane */
        size_t y_size = (size_t)g_img.stride[0] * g_img.h;
        if (copy_size > 0) {
            memcpy(g_img.planes[0], Data, (Size < y_size) ? Size : y_size);
        }
        /* Fill U and V planes with derived data or zero */
        if (g_img.planes[1]) {
            memset(g_img.planes[1], 128, u_plane_size);
        }
        if (g_img.planes[2]) {
            memset(g_img.planes[2], 128, v_plane_size);
        }
    }

    /* Fuzz aom_codec_encode */
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    if (Size > 0) {
        pts = (int64_t)Data[0];
        if (Size > 1) {
            duration = (unsigned long)Data[1];
            if (duration == 0) duration = 1;
        }
        if (Size > 2) {
            flags = (aom_enc_frame_flags_t)Data[2];
        }
    }

    aom_codec_err_t enc_ret = aom_codec_encode(g_ctx, &g_img, pts, duration, flags);
    (void)enc_ret; /* Ignore return for fuzzing, but could check for errors */

    /* Fuzz aom_codec_get_cx_data */
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(g_ctx, &iter)) != NULL) {
        /* Just iterate to exhaust the buffer, no deep inspection needed for crash testing */
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            /* Access frame data to ensure validity */
            if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                /* Do nothing, just ensure pointers are valid */
            }
        }
    }

    /* Flush encoder */
    aom_codec_encode(g_ctx, NULL, 0, 0, 0);
    iter = NULL;
    while ((pkt = aom_codec_get_cx_data(g_ctx, &iter)) != NULL) {
        /* Drain remaining packets */
    }

    return 0;
}