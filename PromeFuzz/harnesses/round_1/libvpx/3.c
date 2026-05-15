// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static vpx_codec_ctx_t codec_ctx;

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Initialize image parameters using fuzz data
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int d_w = 64;
    unsigned int d_h = 64;
    unsigned int align = 16;
    
    if (Size >= 12) {
        // Use first 12 bytes for image parameters
        d_w = (*(uint32_t*)(Data) % 1024) + 1;  // Width 1-1024
        d_h = (*(uint32_t*)(Data + 4) % 1024) + 1; // Height 1-1024
        align = 1 << (*(uint32_t*)(Data + 8) % 5); // Alignment 1,2,4,8,16
    }
    
    // 1. Allocate image
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, d_w, d_h, align);
    if (!img) {
        return 0;
    }
    
    // Initialize image planes with some data
    if (Size > 12) {
        size_t data_offset = 12;
        for (int plane = 0; plane < 3; plane++) {
            if (img->planes[plane]) {
                size_t plane_size = img->stride[plane] * img->h;
                size_t copy_size = (Size - data_offset < plane_size) ? 
                    Size - data_offset : plane_size;
                if (copy_size > 0) {
                    memcpy(img->planes[plane], Data + data_offset, copy_size);
                    data_offset += copy_size;
                }
            }
        }
    }
    
    // 2. Get VP9 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        vpx_img_free(img);
        return 0;
    }
    
    // Initialize encoder with default configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    cfg.g_w = d_w;
    cfg.g_h = d_h;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&codec_ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Encode the image
    if (vpx_codec_encode(&codec_ctx, img, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx);
        vpx_img_free(img);
        return 0;
    }
    
    // 3. Get encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        // Process packet if needed
    }
    
    // 4. Decode using the same context (re-initialize as decoder)
    vpx_codec_destroy(&codec_ctx);  // Destroy encoder first
    
    // Initialize as decoder
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    if (!dec_iface) {
        vpx_img_free(img);
        return 0;
    }
    
    if (vpx_codec_dec_init(&codec_ctx, dec_iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Use remaining fuzz data for decoding
    if (Size > 12) {
        size_t decode_size = Size - 12;
        if (decode_size > 0) {
            vpx_codec_decode(&codec_ctx, Data + 12, decode_size, NULL, 0);
        }
    }
    
    // Flush decoder
    vpx_codec_decode(&codec_ctx, NULL, 0, NULL, 0);
    
    // 5. Destroy codec
    vpx_codec_destroy(&codec_ctx);
    
    // 6. Free image
    vpx_img_free(img);
    
    return 0;
}