// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_image(aom_image_t *img) {
    if (img && img->img_data) {
        aom_img_free(img);
    }
}

static int create_test_image(const uint8_t *data, size_t size, aom_image_t *img, int width, int height) {
    if (width <= 0 || height <= 0) {
        return 0;
    }
    
    // Ensure dimensions are multiples of 2 for YUV420
    width = (width + 1) & ~1;
    height = (height + 1) & ~1;
    
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
        return 0;
    }
    
    // Fill image with fuzzing data if available
    if (data && size > 0) {
        size_t plane_size = img->stride[0] * img->h;
        size_t u_plane_size = img->stride[1] * (img->h / 2);
        size_t v_plane_size = img->stride[2] * (img->h / 2);
        size_t total_needed = plane_size + u_plane_size + v_plane_size;
        
        size_t copy_size = (size < total_needed) ? size : total_needed;
        
        memcpy(img->planes[0], data, copy_size);
        
        // Fill remaining planes with deterministic data if input is short
        if (copy_size < plane_size) {
            memset(img->planes[0] + copy_size, 0x80, plane_size - copy_size);
        }
        if (copy_size < plane_size + u_plane_size) {
            size_t u_start = (copy_size > plane_size) ? plane_size : copy_size;
            size_t u_copy = (copy_size < plane_size + u_plane_size) ? copy_size - u_start : u_plane_size;
            if (u_copy > 0) {
                memcpy(img->planes[1], data + u_start, u_copy);
                memset(img->planes[1] + u_copy, 0x80, u_plane_size - u_copy);
            } else {
                memset(img->planes[1], 0x80, u_plane_size);
            }
        }
        if (copy_size < plane_size + u_plane_size + v_plane_size) {
            size_t v_start = (copy_size > plane_size + u_plane_size) ? plane_size + u_plane_size : copy_size;
            size_t v_copy = (copy_size < plane_size + u_plane_size + v_plane_size) ? copy_size - v_start : v_plane_size;
            if (v_copy > 0) {
                memcpy(img->planes[2], data + v_start, v_copy);
                memset(img->planes[2] + v_copy, 0x80, v_plane_size - v_copy);
            } else {
                memset(img->planes[2], 0x80, v_plane_size);
            }
        }
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_225(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t res;
    int width, height;
    aom_codec_pts_t pts;
    unsigned long duration;
    aom_enc_frame_flags_t flags;
    int enable_flip_idtx;
    int enable_intrabc;
    aom_svc_params_t svc_params;
    char option_key[64];
    char option_value[64];
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Extract parameters from fuzzing input with strict bounds checking
    if (Size < 16) {
        return 0;
    }
    
    width = Data[0] | (Data[1] << 8);
    height = Data[2] | (Data[3] << 8);
    pts = (int64_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
    duration = Data[8] | ((Size > 9) ? (Data[9] << 8) : 0);
    flags = Data[10] | ((Size > 11) ? (Data[11] << 8) : 0);
    enable_flip_idtx = (Size > 12) ? Data[12] : 0;
    enable_intrabc = (Size > 13) ? Data[13] : 0;
    
    // Ensure valid dimensions
    if (width < 2 || height < 2) {
        width = 16;
        height = 16;
    }
    if (width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }
    
    // Ensure duration is valid
    if (duration == 0) {
        duration = 1;
    }
    if (duration > UINT32_MAX) {
        duration = 1;
    }
    
    // Initialize encoder configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_pass = 0;
    
    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }
    
    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set up SVC parameters
    svc_params.number_spatial_layers = (Size > 14) ? (Data[14] % 4) : 1;
    svc_params.number_temporal_layers = (Size > 15) ? (Data[15] % 4) : 1;
    if (svc_params.number_spatial_layers < 1) svc_params.number_spatial_layers = 1;
    if (svc_params.number_temporal_layers < 1) svc_params.number_temporal_layers = 1;
    
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = 63;
        svc_params.min_quantizers[i] = 0;
        svc_params.layer_target_bitrate[i] = 1000;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        svc_params.scaling_factor_num[i] = 1;
        svc_params.scaling_factor_den[i] = 1;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        svc_params.framerate_factor[i] = 1;
    }
    
    // Test aom_codec_set_option with various options
    if (Size > 16) {
        size_t remaining = Size - 16;
        size_t key_len = remaining / 2;
        size_t val_len = remaining - key_len;
        
        if (key_len > 63) key_len = 63;
        if (val_len > 63) val_len = 63;
        
        memcpy(option_key, Data + 16, key_len);
        option_key[key_len] = '\0';
        memcpy(option_value, Data + 16 + key_len, val_len);
        option_value[val_len] = '\0';
        
        // Try to set option
        aom_codec_set_option(&ctx, option_key, option_value);
    }
    
    // Create test image
    if (!create_test_image(Data + 16, Size - 16, &img, width, height)) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Encode frames
    int frame_count = 0;
    int max_frames = 10;
    
    while (frame_count < max_frames) {
        res = aom_codec_encode(&ctx, &img, pts + frame_count, duration, flags);
        if (res != AOM_CODEC_OK) {
            break;
        }
        frame_count++;
    }
    
    // Flush encoder
    while (1) {
        aom_codec_cx_pkt_t *pkt;
        res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) {
            break;
        }
        
        // Check if there's any output
        if (!aom_codec_get_cx_data(&ctx, &pkt)) {
            break;
        }
        
        // We don't free the packet here to avoid potential double-free or API mismatch
        // In a real application, we would handle this based on packet type
        (void)pkt; // Suppress unused variable warning
    }
    
    // Cleanup
    cleanup_image(&img);
    aom_codec_destroy(&ctx);
    
    return 0;
}