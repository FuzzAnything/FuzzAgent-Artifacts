// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
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

// Helper to safely allocate and initialize an image
static aom_image_t* create_fuzz_image(const uint8_t *data, size_t size, aom_img_fmt_t fmt) {
    if (!data || size < 16) return NULL;
    
    aom_image_t *img = (aom_image_t*)malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    
    memset(img, 0, sizeof(aom_image_t));
    
    // Parse width/height from first 8 bytes if possible, else default
    if (size >= 8) {
        uint32_t w = *(uint32_t*)data;
        uint32_t h = *(uint32_t*)(data + 4);
        if (w == 0 || h == 0 || w > 4096 || h > 4096) {
            w = 32; h = 32;
        }
        img->w = img->d_w = img->r_w = w;
        img->h = img->d_h = img->r_h = h;
    } else {
        img->w = img->d_w = img->r_w = 32;
        img->h = img->d_h = img->r_h = 32;
    }

    img->fmt = fmt;
    img->bit_depth = 8;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_COLOCATED;
    img->monochrome = 0;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 8;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;

    // Allocate planes based on format
    size_t y_size = img->w * img->h;
    size_t uv_size = (img->w / 2) * (img->h / 2);
    
    if (fmt == AOM_IMG_FMT_I420 || fmt == AOM_IMG_FMT_YV12) {
        img->planes[0] = (unsigned char*)malloc(y_size);
        img->stride[0] = img->w;
        img->planes[1] = (unsigned char*)malloc(uv_size);
        img->stride[1] = img->w / 2;
        img->planes[2] = (unsigned char*)malloc(uv_size);
        img->stride[2] = img->w / 2;
    } else if (fmt == AOM_IMG_FMT_NV12) {
        img->planes[0] = (unsigned char*)malloc(y_size);
        img->stride[0] = img->w;
        img->planes[1] = (unsigned char*)malloc(uv_size * 2);
        img->stride[1] = img->w / 2;
        img->planes[2] = NULL;
        img->stride[2] = 0;
    } else {
        // Fallback for other planar formats
        img->planes[0] = (unsigned char*)malloc(y_size);
        img->stride[0] = img->w;
        img->planes[1] = (unsigned char*)malloc(uv_size);
        img->stride[1] = img->w / 2;
        img->planes[2] = (unsigned char*)malloc(uv_size);
        img->stride[2] = img->w / 2;
    }

    if (!img->planes[0] || (fmt != AOM_IMG_FMT_NV12 && !img->planes[1])) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    // Fill with data
    size_t total_needed = y_size + (fmt == AOM_IMG_FMT_NV12 ? uv_size * 2 : uv_size * 2);
    size_t copy_size = (size > total_needed) ? total_needed : size;
    memcpy(img->planes[0], data, copy_size);
    
    // Fill remaining planes with zeros or pattern if data runs out
    if (img->planes[1]) memset(img->planes[1], 0x80, uv_size);
    if (img->planes[2]) memset(img->planes[2], 0x80, uv_size);

    img->sz = y_size + uv_size * 2;

    return img;
}

static void free_image(aom_image_t *img) {
    if (!img) return;
    if (img->planes[0]) free(img->planes[0]);
    if (img->planes[1]) free(img->planes[1]);
    if (img->planes[2]) free(img->planes[2]);
    free(img);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Fuzz config parameters safely
    uint32_t w = 32;
    uint32_t h = 32;
    
    if (Size >= 8) {
        uint32_t raw_w = *(uint32_t*)Data;
        uint32_t raw_h = *(uint32_t*)(Data + 4);
        if (raw_w > 0 && raw_w <= 4096) w = raw_w;
        if (raw_h > 0 && raw_h <= 4096) h = raw_h;
    }
    
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        // Test error handling on init failure
        const char *err_str = aom_codec_error(&ctx);
        (void)err_str;
        const char *err_code_str = aom_codec_err_to_string(ret);
        (void)err_code_str;
        return 0;
    }

    // 1. Test aom_codec_set_option with fuzzed keys/values
    // Extract key and value from input safely
    size_t key_len = 0;
    size_t val_len = 0;
    
    if (Size > 10) key_len = Data[10] % 20;
    if (Size > 11) val_len = Data[11] % 20;
    
    char key[25] = {0};
    char val[25] = {0};
    
    // Ensure we don't read past the buffer
    size_t offset = 12;
    if (offset + key_len <= Size) {
        memcpy(key, Data + offset, key_len);
        key[key_len] = '\0';
    }
    
    offset += key_len;
    if (offset + val_len <= Size) {
        memcpy(val, Data + offset, val_len);
        val[val_len] = '\0';
    }

    ret = aom_codec_set_option(&ctx, key, val);
    (void)ret; // Ignore return, just exercise the path

    // 2. Test aom_codec_control
    // Try various control IDs, including invalid ones
    int ctrl_id = 100;
    if (Size > 20) {
        ctrl_id = 100 + (int)(Data[20] % 100);
    }
    
    // Try to pass an integer argument
    int arg_int = 0;
    if (Size > 22) arg_int = (int)Data[22];
    
    ret = aom_codec_control(&ctx, ctrl_id, arg_int);
    (void)ret;

    // 3. Test aom_codec_encode
    // Create an image
    aom_image_t *img = create_fuzz_image(Data, Size, AOM_IMG_FMT_I420);
    
    if (img) {
        aom_codec_pts_t pts = 1000;
        unsigned long duration = 100;
        
        if (Size > 30) pts = (aom_codec_pts_t)(Data[30] * 1000);
        if (Size > 31) {
            duration = (unsigned long)Data[31];
            if (duration == 0) duration = 100;
        }
        
        aom_enc_frame_flags_t flags = 0;
        if (Size > 32) flags = (aom_enc_frame_flags_t)Data[32];
        
        ret = aom_codec_encode(&ctx, img, pts, duration, flags);
        (void)ret;
        
        // Flush the encoder
        ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        (void)ret;
        
        free_image(img);
    } else {
        // Test encoding with NULL image (flush) even if image creation failed
        ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        (void)ret;
    }

    // 4. Test aom_codec_error and aom_codec_err_to_string
    const char *err_msg = aom_codec_error(&ctx);
    (void)err_msg;
    
    // Test with a specific error code
    const char *err_str = aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    (void)err_str;
    err_str = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    (void)err_str;
    err_str = aom_codec_err_to_string(9999); // Unrecognized
    (void)err_str;

    // 5. Test with null context for error functions
    const char *null_err = aom_codec_error(NULL);
    (void)null_err;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}