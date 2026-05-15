// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_codec_ctx_t ctx;
static aom_codec_enc_cfg_t cfg;
static aom_image_t img;
static int encoder_initialized = 0;

static void cleanup_encoder(void) {
    if (encoder_initialized) {
        aom_codec_destroy(&ctx);
        encoder_initialized = 0;
    }
}

static int init_encoder(void) {
    if (encoder_initialized) {
        return 1;
    }
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }
    
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    encoder_initialized = 1;
    return 1;
}

static void init_test_image(void) {
    memset(&img, 0, sizeof(img));
    img.fmt = AOM_IMG_FMT_I420;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = 0;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    img.w = 64;
    img.h = 64;
    img.bit_depth = 8;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    
    size_t y_size = img.w * img.h;
    size_t uv_size = (img.w >> img.x_chroma_shift) * (img.h >> img.y_chroma_shift);
    
    img.planes[0] = malloc(y_size);
    img.planes[1] = malloc(uv_size);
    img.planes[2] = malloc(uv_size);
    
    img.stride[0] = img.w;
    img.stride[1] = img.w >> img.x_chroma_shift;
    img.stride[2] = img.w >> img.x_chroma_shift;
    
    img.sz = y_size + 2 * uv_size;
    img.bps = 8;
    img.temporal_id = 0;
    img.spatial_id = 0;
    img.user_priv = NULL;
    img.img_data = NULL;
    img.img_data_owner = 0;
    img.self_allocd = 1;
    img.metadata = NULL;
    img.fb_priv = NULL;
}

static void cleanup_image(void) {
    if (img.planes[0]) free(img.planes[0]);
    if (img.planes[1]) free(img.planes[1]);
    if (img.planes[2]) free(img.planes[2]);
    memset(&img, 0, sizeof(img));
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    if (!init_encoder()) {
        return 0;
    }
    
    init_test_image();
    
    uint8_t mode = Data[0] % 6;
    size_t data_offset = 1;
    
    switch (mode) {
        case 0: {
            if (data_offset < Size) {
                size_t key_len = (Size - data_offset) / 2;
                if (key_len > 0) {
                    char *key = malloc(key_len + 1);
                    char *value = malloc(key_len + 1);
                    
                    if (key && value) {
                        memcpy(key, Data + data_offset, key_len);
                        key[key_len] = '\0';
                        
                        size_t value_offset = data_offset + key_len;
                        if (value_offset < Size) {
                            size_t remaining = Size - value_offset;
                            size_t val_len = remaining > key_len ? key_len : remaining;
                            memcpy(value, Data + value_offset, val_len);
                            value[val_len] = '\0';
                            
                            aom_codec_set_option(&ctx, key, value);
                        }
                    }
                    
                    if (key) free(key);
                    if (value) free(value);
                }
            }
            break;
        }
        
        case 1: {
            if (data_offset < Size) {
                int ctrl_id = Data[data_offset] % 256;
                data_offset++;
                
                if (data_offset < Size) {
                    int arg = Data[data_offset];
                    aom_codec_control(&ctx, ctrl_id, arg);
                }
            }
            break;
        }
        
        case 2: {
            if (img.planes[0] && img.planes[1] && img.planes[2]) {
                size_t y_size = img.w * img.h;
                size_t uv_size = (img.w >> img.x_chroma_shift) * (img.h >> img.y_chroma_shift);
                
                if (data_offset < Size) {
                    size_t copy_size = Size - data_offset;
                    size_t to_copy = copy_size > y_size ? y_size : copy_size;
                    memcpy(img.planes[0], Data + data_offset, to_copy);
                    data_offset += to_copy;
                    
                    if (data_offset < Size) {
                        copy_size = Size - data_offset;
                        to_copy = copy_size > uv_size ? uv_size : copy_size;
                        memcpy(img.planes[1], Data + data_offset, to_copy);
                        data_offset += to_copy;
                    }
                    
                    if (data_offset < Size) {
                        copy_size = Size - data_offset;
                        to_copy = copy_size > uv_size ? uv_size : copy_size;
                        memcpy(img.planes[2], Data + data_offset, to_copy);
                    }
                }
                
                aom_codec_pts_t pts = 0;
                unsigned long duration = 1;
                aom_enc_frame_flags_t flags = 0;
                
                aom_codec_encode(&ctx, &img, pts, duration, flags);
            }
            break;
        }
        
        case 3: {
            aom_codec_encode(&ctx, NULL, 0, 0, 0);
            break;
        }
        
        case 4: {
            const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
            (void)preview;
            break;
        }
        
        case 5: {
            if (encoder_initialized) {
                aom_codec_destroy(&ctx);
                encoder_initialized = 0;
                
                if (!init_encoder()) {
                    return 0;
                }
            }
            break;
        }
    }
    
    cleanup_image();
    
    return 0;
}