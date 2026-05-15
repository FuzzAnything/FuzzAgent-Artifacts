// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

// Declare the encoder interface function since it's not in the headers we have
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static int read_int(const uint8_t **data, size_t *size, int min, int max) {
    if (*size < sizeof(int)) return min;
    int val = *(const int*)(*data);
    *data += sizeof(int);
    *size -= sizeof(int);
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static const char* read_string(const uint8_t **data, size_t *size, char *buf, size_t buf_size) {
    if (buf_size == 0) return "";
    size_t len = 0;
    while (len < buf_size - 1 && len < *size && (*data)[len] != 0) {
        buf[len] = (*data)[len];
        len++;
    }
    buf[len] = '\0';
    *data += len;
    *size -= len;
    if (len < *size && (*data)[0] == 0) {
        (*data)++;
        (*size)--;
    }
    return buf;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    const uint8_t *data = Data;
    size_t size = Size;
    
    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Read some configuration parameters from fuzzer input
    cfg.g_w = read_int(&data, &size, 16, 4096);
    cfg.g_h = read_int(&data, &size, 16, 4096);
    cfg.g_threads = read_int(&data, &size, 0, 64);
    cfg.g_profile = read_int(&data, &size, 0, 3);
    cfg.rc_target_bitrate = read_int(&data, &size, 1, 1000000);
    cfg.rc_min_quantizer = read_int(&data, &size, 0, 63);
    cfg.rc_max_quantizer = read_int(&data, &size, 0, 63);
    cfg.g_error_resilient = read_int(&data, &size, 0, 1);
    cfg.g_pass = read_int(&data, &size, 0, 3);
    cfg.g_lag_in_frames = read_int(&data, &size, 0, 25);
    cfg.kf_mode = read_int(&data, &size, 0, 2);
    cfg.kf_min_dist = read_int(&data, &size, 0, 1000);
    cfg.kf_max_dist = read_int(&data, &size, 0, 1000);
    
    // Initialize codec context
    aom_codec_ctx_t codec;
    if (aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some options from fuzzer input
    char key_buf[256], value_buf[256];
    for (int i = 0; i < 5 && size > 2; i++) {
        const char *key = read_string(&data, &size, key_buf, sizeof(key_buf));
        const char *value = read_string(&data, &size, value_buf, sizeof(value_buf));
        aom_codec_set_option(&codec, key, value);
    }
    
    // Try various control IDs
    int control_ids[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        256, 257, 258, 259, 260,
        512, 513, 514, 515,
        768, 769, 770,
        1024, 1025, 1026
    };
    
    for (int i = 0; i < sizeof(control_ids)/sizeof(control_ids[0]) && size >= sizeof(int); i++) {
        int param = read_int(&data, &size, 0, 1000);
        aom_codec_control(&codec, control_ids[i], param);
    }
    
    // Allocate and initialize an image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (img) {
        // Fill image planes with fuzzer data
        size_t y_plane_size = cfg.g_w * cfg.g_h;
        size_t uv_plane_size = (cfg.g_w / 2) * (cfg.g_h / 2);
        
        if (size > y_plane_size) {
            memcpy(img->planes[0], data, y_plane_size);
            data += y_plane_size;
            size -= y_plane_size;
            
            if (size > uv_plane_size * 2) {
                memcpy(img->planes[1], data, uv_plane_size);
                data += uv_plane_size;
                size -= uv_plane_size;
                
                memcpy(img->planes[2], data, uv_plane_size);
                data += uv_plane_size;
                size -= uv_plane_size;
            }
        }
        
        // Encode the image
        aom_codec_pts_t pts = read_int(&data, &size, 0, 1000);
        unsigned long duration = read_int(&data, &size, 1, 1000);
        aom_enc_frame_flags_t flags = read_int(&data, &size, 0, 3);
        
        aom_codec_encode(&codec, img, pts, duration, flags);
        
        // Try to get preview frame
        const aom_image_t *preview = aom_codec_get_preview_frame(&codec);
        (void)preview; // Preview may be NULL
        
        // Encode a NULL frame to flush
        aom_codec_encode(&codec, NULL, 0, 1, 0);
        
        aom_img_free(img);
    }
    
    // Clean up
    aom_codec_destroy(&codec);
    
    return 0;
}