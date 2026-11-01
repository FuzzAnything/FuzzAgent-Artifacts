// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

// Helper to safely initialize a minimal image structure for fuzzing
static int prepare_image_from_fuzz(aom_image_t *img, const uint8_t *data, size_t size) {
    if (size < 8) return 0;

    // Use first 8 bytes to determine width and height, ensuring they are non-zero and reasonable
    uint32_t w = *(uint32_t *)(data);
    uint32_t h = *(uint32_t *)(data + 4);

    // Clamp dimensions to avoid huge allocations
    if (w == 0 || h == 0 || w > 4096 || h > 4096) {
        w = 16;
        h = 16;
    }

    img->fmt = AOM_IMG_FMT_I420;
    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->csp = AOM_CSP_COLOCATED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->bps = 8;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;

    // Allocate planes
    int stride_y = w;
    int stride_uv = (w + 1) / 2;

    size_t sz_y = (size_t)stride_y * h;
    size_t sz_uv = (size_t)stride_uv * (h / 2);

    img->planes[0] = malloc(sz_y);
    img->planes[1] = malloc(sz_uv);
    img->planes[2] = malloc(sz_uv);
    img->stride[0] = stride_y;
    img->stride[1] = stride_uv;
    img->stride[2] = stride_uv;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        img->planes[0] = NULL;
        img->planes[1] = NULL;
        img->planes[2] = NULL;
        return 0;
    }

    // Fill with data or zeros
    if (size > 8) {
        size_t data_len = size - 8;
        size_t total_needed = sz_y + sz_uv + sz_uv;
        size_t copy_len = (data_len < total_needed) ? data_len : total_needed;
        memcpy(img->planes[0], data + 8, copy_len);
        // Fill rest with zeros if needed
        if (copy_len < total_needed) {
            memset(img->planes[0] + copy_len, 0, total_needed - copy_len);
        }
    } else {
        memset(img->planes[0], 0, sz_y);
        memset(img->planes[1], 0, sz_uv);
        memset(img->planes[2], 0, sz_uv);
    }

    img->sz = sz_y + sz_uv + sz_uv;
    return 1;
}

static void cleanup_image(aom_image_t *img) {
    if (img) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        img->planes[0] = NULL;
        img->planes[1] = NULL;
        img->planes[2] = NULL;
        // Note: img_data is usually managed internally, but if self_allocd is 0, we don't free it here
    }
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // 1. Initialize Encoder Context
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (Size > 0) cfg.g_w = (Size % 4096) + 16;
    if (Size > 1) cfg.g_h = ((Size >> 8) % 4096) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = (Size > 4) ? (Data[4] % 16) + 1 : 1;
    // Removed cfg.g_row_mt as it does not exist in the struct

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Try to get error info even on init failure
        const char *err_str = aom_codec_error(&ctx);
        (void)err_str;
        const char *err_detail = aom_codec_error_detail(&ctx);
        (void)err_detail;
        return 0;
    }

    // 2. Fuzz aom_codec_set_option
    // We construct a key/value pair from the fuzz data
    if (Size > 10) {
        // Create a safe string for key and value
        char key[64] = {0};
        char val[64] = {0};
        size_t key_len = (Size > 20) ? (Data[10] % 32) : 4;
        size_t val_len = (Size > 20) ? (Data[11] % 32) : 4;
        
        if (key_len > 0 && key_len < sizeof(key)) {
            memcpy(key, Data + 12, key_len);
            key[key_len] = '\0';
        }
        if (val_len > 0 && val_len < sizeof(val)) {
            memcpy(val, Data + 12 + key_len, val_len);
            val[val_len] = '\0';
        }
        
        // Try setting the option
        aom_codec_set_option(&ctx, key, val);
        
        // Check error
        if (ctx.err != AOM_CODEC_OK) {
            const char *e_str = aom_codec_err_to_string(ctx.err);
            (void)e_str;
            const char *e_detail = aom_codec_error_detail(&ctx);
            (void)e_detail;
        }
    }

    // 3. Fuzz aom_codec_encode
    // Prepare an image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    if (!prepare_image_from_fuzz(&img, Data, Size)) {
        // If image prep failed, try encoding with NULL to flush
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz PTS and Duration
    aom_codec_pts_t pts = 0;
    if (Size > 20) {
        pts = ((int64_t)Data[20] << 24) | ((int64_t)Data[21] << 16) | ((int64_t)Data[22] << 8) | Data[23];
    }
    
    unsigned long duration = 1;
    if (Size > 24) {
        duration = ((unsigned long)Data[24] << 24) | ((unsigned long)Data[25] << 16) | ((unsigned long)Data[26] << 8) | Data[27];
        if (duration == 0) duration = 1;
        if (duration > UINT32_MAX) duration = UINT32_MAX;
    }

    aom_enc_frame_flags_t flags = 0;
    if (Size > 28) flags = Data[28];

    // Encode frame
    err = aom_codec_encode(&ctx, &img, pts, duration, flags);
    if (err != AOM_CODEC_OK) {
        const char *e_str = aom_codec_err_to_string(err);
        (void)e_str;
        const char *e_detail = aom_codec_error_detail(&ctx);
        (void)e_detail;
    }

    // Flush the encoder
    // Note: aom_codec_get_cx_data returns a pointer to internal data. 
    // We do not need to explicitly pop it in libaom's standard API usage for simple fuzzing,
    // as the next call to get_cx_data or encode will handle state. 
    // However, to be safe and avoid memory leaks in some internal implementations if they rely on it,
    // we just loop until no data is returned.
    while (1) {
        aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(&ctx, NULL);
        if (!pkt) break;
        // In standard libaom, you don't call aom_codec_pop_cx_data. 
        // The data is valid until the next call. We just continue to flush.
        // If we were storing data, we would process it here.
    }

    // 4. Test error retrieval functions with valid and invalid contexts
    const char *err_msg = aom_codec_error(&ctx);
    (void)err_msg;
    
    const char *err_detail_msg = aom_codec_error_detail(&ctx);
    (void)err_detail_msg;

    // Test with NULL context
    const char *null_err = aom_codec_error(NULL);
    (void)null_err;
    
    const char *null_detail = aom_codec_error_detail(NULL);
    (void)null_detail;

    // Test error string conversion
    const char *str_ok = aom_codec_err_to_string(AOM_CODEC_OK);
    (void)str_ok;
    const char *str_err = aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    (void)str_err;
    const char *str_mem = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    (void)str_mem;

    // Cleanup
    cleanup_image(&img);
    aom_codec_destroy(&ctx);

    return 0;
}