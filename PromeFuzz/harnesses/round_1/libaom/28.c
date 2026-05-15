// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_image_t *create_dummy_image(const uint8_t *data, size_t size) {
    static const unsigned int width = 16;
    static const unsigned int height = 16;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    // Fill planes with dummy data
    size_t offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        unsigned int plane_width = (plane == 0) ? width : width / 2;
        unsigned int plane_height = (plane == 0) ? height : height / 2;
        
        for (unsigned int y = 0; y < plane_height; ++y) {
            size_t row_size = plane_width;
            size_t copy_size = (size - offset < row_size) ? size - offset : row_size;
            if (copy_size > 0) {
                memcpy(img->planes[plane] + y * img->stride[plane], 
                       data + offset, copy_size);
                offset += copy_size;
                if (offset >= size) offset = 0;
            }
        }
    }
    
    img->bit_depth = 8;
    img->range = AOM_CR_STUDIO_RANGE;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    
    return img;
}

static void fuzz_aom_codec_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int ctrl_id = *(int*)data;
    if (ctrl_id == 0) return; // Must be nonzero per API
    
    // Try different parameter types based on control ID
    if (ctrl_id == AOME_SET_CPUUSED || ctrl_id == AOME_SET_ENABLEAUTOALTREF || 
        ctrl_id == AOME_SET_SHARPNESS) {
        int value = (size >= sizeof(int) * 2) ? *(int*)(data + sizeof(int)) : 1;
        aom_codec_control(ctx, ctrl_id, value);
    } else if (ctrl_id == AOME_SET_CQ_LEVEL || ctrl_id == AOME_SET_ARNR_MAXFRAMES) {
        unsigned int value = (size >= sizeof(int) * 2) ? *(unsigned int*)(data + sizeof(int)) : 10;
        aom_codec_control(ctx, ctrl_id, value);
    } else {
        // Generic fallback with int parameter
        int value = (size >= sizeof(int) * 2) ? *(int*)(data + sizeof(int)) : 0;
        aom_codec_control(ctx, ctrl_id, value);
    }
}

static void fuzz_aom_codec_set_option(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Extract key and value from fuzzer data
    size_t key_len = data[0] % 32;
    if (key_len == 0 || key_len + 1 >= size) key_len = 1;
    
    size_t val_len = data[key_len] % 32;
    if (val_len == 0 || key_len + val_len + 1 >= size) val_len = 1;
    
    char key[33] = {0};
    char value[33] = {0};
    
    memcpy(key, data + 1, (key_len < 32) ? key_len : 32);
    memcpy(value, data + key_len + 1, (val_len < 32) ? val_len : 32);
    
    aom_codec_set_option(ctx, key, value);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    // Test aom_codec_enc_config_default
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = data[0] % 3; // 0, 1, or 2 for the three usages
    aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg, usage);
    if (err != AOM_CODEC_OK) return 0;
    
    // Modify config with fuzzer data
    if (size >= sizeof(unsigned int) * 4) {
        cfg.g_w = 64 + (*(unsigned int*)data % 192);
        cfg.g_h = 64 + (*(unsigned int*)(data + sizeof(unsigned int)) % 192);
        cfg.rc_target_bitrate = 100 + (*(unsigned int*)(data + sizeof(unsigned int) * 2) % 9900);
        cfg.g_limit = 1 + (*(unsigned int*)(data + sizeof(unsigned int) * 3) % 100);
    }
    
    // Test aom_codec_enc_init_ver
    aom_codec_ctx_t ctx;
    err = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) return 0;
    
    // Test aom_codec_enc_config_set with modified config
    if (size >= sizeof(unsigned int) * 8) {
        cfg.g_threads = 1 + (*(unsigned int*)(data + sizeof(unsigned int) * 4) % 8);
        cfg.rc_min_quantizer = *(unsigned int*)(data + sizeof(unsigned int) * 5) % 64;
        cfg.rc_max_quantizer = 4 + (*(unsigned int*)(data + sizeof(unsigned int) * 6) % 60);
        if (cfg.rc_max_quantizer < cfg.rc_min_quantizer) {
            cfg.rc_max_quantizer = cfg.rc_min_quantizer + 4;
        }
        cfg.g_error_resilient = *(unsigned int*)(data + sizeof(unsigned int) * 7) & 0xF;
        aom_codec_enc_config_set(&ctx, &cfg);
    }
    
    // Test aom_codec_set_option
    fuzz_aom_codec_set_option(&ctx, data, size);
    
    // Test aom_codec_control
    fuzz_aom_codec_control(&ctx, data, size);
    
    // Test aom_codec_encode with dummy image
    aom_image_t *img = create_dummy_image(data, size);
    if (img) {
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        // Encode a frame
        aom_codec_encode(&ctx, img, pts, duration, flags);
        
        // Encode a flush frame (NULL img)
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
        
        aom_img_free(img);
    }
    
    // Clean up
    aom_codec_destroy(&ctx);
    
    return 0;
}