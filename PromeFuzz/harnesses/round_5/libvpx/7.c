// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static void fuzz_vpx_image_functions(const uint8_t *data, size_t size) {
    if (size < 32) return;
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // Format (limited to valid values)
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(data[offset++] % 16);
    
    // Dimensions (capped to reasonable values for fuzzing)
    unsigned int d_w = ((data[offset] << 8) | data[offset + 1]) % 1024 + 1;
    offset += 2;
    unsigned int d_h = ((data[offset] << 8) | data[offset + 1]) % 1024 + 1;
    offset += 2;
    
    // Alignment (capped to reasonable values)
    unsigned int align = (data[offset++] % 256) + 1;
    
    // Rectangle parameters
    unsigned int rect_x = ((data[offset] << 8) | data[offset + 1]) % (d_w * 2);
    offset += 2;
    unsigned int rect_y = ((data[offset] << 8) | data[offset + 1]) % (d_h * 2);
    offset += 2;
    unsigned int rect_w = ((data[offset] << 8) | data[offset + 1]) % (d_w * 2);
    offset += 2;
    unsigned int rect_h = ((data[offset] << 8) | data[offset + 1]) % (d_h * 2);
    offset += 2;
    
    // Test vpx_img_alloc
    vpx_image_t *img1 = vpx_img_alloc(NULL, fmt, d_w, d_h, align);
    if (img1) {
        // Test vpx_img_set_rect
        vpx_img_set_rect(img1, rect_x, rect_y, rect_w, rect_h);
        
        // Test vpx_img_flip
        vpx_img_flip(img1);
        
        // Cleanup
        vpx_img_free(img1);
    }
    
    // Test vpx_img_wrap with different scenarios
    if (size - offset > 1024) {
        // Calculate approximate buffer size needed
        size_t buffer_size = d_w * d_h * 4; // Conservative estimate
        if (buffer_size > 1024 * 1024) buffer_size = 1024 * 1024; // Cap at 1MB
        
        unsigned char *img_data = (unsigned char*)malloc(buffer_size);
        if (img_data) {
            // Fill with fuzzer data
            size_t copy_size = (size - offset) < buffer_size ? (size - offset) : buffer_size;
            memcpy(img_data, data + offset, copy_size);
            
            // Test vpx_img_wrap
            vpx_image_t *img2 = vpx_img_wrap(NULL, fmt, d_w, d_h, align, img_data);
            if (img2) {
                // Test operations on wrapped image
                vpx_img_set_rect(img2, rect_x % d_w, rect_y % d_h, 
                                rect_w % (d_w - (rect_x % d_w) + 1),
                                rect_h % (d_h - (rect_y % d_h) + 1));
                vpx_img_flip(img2);
                vpx_img_free(img2);
            }
            
            // Note: img_data is owned by us, not by vpx_img_wrap
            free(img_data);
        }
    }
    
    // Test vpx_img_alloc with non-NULL img parameter
    vpx_image_t img_struct;
    vpx_image_t *img3 = vpx_img_alloc(&img_struct, fmt, d_w, d_h, align);
    if (img3) {
        vpx_img_free(img3);
    }
}

static void fuzz_vpx_codec_encode(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    // Initialize codec context
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Get VP8 encoder interface
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return;
    
    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Set some configuration from fuzzer data
    cfg.g_w = ((data[0] << 8) | data[1]) % 640 + 1;
    cfg.g_h = ((data[2] << 8) | data[3]) % 480 + 1;
    cfg.rc_target_bitrate = ((data[4] << 8) | data[5]) % 1000 + 100;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Create an image for encoding
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (img) {
        // Fill image planes with fuzzer data (simplified)
        size_t offset = 6;
        for (int plane = 0; plane < 3; plane++) {
            int plane_size = img->stride[plane] * (plane == 0 ? cfg.g_h : cfg.g_h / 2);
            if (plane_size > 0 && offset < size) {
                size_t copy_size = (size - offset) < (size_t)plane_size ? 
                                  (size - offset) : plane_size;
                if (copy_size > 0 && img->planes[plane]) {
                    memcpy(img->planes[plane], data + offset, copy_size);
                    offset += copy_size;
                }
            }
        }
        
        // Encode the frame
        vpx_codec_encode(&ctx, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Flush encoder
        vpx_codec_encode(&ctx, NULL, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        const vpx_codec_cx_pkt_t *pkt;
        vpx_codec_iter_t iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet if needed
        }
        
        vpx_img_free(img);
    }
    
    // Destroy codec
    vpx_codec_destroy(&ctx);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Test image functions
    fuzz_vpx_image_functions(Data, Size);
    
    // Test encoder functions (with smaller probability to avoid timeout)
    if (Size > 100 && (Data[0] % 10 == 0)) {  // ~10% of inputs
        fuzz_vpx_codec_encode(Data, Size);
    }
    
    return 0;
}