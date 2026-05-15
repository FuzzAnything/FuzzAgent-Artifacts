// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

// Declare the AV1 encoder interface function
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    static int width = 64;
    static int height = 64;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    
    if (!img) return NULL;
    
    // Fill planes with fuzzer data
    size_t offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = (plane == 0) ? width : width / 2;
        int plane_height = (plane == 0) ? height : height / 2;
        size_t plane_size = plane_width * plane_height;
        
        if (plane_size == 0) continue;
        
        for (int y = 0; y < plane_height; ++y) {
            size_t row_size = plane_width;
            size_t copy_size = (size - offset) < row_size ? (size - offset) : row_size;
            
            if (copy_size > 0) {
                memcpy(img->planes[plane] + y * img->stride[plane], 
                       data + offset, copy_size);
                offset = (offset + copy_size) % size;
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

static void generate_random_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Get default config
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;
    
    aom_codec_enc_config_default(iface, cfg, 0);
    
    // Modify some parameters with fuzzer data
    if (size > 0) {
        cfg->g_w = 64 + (data[0] % 64);
        cfg->g_h = 64 + (data[1] % 64);
        cfg->rc_target_bitrate = 100 + (data[2] % 1000);
        cfg->rc_min_quantizer = data[3] % 64;
        cfg->rc_max_quantizer = cfg->rc_min_quantizer + (data[4] % (64 - cfg->rc_min_quantizer + 1));
        cfg->g_threads = 1 + (data[5] % 4);
        cfg->g_error_resilient = (data[6] & 1) ? 1 : 0;
        cfg->g_pass = (data[7] % 3 == 0) ? AOM_RC_ONE_PASS : 
                     (data[7] % 3 == 1) ? AOM_RC_FIRST_PASS : AOM_RC_SECOND_PASS;
        cfg->g_lag_in_frames = data[8] % 16;
        cfg->kf_mode = (data[9] & 1) ? AOM_KF_AUTO : AOM_KF_DISABLED;
        cfg->kf_min_dist = data[10] % 32;
        cfg->kf_max_dist = cfg->kf_min_dist + (data[11] % 100);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
    if (size < 12) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    // Generate random config
    generate_random_config(&cfg, data, size);
    
    // Initialize encoder
    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Test aom_codec_control with various control IDs
    int ctrl_id;
    int value;
    size_t offset = 12;
    
    // Try a few different control IDs based on fuzzer data
    for (int i = 0; i < 5 && offset + 2 < size; i++) {
        ctrl_id = data[offset] % 100;  // Use modulo to get valid-ish control IDs
        value = data[offset + 1];
        aom_codec_control(&ctx, ctrl_id, value);
        offset += 2;
    }
    
    // Create test image
    aom_image_t *img = create_test_image(data + offset, size - offset);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Encode a frame
    aom_codec_encode(&ctx, img, 0, 1, 0);
    
    // Test setting cx data buffer
    uint8_t buffer[4096];
    aom_fixed_buf_t fixed_buf;
    fixed_buf.buf = buffer;
    fixed_buf.sz = sizeof(buffer);
    
    unsigned int pad_before = (size > offset + 1) ? data[offset] % 256 : 0;
    unsigned int pad_after = (size > offset + 2) ? data[offset + 1] % 256 : 0;
    aom_codec_set_cx_data_buf(&ctx, &fixed_buf, pad_before, pad_after);
    
    // Encode another frame with different parameters
    aom_codec_encode(&ctx, img, 1, 1, 0);
    
    // Flush encoder
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    
    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // Test reconfiguring encoder
    if (size > offset + 20) {
        cfg.g_w = 32 + (data[offset + 3] % 96);
        cfg.g_h = 32 + (data[offset + 4] % 96);
        cfg.rc_target_bitrate = 50 + (data[offset + 5] % 500);
        aom_codec_enc_config_set(&ctx, &cfg);
    }
    
    // Encode one more frame after reconfig
    aom_codec_encode(&ctx, img, 2, 1, 0);
    
    // Clean up
    aom_img_free(img);
    aom_codec_destroy(&ctx);
    
    return 0;
}