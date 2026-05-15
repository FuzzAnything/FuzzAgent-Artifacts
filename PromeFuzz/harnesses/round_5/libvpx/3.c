// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Create dummy file for any potential file operations
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Step 1: vpx_img_alloc
    // Use fuzzed data to determine image parameters (with bounds checking)
    // Ensure we don't read beyond the buffer
    unsigned int width = 16;
    unsigned int height = 16;
    unsigned int align = 1;
    
    if (Size > 0) {
        width = 16 + (Data[0] % 256);  // 16-271 pixels
    }
    if (Size > 1) {
        height = 16 + (Data[1] % 256); // 16-271 pixels
    }
    if (Size > 2) {
        align = 1 << (Data[2] % 5);    // 1,2,4,8,16 alignment
    }
    
    // Ensure dimensions don't exceed limits (2^27)
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align > 65536) align = 65536;
    if (align == 0) align = 1; // Ensure align is at least 1
    
    // Choose a format from the fuzzed data
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420; // Default format
    if (Size > 3) {
        vpx_img_fmt_t formats[] = {
            VPX_IMG_FMT_YV12,
            VPX_IMG_FMT_I420,
            VPX_IMG_FMT_I422,
            VPX_IMG_FMT_I444,
            VPX_IMG_FMT_I440,
            VPX_IMG_FMT_NV12,
            VPX_IMG_FMT_I42016,
            VPX_IMG_FMT_I42216,
            VPX_IMG_FMT_I44416,
            VPX_IMG_FMT_I44016
        };
        size_t num_formats = sizeof(formats) / sizeof(formats[0]);
        fmt = formats[Data[3] % num_formats];
    }
    
    vpx_image_t* image = vpx_img_alloc(NULL, fmt, width, height, align);
    
    // Step 2: vpx_codec_vp9_cx
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
    
    // Step 3: vpx_codec_get_cx_data
    // First we need to create a codec context to use with get_cx_data
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iter_t iter = NULL;
    
    // Initialize encoder with default configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
        cfg.g_w = width;
        cfg.g_h = height;
        
        // Try to initialize the encoder
        if (vpx_codec_enc_init(&codec_ctx, iface, &cfg, 0) == VPX_CODEC_OK) {
            // Try to get encoded data (will be NULL since we haven't encoded anything)
            const vpx_codec_cx_pkt_t* pkt = vpx_codec_get_cx_data(&codec_ctx, &iter);
            (void)pkt; // Suppress unused variable warning
            
            // Step 4: vpx_codec_decode
            // Create a decoder context
            vpx_codec_ctx_t dec_ctx;
            vpx_codec_iface_t* dec_iface = vpx_codec_vp9_dx();
            
            if (vpx_codec_dec_init(&dec_ctx, dec_iface, NULL, 0) == VPX_CODEC_OK) {
                // Use fuzzed data as input to decode
                // Limit the size to avoid excessive processing time
                size_t decode_size = Size > 8192 ? 8192 : Size;
                if (decode_size > 0) {
                    vpx_codec_decode(&dec_ctx, Data, decode_size, NULL, 0);
                }
                
                // Clean up decoder
                vpx_codec_destroy(&dec_ctx);
            }
            
            // Step 5: vpx_codec_destroy (for encoder)
            vpx_codec_destroy(&codec_ctx);
        }
    }
    
    // Step 6: vpx_img_free
    vpx_img_free(image);
    
    // Clean up dummy file
    remove(dummy_file);
    
    return 0;
}