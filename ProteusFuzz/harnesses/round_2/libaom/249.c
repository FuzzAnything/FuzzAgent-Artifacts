// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img && img->planes[0]) {
        aom_img_free(img);
    }
}

static aom_image_t *create_fuzz_image(const uint8_t *data, size_t size) {
    if (size < 8) return NULL;
    
    // Extract width and height from the first 8 bytes
    uint32_t w = *(uint32_t *)(data);
    uint32_t h = *(uint32_t *)(data + 4);
    
    // Sanity checks to prevent huge allocations
    if (w == 0 || h == 0 || w > 4096 || h > 4096) {
        return NULL;
    }
    
    // Limit size to avoid OOM
    if (size < (w * h * 3 / 2)) {
        return NULL;
    }
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;
    
    // Fill with fuzz data
    size_t y_size = w * h;
    size_t uv_size = (w * h) / 4;
    
    if (size >= y_size + uv_size * 2) {
        memcpy(img->planes[AOM_PLANE_Y], data + 8, y_size);
        memcpy(img->planes[AOM_PLANE_U], data + 8 + y_size, uv_size);
        memcpy(img->planes[AOM_PLANE_V], data + 8 + y_size + uv_size, uv_size);
    } else {
        // Fill with pattern if data is insufficient
        memset(img->planes[AOM_PLANE_Y], 0, y_size);
        memset(img->planes[AOM_PLANE_U], 0, uv_size);
        memset(img->planes[AOM_PLANE_V], 0, uv_size);
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_249(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Fuzz configuration parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Fuzz aom_codec_set_option
    // Create a null-terminated string from fuzz data for option name and value
    char option_name[64];
    char option_value[64];
    size_t name_len = Size > 60 ? 60 : Size;
    size_t val_len = Size > 60 ? 60 : Size;
    
    if (name_len > 0) {
        memcpy(option_name, Data, name_len);
        option_name[name_len] = '\0';
    } else {
        strcpy(option_name, "dummy");
    }
    
    if (val_len > 0) {
        memcpy(option_value, Data, val_len);
        option_value[val_len] = '\0';
    } else {
        strcpy(option_value, "1");
    }
    
    // Try to set various options
    aom_codec_set_option(&ctx, option_name, option_value);
    
    // Try with some known options to trigger different code paths
    aom_codec_set_option(&ctx, "cpu-used", "0");
    aom_codec_set_option(&ctx, "cpu-used", "8");
    aom_codec_set_option(&ctx, "crf", "0");
    aom_codec_set_option(&ctx, "crf", "63");
    aom_codec_set_option(&ctx, "error-resilient", "0");
    aom_codec_set_option(&ctx, "error-resilient", "1");
    aom_codec_set_option(&ctx, "error-resilient", "2");
    aom_codec_set_option(&ctx, "enable-adaptive-sharpness", "0");
    aom_codec_set_option(&ctx, "enable-adaptive-sharpness", "1");
    aom_codec_set_option(&ctx, "enable-flip-idtx", "0");
    aom_codec_set_option(&ctx, "enable-flip-idtx", "1");
    
    // Create a test image
    aom_image_t *img = create_fuzz_image(Data, Size);
    
    // Fuzz aom_codec_encode
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    if (img) {
        // Encode with valid image
        aom_codec_encode(&ctx, img, pts, duration, flags);
        
        // Encode with increasing PTS
        pts += duration;
        aom_codec_encode(&ctx, img, pts, duration, flags);
        
        // Try with different flags (using 1 as AOM_EFLAG_FORCE_KEY)
        aom_codec_encode(&ctx, img, pts + duration, duration, 1);
        
        // Try with zero duration (should fail or be handled)
        aom_codec_encode(&ctx, img, pts + duration, 0, flags);
        
        cleanup_image(img);
    }
    
    // Flush the encoder
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    
    // Drain remaining packets
    aom_codec_cx_pkt_t pkt;
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process encoded frame if needed
            // For fuzzing, we just consume it
        }
        // Note: aom_codec_get_cx_data does not free the data, 
        // but for encoder frames, the memory is managed by the codec
    }
    
    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        // Use the data if needed
        if (global_headers->buf && global_headers->sz > 0) {
            // Just access to ensure it's valid
            volatile char *ptr = (char *)global_headers->buf;
            (void)ptr[0];
        }
        // Free the buffer as per documentation
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }
    
    // Cleanup
    cleanup_encoder(&ctx);
    
    return 0;
}