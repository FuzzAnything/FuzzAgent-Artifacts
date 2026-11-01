// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int try_encode_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size, int frame_idx) {
    if (!ctx || !data || size == 0) return 0;

    aom_image_t img;
    memset(&img, 0, sizeof(img));

    // Parse dimensions from fuzz input if available, otherwise use defaults
    unsigned int width = 16;
    unsigned int height = 16;
    unsigned int bit_depth = 8;

    if (size >= 4) {
        width = (data[0] & 0x3F) + 16; // Limit width to avoid huge allocations
        height = (data[1] & 0x3F) + 16;
        bit_depth = (data[2] & 0x03) + 8; // 8, 10, 12
        if (width > 1024) width = 1024;
        if (height > 1024) height = 1024;
        if (bit_depth > 12) bit_depth = 12;
    }

    img.fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) img.fmt = AOM_IMG_FMT_I42016;

    img.w = width;
    img.h = height;
    img.bit_depth = bit_depth;
    img.d_w = width;
    img.d_h = height;
    img.r_w = width;
    img.r_h = height;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.range = AOM_CR_STUDIO_RANGE;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.csp = AOM_CSP_COLOCATED;
    img.monochrome = 0;
    img.temporal_id = 0;
    img.spatial_id = 0;

    if (aom_img_alloc(&img, img.fmt, img.w, img.h, 1) != 0) {
        return 0;
    }

    // Fill image planes with fuzz data
    size_t plane_size = (size_t)img.stride[0] * img.h;
    size_t total_data_needed = plane_size * 3; // Y, U, V
    size_t available = size - (size > 4 ? 4 : 0);

    if (available > 0) {
        const uint8_t *src = data + (size > 4 ? 4 : 0);
        for (int p = 0; p < 3; p++) {
            size_t plane_stride = img.stride[p];
            size_t plane_h = img.h >> img.y_chroma_shift;
            size_t plane_w = img.w >> img.x_chroma_shift;
            size_t plane_bytes = (size_t)plane_stride * plane_h;
            
            for (unsigned int y = 0; y < plane_h; y++) {
                uint8_t *dst = img.planes[p] + y * plane_stride;
                size_t copy_len = plane_w;
                if (copy_len > available) copy_len = available;
                if (copy_len > 0) {
                    memcpy(dst, src, copy_len);
                    src += copy_len;
                    available -= copy_len;
                }
                // Fill rest with 0 if data runs out
                if (copy_len < plane_w) {
                    memset(dst + copy_len, 0, plane_w - copy_len);
                }
            }
        }
    }

    aom_codec_pts_t pts = (aom_codec_pts_t)frame_idx;
    unsigned long duration = 1;
    if (size > 8) duration = (unsigned long)data[4] + 1;
    if (duration > UINT32_MAX) duration = UINT32_MAX;

    aom_enc_frame_flags_t flags = 0;
    if (size > 9) flags = (aom_enc_frame_flags_t)data[5];

    aom_codec_err_t ret = aom_codec_encode(ctx, &img, pts, duration, flags);
    
    // Drain output packets to prevent memory leaks in context
    aom_codec_cx_pkt_t pkt;
    while (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // We just ensure we consume the packet.
        }
    }

    aom_img_free(&img);
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Configure based on input
    cfg.g_w = (Data[0] & 0x3F) + 16;
    cfg.g_h = (Data[1] & 0x3F) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = (Data[2] & 1);
    cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_set_option
    if (Size > 12) {
        // Extract key and value from data
        size_t key_len = Data[10] % 20;
        size_t val_len = (Data[11] & 0x0F) % 20;
        if (Size >= 12 + key_len + val_len) {
            char key[21] = {0};
            char val[21] = {0};
            memcpy(key, Data + 12, key_len);
            memcpy(val, Data + 12 + key_len, val_len);
            aom_codec_set_option(&ctx, key, val);
        }
    }

    // Fuzz aom_codec_encode
    int frames = (Size > 14) ? (Data[14] % 5) + 1 : 3;
    for (int i = 0; i < frames; i++) {
        try_encode_frame(&ctx, Data, Size, i);
    }

    // Flush the encoder
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    aom_codec_cx_pkt_t pkt;
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        // Skip unref as it caused compilation error in this environment
    }

    aom_codec_destroy(&ctx);
    return 0;
}