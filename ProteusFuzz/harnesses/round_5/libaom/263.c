// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIRECTIONAL_INTRA at aomcx.h:2282:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_AUTO_INTRA_TOOLS_OFF at aomcx.h:2303:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DV_COST_UPD_FREQ at aomcx.h:2273:1 in aomcx.h
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

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static aom_codec_ctx_t *g_ctx = NULL;
static aom_image_t g_img;
static int g_initialized = 0;
static aom_codec_pts_t g_pts = 0;

static void cleanup_encoder(void) {
    if (g_ctx != NULL) {
        aom_codec_destroy(g_ctx);
        g_ctx = NULL;
    }
    if (g_img.planes[0] != NULL) {
        aom_img_free(&g_img);
        memset(&g_img, 0, sizeof(g_img));
    }
    g_initialized = 0;
    g_pts = 0;
}

static int init_encoder(const uint8_t *data, size_t size) {
    if (g_initialized) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = (data[1] % 512) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 1000) + 1;
        cfg.rc_target_bitrate = (data[4] % 10000) + 1000;
        cfg.g_bit_depth = (data[5] % 5) + 8;
        cfg.g_profile = (data[6] % 4);
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_bit_depth = 8;
        cfg.g_profile = 0;
    }

    cfg.g_w = (cfg.g_w + 15) & ~15;
    cfg.g_h = (cfg.g_h + 15) & ~15;

    if (aom_codec_enc_init(&g_ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (cfg.g_bit_depth > 8) {
        fmt = AOM_IMG_FMT_I42016;
    }

    g_img.fmt = fmt;
    g_img.w = cfg.g_w;
    g_img.h = cfg.g_h;
    g_img.bit_depth = cfg.g_bit_depth;
    g_img.d_w = g_img.w;
    g_img.d_h = g_img.h;
    g_img.r_w = g_img.w;
    g_img.r_h = g_img.h;
    g_img.x_chroma_shift = 1;
    g_img.y_chroma_shift = 1;
    g_img.planes[0] = NULL;
    g_img.planes[1] = NULL;
    g_img.planes[2] = NULL;
    g_img.stride[0] = 0;
    g_img.stride[1] = 0;
    g_img.stride[2] = 0;
    g_img.sz = 0;
    g_img.bps = 8;
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

    if (aom_img_alloc(&g_img, fmt, g_img.w, g_img.h, 1) != 0) {
        cleanup_encoder();
        return -1;
    }

    g_initialized = 1;
    return 0;
}

static void fill_image_with_data(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img || !img->planes[0]) return;

    size_t plane_size = (size_t)img->stride[0] * img->h;
    size_t total_size = plane_size;
    if (img->planes[1]) total_size += (size_t)img->stride[1] * img->h;
    if (img->planes[2]) total_size += (size_t)img->stride[2] * img->h;

    if (size == 0) {
        memset(img->planes[0], 128, plane_size);
        if (img->planes[1]) memset(img->planes[1], 128, (size_t)img->stride[1] * img->h);
        if (img->planes[2]) memset(img->planes[2], 128, (size_t)img->stride[2] * img->h);
        return;
    }

    size_t offset = 0;
    for (int i = 0; i < img->h; i++) {
        size_t row_len = img->stride[0];
        size_t copy_len = (size < offset + row_len) ? size - offset : row_len;
        memcpy(img->planes[0] + i * img->stride[0], data + offset, copy_len);
        if (copy_len < row_len) {
            memset(img->planes[0] + i * img->stride[0] + copy_len, 0, row_len - copy_len);
        }
        offset += row_len;
    }

    if (img->planes[1]) {
        for (int i = 0; i < img->h >> img->y_chroma_shift; i++) {
            size_t row_len = img->stride[1];
            size_t copy_len = (size < offset + row_len) ? size - offset : row_len;
            memcpy(img->planes[1] + i * img->stride[1], data + offset, copy_len);
            if (copy_len < row_len) {
                memset(img->planes[1] + i * img->stride[1] + copy_len, 0, row_len - copy_len);
            }
            offset += row_len;
        }
    }

    if (img->planes[2]) {
        for (int i = 0; i < img->h >> img->y_chroma_shift; i++) {
            size_t row_len = img->stride[2];
            size_t copy_len = (size < offset + row_len) ? size - offset : row_len;
            memcpy(img->planes[2] + i * img->stride[2], data + offset, copy_len);
            if (copy_len < row_len) {
                memset(img->planes[2] + i * img->stride[2] + copy_len, 0, row_len - copy_len);
            }
            offset += row_len;
        }
    }
}

int LLVMFuzzerTestOneInput_263(const uint8_t *Data, size_t Size) {
    if (!g_initialized || g_ctx == NULL) {
        if (init_encoder(Data, Size) != 0) {
            return 0;
        }
    }

    int do_flush = 0;
    if (Size > 0) {
        do_flush = Data[0] & 1;
    }

    if (!do_flush) {
        fill_image_with_data(&g_img, Data, Size);

        unsigned long duration = 1;
        if (Size > 1) {
            duration = (Data[1] % 100) + 1;
        }
        if (duration > UINT32_MAX) duration = UINT32_MAX;

        aom_codec_err_t res = aom_codec_encode(g_ctx, &g_img, g_pts, duration, 0);
        
        if (res == AOM_CODEC_OK) {
            g_pts += duration;
        }
    } else {
        aom_codec_encode(g_ctx, NULL, 0, 0, 0);
    }

    if (Size > 2) {
        int val = Data[2] & 1;
        aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(g_ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    if (Size > 4) {
        int val = Data[4] & 1;
        aom_codec_control_typechecked_AV1E_SET_ENABLE_DIRECTIONAL_INTRA(g_ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
    }

    if (Size > 5) {
        int val = Data[5] & 1;
        aom_codec_control_typechecked_AV1E_SET_AUTO_INTRA_TOOLS_OFF(g_ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, val);
    }

    if (Size > 6) {
        int val = Data[6] % 10;
        aom_codec_control_typechecked_AV1E_SET_DV_COST_UPD_FREQ(g_ctx, AV1E_SET_DV_COST_UPD_FREQ, val);
    }

    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(g_ctx, &iter)) != NULL) {
        // No need to manually destroy packets; they are managed by the context
        // or freed when the context is destroyed.
    }

    return 0;
}