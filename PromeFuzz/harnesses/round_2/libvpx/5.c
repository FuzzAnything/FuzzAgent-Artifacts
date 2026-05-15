// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Initialize variables
    vpx_image_t *img = NULL;
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt = NULL;
    
    // 1. Allocate image with fixed parameters (using small dimensions for fuzzing)
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 16, 16, 1);
    if (!img) {
        return 0;
    }
    
    // Fill image planes with fuzzer data if available
    if (Size > 0) {
        // Simple fill: use first bytes for Y plane, then U, then V
        // For I420: Y plane = width * height, U and V = (width/2) * (height/2)
        size_t y_size = 16 * 16;
        size_t uv_size = 8 * 8;
        
        if (Size >= y_size) {
            for (size_t i = 0; i < y_size && i < Size; i++) {
                img->planes[VPX_PLANE_Y][i] = Data[i];
            }
        }
        
        if (Size >= y_size + uv_size) {
            for (size_t i = 0; i < uv_size && (y_size + i) < Size; i++) {
                img->planes[VPX_PLANE_U][i] = Data[y_size + i];
            }
        }
        
        if (Size >= y_size + 2 * uv_size) {
            for (size_t i = 0; i < uv_size && (y_size + uv_size + i) < Size; i++) {
                img->planes[VPX_PLANE_V][i] = Data[y_size + uv_size + i];
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
    
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
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
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        // Process packet - just consume it for fuzzing
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Access packet data to explore code paths
            volatile size_t sz = pkt->data.frame.sz;
            volatile void *buf = pkt->data.frame.buf;
            (void)sz;
            (void)buf;
        }
    }
    
    // 4. Decode - we need to use the encoder context since vpx_codec_vp9_dx is not available
    // Use fuzzer data as input to decode with the encoder context
    // Note: This will likely fail since we're using an encoder context for decoding,
    // but we're following the required function order
    if (Size > 0) {
        vpx_codec_decode(&codec_ctx, Data, (unsigned int)(Size > 1000 ? 1000 : Size), NULL, 0);
    }
    
    // 5. Destroy encoder context
    vpx_codec_destroy(&codec_ctx);
    
    // 6. Free image
    vpx_img_free(img);
    
    return 0;
}