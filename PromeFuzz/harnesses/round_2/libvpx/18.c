// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"

// Declare the interface functions that are defined in the library
extern vpx_codec_iface_t* vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t* vpx_codec_vp9_dx(void);
extern vpx_codec_iface_t* vpx_codec_vp8_cx(void);
extern vpx_codec_iface_t* vpx_codec_vp9_cx(void);

static void dummy_image_init(vpx_image_t *img, const uint8_t *data, size_t size) {
    if (!img || size < 16) return;
    
    // Use fuzz data to initialize image parameters
    img->fmt = (vpx_img_fmt_t)(data[0] % 8);
    img->cs = (vpx_color_space_t)(data[1] % 8);
    img->range = (vpx_color_range_t)(data[2] % 2);
    img->w = (data[3] << 8) | data[4];
    img->h = (data[5] << 8) | data[6];
    img->bit_depth = (data[7] % 16) + 1;
    img->d_w = img->w;
    img->d_h = img->h;
    img->r_w = img->w;
    img->r_h = img->h;
    img->x_chroma_shift = (data[8] % 4);
    img->y_chroma_shift = (data[9] % 4);
    img->bps = (data[10] << 8) | data[11];
    
    // Initialize planes and strides with dummy values
    for (int i = 0; i < 4; i++) {
        img->stride[i] = img->w;
    }
    
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->fb_priv = NULL;
}

static vpx_codec_iface_t* get_decoder_iface(const uint8_t *data, size_t size) {
    if (size == 0) return vpx_codec_vp8_dx();
    return (data[0] % 2 == 0) ? vpx_codec_vp8_dx() : vpx_codec_vp9_dx();
}

static vpx_codec_iface_t* get_encoder_iface(const uint8_t *data, size_t size) {
    if (size == 0) return vpx_codec_vp8_cx();
    return (data[0] % 2 == 0) ? vpx_codec_vp8_cx() : vpx_codec_vp9_cx();
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test 1: vpx_codec_get_preview_frame
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface = get_encoder_iface(Data, Size);
        
        // Initialize encoder with dummy config
        vpx_codec_enc_cfg_t cfg;
        if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
            if (Size > 100) {
                cfg.g_w = (Data[1] << 8) | Data[2];
                cfg.g_h = (Data[3] << 8) | Data[4];
                if (cfg.g_w == 0) cfg.g_w = 1;
                if (cfg.g_h == 0) cfg.g_h = 1;
            }
            
            if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) == VPX_CODEC_OK) {
                // Try to get preview frame
                const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
                (void)preview; // Result may be NULL, that's OK
                
                vpx_codec_destroy(&ctx);
            }
        }
    }
    
    // Test 2: vpx_codec_get_frame and vpx_codec_decode
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface = get_decoder_iface(Data, Size);
        
        vpx_codec_dec_cfg_t cfg;
        cfg.threads = (Size > 10) ? (Data[5] % 8) + 1 : 1;
        cfg.w = (Size > 12) ? (Data[6] << 8) | Data[7] : 0;
        cfg.h = (Size > 14) ? (Data[8] << 8) | Data[9] : 0;
        
        if (vpx_codec_dec_init(&ctx, iface, &cfg, 0) == VPX_CODEC_OK) {
            // Decode the input data
            vpx_codec_decode(&ctx, Data, Size, NULL, 0);
            
            // Iterate through decoded frames
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                (void)img; // Just iterate, don't use the image
            }
            
            // Try to flush decoder
            vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
            
            vpx_codec_destroy(&ctx);
        }
    }
    
    // Test 3: vpx_codec_dec_init_ver (called via macro in test 2, but test directly)
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface = get_decoder_iface(Data, Size);
        
        vpx_codec_dec_cfg_t cfg;
        cfg.threads = (Size > 20) ? (Data[10] % 8) + 1 : 1;
        cfg.w = (Size > 22) ? (Data[11] << 8) | Data[12] : 0;
        cfg.h = (Size > 24) ? (Data[13] << 8) | Data[14] : 0;
        
        // Call the _ver function directly
        vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        (void)err;
        
        if (ctx.err == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }
    }
    
    // Test 4: vpx_codec_encode
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface = get_encoder_iface(Data, Size);
        
        vpx_codec_enc_cfg_t cfg;
        if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
            if (Size > 50) {
                cfg.g_w = (Data[15] << 8) | Data[16];
                cfg.g_h = (Data[17] << 8) | Data[18];
                if (cfg.g_w == 0) cfg.g_w = 1;
                if (cfg.g_h == 0) cfg.g_h = 1;
            }
            
            if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) == VPX_CODEC_OK) {
                // Create dummy image for encoding
                vpx_image_t img;
                if (Size > 30) {
                    dummy_image_init(&img, Data + 20, Size - 20);
                    
                    // Encode with fuzzed parameters
                    vpx_codec_pts_t pts = (Size > 40) ? (Data[25] << 24) | (Data[26] << 16) | (Data[27] << 8) | Data[28] : 0;
                    unsigned long duration = (Size > 45) ? (Data[29] << 8) | Data[30] : 1;
                    vpx_enc_frame_flags_t flags = (Size > 50) ? Data[31] : 0;
                    vpx_enc_deadline_t deadline = (Size > 55) ? (Data[32] << 8) | Data[33] : 0;
                    
                    vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
                }
                
                // Flush encoder
                vpx_codec_encode(&ctx, NULL, 0, 1, 0, 0);
                
                vpx_codec_destroy(&ctx);
            }
        }
    }
    
    // Test 5: vpx_codec_control_
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface = get_decoder_iface(Data, Size);
        
        if (vpx_codec_dec_init(&ctx, iface, NULL, 0) == VPX_CODEC_OK) {
            // Try various control IDs based on fuzz data
            for (int i = 0; i < 5 && i * 2 + 1 < (int)Size; i++) {
                int ctrl_id = Data[i * 2] % 256;
                int value = Data[i * 2 + 1];
                
                // Try different argument types based on control ID
                if (ctrl_id % 3 == 0) {
                    vpx_codec_control_(&ctx, ctrl_id, value);
                } else if (ctrl_id % 3 == 1) {
                    int *ptr = &value;
                    vpx_codec_control_(&ctx, ctrl_id, ptr);
                } else {
                    vpx_codec_control_(&ctx, ctrl_id, (void*)&value);
                }
            }
            
            vpx_codec_destroy(&ctx);
        }
        
        // Also test with encoder
        vpx_codec_ctx_t enc_ctx;
        vpx_codec_iface_t *enc_iface = get_encoder_iface(Data, Size);
        
        vpx_codec_enc_cfg_t cfg;
        if (vpx_codec_enc_config_default(enc_iface, &cfg, 0) == VPX_CODEC_OK) {
            cfg.g_w = 16;
            cfg.g_h = 16;
            
            if (vpx_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) == VPX_CODEC_OK) {
                // Try encoder controls
                if (Size > 60) {
                    int ctrl_id = Data[40] % 256;
                    int value = Data[41];
                    vpx_codec_control_(&enc_ctx, ctrl_id, value);
                }
                
                vpx_codec_destroy(&enc_ctx);
            }
        }
    }
    
    // Test 6: vpx_codec_decode (additional tests)
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface = get_decoder_iface(Data, Size);
        
        if (vpx_codec_dec_init(&ctx, iface, NULL, 0) == VPX_CODEC_OK) {
            // Test fragmented input if flag is set
            unsigned int flags = (Size > 70) ? Data[50] : 0;
            if (flags & 0x01) {
                // Try with input fragments flag
                vpx_codec_dec_init(&ctx, iface, NULL, VPX_CODEC_USE_INPUT_FRAGMENTS);
            }
            
            // Decode in chunks if data is large enough
            if (Size > 100) {
                size_t chunk_size = Size / 3;
                vpx_codec_decode(&ctx, Data, chunk_size, NULL, 0);
                vpx_codec_decode(&ctx, Data + chunk_size, chunk_size, NULL, 0);
                vpx_codec_decode(&ctx, Data + chunk_size * 2, Size - chunk_size * 2, NULL, 0);
            } else {
                vpx_codec_decode(&ctx, Data, Size, NULL, 0);
            }
            
            // Flush
            vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
            
            vpx_codec_destroy(&ctx);
        }
    }
    
    return 0;
}