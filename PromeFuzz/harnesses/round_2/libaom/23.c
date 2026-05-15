// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

static aom_image_t *create_test_image(int width, int height, aom_img_fmt_t fmt, unsigned int bit_depth) {
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    // Fill planes with some data
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            int stride = img->stride[plane];
            int plane_height = (plane == 0) ? height : (height >> img->y_chroma_shift);
            for (int y = 0; y < plane_height; y++) {
                memset(img->planes[plane] + y * stride, y % 256, stride);
            }
        }
    }
    
    return img;
}

static void free_test_image(aom_image_t *img) {
    if (img) aom_img_free(img);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Use first byte to determine test configuration
    uint8_t config = data[0];
    size_t data_offset = 1;
    
    // Initialize encoder
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some basic parameters
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder with version check
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Test aom_codec_control with various control IDs
    if (size > data_offset + 4) {
        int ctrl_id;
        memcpy(&ctrl_id, data + data_offset, 4);
        data_offset += 4;
        
        // Try different control parameters based on remaining data
        if (size > data_offset) {
            int param = data[data_offset];
            data_offset++;
            
            // Try a few common control IDs
            switch (ctrl_id % 10) {
                case 0:
                    // AOME_SET_CPUUSED
                    aom_codec_control(&ctx, 8, param % 9);
                    break;
                case 1:
                    // AOME_SET_ENABLEAUTOALTREF
                    aom_codec_control(&ctx, 18, param & 1);
                    break;
                case 2:
                    // AOME_SET_ARNR_MAXFRAMES
                    aom_codec_control(&ctx, 5, param % 5);
                    break;
                case 3:
                    // AOME_SET_ARNR_STRENGTH
                    aom_codec_control(&ctx, 6, param % 7);
                    break;
                case 4:
                    // AOME_SET_CQ_LEVEL
                    aom_codec_control(&ctx, 10, param % 64);
                    break;
                default:
                    // Try with the raw control ID (might be invalid)
                    aom_codec_control(&ctx, ctrl_id, param);
                    break;
            }
        }
    }
    
    // Create test image
    aom_image_t *img = create_test_image(64, 64, AOM_IMG_FMT_I420, 8);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Test aom_codec_encode with image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    // Encode a few frames with different parameters
    for (int i = 0; i < 3; i++) {
        if (size > data_offset + i) {
            flags = data[data_offset + i] & 0xFF;
        }
        
        aom_codec_err_t encode_result = aom_codec_encode(&ctx, img, pts, duration, flags);
        (void)encode_result; // Result checked by context error field
        
        pts += duration;
    }
    
    // Test aom_codec_set_cx_data_buf
    if (size > data_offset + 16) {
        uint8_t buffer[1024];
        aom_fixed_buf_t buf;
        buf.buf = buffer;
        buf.sz = sizeof(buffer);
        
        unsigned int pad_before, pad_after;
        memcpy(&pad_before, data + data_offset, 4);
        memcpy(&pad_after, data + data_offset + 4, 4);
        data_offset += 8;
        
        aom_codec_set_cx_data_buf(&ctx, &buf, pad_before % 64, pad_after % 64);
        
        // Also test with NULL buffer
        if ((config & 0x1) == 0) {
            aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
        }
    }
    
    // Flush encoder (encode with NULL image)
    aom_codec_encode(&ctx, NULL, pts, duration, 0);
    
    // Test aom_codec_get_preview_frame
    const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
    (void)preview; // Preview may be NULL, that's OK
    
    // Try to get compressed data
    const aom_codec_cx_pkt_t *pkt;
    aom_codec_iter_t iter = NULL;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process packet if needed
        (void)pkt;
    }
    
    // Clean up
    free_test_image(img);
    
    // Test aom_codec_destroy
    aom_codec_err_t destroy_result = aom_codec_destroy(&ctx);
    (void)destroy_result;
    
    // Also test destroy with invalid context
    if ((config & 0x2) == 0) {
        aom_codec_ctx_t invalid_ctx;
        memset(&invalid_ctx, 0, sizeof(invalid_ctx));
        aom_codec_destroy(&invalid_ctx);
    }
    
    return 0;
}