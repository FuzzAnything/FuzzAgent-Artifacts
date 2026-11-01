// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

// Dummy callback functions for external partition model with correct signatures
static aom_ext_part_status_t dummy_create_model(void *priv, const aom_ext_part_config_t *cfg, void **model) {
    (void)priv;
    (void)cfg;
    if (model) *model = priv;
    return AOM_EXT_PART_OK;
}

static aom_ext_part_status_t dummy_send_features(void *model, const aom_partition_features_t *features) {
    (void)model;
    (void)features;
    return AOM_EXT_PART_OK;
}

static aom_ext_part_status_t dummy_get_partition_decision(void *model, aom_partition_decision_t *decision) {
    (void)model;
    if (decision) {
        memset(decision, 0, sizeof(aom_partition_decision_t));
    }
    return AOM_EXT_PART_OK;
}

static aom_ext_part_status_t dummy_send_partition_stats(void *model, const aom_partition_stats_t *stats) {
    (void)model;
    (void)stats;
    return AOM_EXT_PART_OK;
}

static aom_ext_part_status_t dummy_delete_model(void *model) {
    (void)model;
    return AOM_EXT_PART_OK;
}

static int create_image_from_fuzz_data(const uint8_t *data, size_t size, aom_image_t *img, int width, int height, int bit_depth) {
    if (size == 0) return 0;

    img->fmt = AOM_IMG_FMT_I420;
    img->w = width;
    img->h = height;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->bit_depth = bit_depth;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_UNSPECIFIED;
    img->tc = AOM_CICP_TC_UNSPECIFIED;
    img->mc = AOM_CICP_MC_UNSPECIFIED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_UNKNOWN;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;
    img->self_allocd = 0;
    img->img_data_owner = 0;

    // Allocate planes
    size_t y_size = (size_t)width * height;
    size_t uv_size = (size_t)width * height / 4;
    
    if (y_size + 2 * uv_size > size) {
        // Not enough data, create a small valid image with dummy data
        img->w = 16;
        img->h = 16;
        y_size = 256;
        uv_size = 64;
    }

    img->planes[0] = malloc(y_size);
    img->planes[1] = malloc(uv_size);
    img->planes[2] = malloc(uv_size);
    img->stride[0] = width;
    img->stride[1] = width / 2;
    img->stride[2] = width / 2;
    img->sz = y_size + 2 * uv_size;
    img->bps = bit_depth;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        return -1;
    }

    // Fill with fuzz data or pattern
    size_t fill_size = (size < y_size + 2 * uv_size) ? size : y_size + 2 * uv_size;
    if (data) {
        memcpy(img->planes[0], data, fill_size);
    } else {
        memset(img->planes[0], 128, fill_size);
    }
    
    // Ensure chroma planes have data
    if (uv_size > 0) {
        memset(img->planes[1], 128, uv_size);
        memset(img->planes[2], 128, uv_size);
    }

    return 0;
}

static void free_image(aom_image_t *img) {
    if (img) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        img->planes[0] = NULL;
        img->planes[1] = NULL;
        img->planes[2] = NULL;
    }
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Parse fuzz data to determine parameters
    int width = 16;
    int height = 16;
    int bit_depth = 8;
    int use_external_partition = 0;
    int use_error_resilient = 0;

    if (Size >= 2) {
        width = Data[0] % 256 + 16;
        height = Data[1] % 256 + 16;
        if (width > 1920 || height > 1080) { width = 64; height = 64; }
    }
    if (Size >= 3) {
        bit_depth = (Data[2] % 4) + 8; // 8, 10, 12
        if (bit_depth > 12) bit_depth = 8;
    }
    if (Size >= 4) {
        use_external_partition = (Data[3] % 2);
    }
    if (Size >= 5) {
        use_error_resilient = (Data[4] % 2);
    }

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Apply optional controls based on fuzz data
    if (use_external_partition) {
        aom_ext_part_funcs_t funcs = {
            .create_model = dummy_create_model,
            .send_features = dummy_send_features,
            .get_partition_decision = dummy_get_partition_decision,
            .send_partition_stats = dummy_send_partition_stats,
            .delete_model = dummy_delete_model,
            .decision_mode = AOM_EXT_PART_WHOLE_TREE,
            .priv = (void *)Data
        };
        // Use the correct constant name as per the error message
        aom_codec_control(&ctx, AV1E_SET_EXTERNAL_PARTITION, &funcs);
    }

    if (use_error_resilient) {
        int mode = 1;
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, &mode);
    }

    // Prepare image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    if (create_image_from_fuzz_data(Data, Size, &img, width, height, bit_depth) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    // Encode loop
    int frame_count = 0;
    int max_frames = 10;
    
    while (frame_count < max_frames) {
        const aom_image_t *img_ptr = (frame_count < 5) ? &img : NULL;
        
        // Vary flags - use AOM_EFLAG_FORCE_KF if available, otherwise 0
        // The error indicated AOM_EFLAG_FORCE_KEY was missing. 
        // In libaom, the flag is typically AOM_EFLAG_FORCE_KF.
        if (frame_count == 2) flags = AOM_EFLAG_FORCE_KF;
        else flags = 0;

        aom_codec_err_t res = aom_codec_encode(&ctx, img_ptr, pts, duration, flags);
        
        if (res != AOM_CODEC_OK) {
            // Handle error, but continue to flush
            if (res == AOM_CODEC_INVALID_PARAM) {
                // Invalid params might happen with bad fuzz data, break or continue
            }
        }

        // Collect output data
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Valid frame data
                (void)pkt->data.frame.buf;
                (void)pkt->data.frame.sz;
            } else if (pkt->kind == AOM_CODEC_STATS_PKT) {
                // Stats
            } else if (pkt->kind == AOM_CODEC_PSNR_PKT) {
                // PSNR
            }
        }

        if (img_ptr == NULL) break; // Flush done
        
        pts += duration;
        frame_count++;
    }

    // Flush remaining
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    aom_codec_iter_t iter = NULL;
    while (aom_codec_get_cx_data(&ctx, &iter) != NULL) {
        // Drain
    }

    // Cleanup
    free_image(&img);
    aom_codec_destroy(&ctx);

    return 0;
}