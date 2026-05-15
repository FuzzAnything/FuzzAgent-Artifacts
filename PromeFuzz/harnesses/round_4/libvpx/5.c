// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_decoder.h"

static void fuzz_image_alloc_and_free(const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    // Extract parameters from fuzz data
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(data[0] % 13); // 13 formats in enum
    unsigned int d_w = ((unsigned int)data[1] << 8) | data[2];
    unsigned int d_h = ((unsigned int)data[3] << 8) | data[4];
    unsigned int align = ((unsigned int)data[5] << 8) | data[6];
    
    // Apply constraints from documentation
    if (d_w > 0x08000000) d_w = 0x08000000;
    if (d_h > 0x08000000) d_h = 0x08000000;
    if (align > 65536) align = 65536;
    if (align == 0) align = 1; // Avoid zero alignment
    
    // Allocate image
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, d_w, d_h, align);
    
    // If allocation succeeded, try to fill with some data
    if (img) {
        size_t data_offset = 7;
        for (int plane = 0; plane < 4; plane++) {
            if (img->planes[plane] && img->stride[plane] > 0) {
                // Calculate actual plane height based on chroma subsampling
                unsigned int plane_height = img->h;
                if (plane > 0 && img->y_chroma_shift > 0) {
                    plane_height = (img->h + (1 << img->y_chroma_shift) - 1) >> img->y_chroma_shift;
                }
                
                size_t plane_size = img->stride[plane] * plane_height;
                if (plane_size > 0 && data_offset < size) {
                    size_t copy_size = (size - data_offset < plane_size) ? 
                                      size - data_offset : plane_size;
                    memcpy(img->planes[plane], data + data_offset, copy_size);
                    data_offset += copy_size;
                }
            }
        }
    }
    
    // Free image
    vpx_img_free(img);
}

static void fuzz_encoder_interface(const uint8_t *data, size_t size) {
    // Get encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return;
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Use default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Modify config with fuzz data if we have enough
    if (size >= 8) {
        cfg.g_w = ((unsigned int)data[0] << 8) | data[1];
        cfg.g_h = ((unsigned int)data[2] << 8) | data[3];
        cfg.g_timebase.num = ((int)data[4] << 8) | data[5];
        cfg.g_timebase.den = ((int)data[6] << 8) | data[7];
        if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1;
        if (cfg.g_w > 4096) cfg.g_w = 4096;
        if (cfg.g_h > 4096) cfg.g_h = 4096;
        if (cfg.g_w == 0) cfg.g_w = 64;
        if (cfg.g_h == 0) cfg.g_h = 64;
    }
    
    // Initialize encoder
    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Create a dummy image for encoding
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (img) {
        // Fill with some data, respecting plane dimensions
        for (int plane = 0; plane < 3; plane++) {
            if (img->planes[plane] && img->stride[plane] > 0) {
                // Calculate actual plane height based on chroma subsampling
                unsigned int plane_height = img->h;
                if (plane > 0) {
                    // For I420, chroma planes are half height
                    plane_height = (img->h + 1) >> 1;
                }
                
                size_t plane_size = img->stride[plane] * plane_height;
                if (plane_size > 0) {
                    // Use a safe fill pattern
                    for (size_t i = 0; i < plane_size; i++) {
                        img->planes[plane][i] = 128;
                    }
                }
            }
        }
        
        // Encode the image
        if (vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
            // Get encoded data
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Do nothing with packet, just iterate
            }
        }
        
        vpx_img_free(img);
    }
    
    // Destroy encoder
    vpx_codec_destroy(&codec);
}

static void fuzz_decoder(const uint8_t *data, size_t size) {
    // For decoder, we need to use the same encoder interface to create data first
    // Then decode it
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return;
    
    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Set small dimensions for quick encoding
    cfg.g_w = 64;
    cfg.g_h = 64;
    
    if (vpx_codec_enc_init(&encoder, iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Create a simple image
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&encoder);
        return;
    }
    
    // Fill with test pattern, respecting plane dimensions
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane] && img->stride[plane] > 0) {
            // Calculate actual plane height based on chroma subsampling
            unsigned int plane_height = img->h;
            if (plane > 0) {
                // For I420, chroma planes are half height
                plane_height = (img->h + 1) >> 1;
            }
            
            size_t plane_size = img->stride[plane] * plane_height;
            if (plane_size > 0) {
                // Use a safe fill pattern
                for (size_t i = 0; i < plane_size; i++) {
                    img->planes[plane][i] = 128;
                }
            }
        }
    }
    
    // Encode
    if (vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        
        // Find a frame packet
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Now decode the encoded frame
                vpx_codec_ctx_t decoder;
                
                // Initialize decoder with VP9 interface
                if (vpx_codec_dec_init(&decoder, iface, NULL, 0) == VPX_CODEC_OK) {
                    // Decode the frame
                    vpx_codec_decode(&decoder, (const uint8_t*)pkt->data.frame.buf, 
                                    pkt->data.frame.sz, NULL, 0);
                    
                    // Destroy decoder
                    vpx_codec_destroy(&decoder);
                }
                break;
            }
        }
    }
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to decide which path to take
    uint8_t selector = Data[0];
    
    // Test different code paths
    if (selector < 85) {
        // Test image allocation and encoder interface
        fuzz_image_alloc_and_free(Data, Size);
        fuzz_encoder_interface(Data, Size);
    } else if (selector < 170) {
        // Test decoder with encoded data
        fuzz_decoder(Data, Size);
    } else {
        // Test all functions together
        fuzz_image_alloc_and_free(Data, Size);
        fuzz_encoder_interface(Data, Size);
        fuzz_decoder(Data, Size);
    }
    
    return 0;
}