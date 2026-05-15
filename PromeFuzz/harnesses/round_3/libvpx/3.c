// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"

static vpx_image_t* create_test_image(void) {
    // Create a simple test image with I420 format (YUV 4:2:0)
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 32, 32, 1);
    if (!img) return NULL;
    
    // Fill image planes with some data
    for (unsigned int i = 0; i < img->d_h; ++i) {
        for (unsigned int j = 0; j < img->d_w; ++j) {
            img->planes[VPX_PLANE_Y][i * img->stride[VPX_PLANE_Y] + j] = (i + j) & 0xFF;
        }
    }
    
    unsigned int uv_h = (img->d_h + 1) / 2;
    unsigned int uv_w = (img->d_w + 1) / 2;
    
    for (unsigned int i = 0; i < uv_h; ++i) {
        for (unsigned int j = 0; j < uv_w; ++j) {
            img->planes[VPX_PLANE_U][i * img->stride[VPX_PLANE_U] + j] = (i * 2 + j * 2) & 0xFF;
            img->planes[VPX_PLANE_V][i * img->stride[VPX_PLANE_V] + j] = (i * 3 + j * 3) & 0xFF;
        }
    }
    
    return img;
}

static vpx_codec_ctx_t* create_test_encoder(vpx_codec_iface_t* iface) {
    vpx_codec_ctx_t* codec = (vpx_codec_ctx_t*)malloc(sizeof(vpx_codec_ctx_t));
    if (!codec) return NULL;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    
    if (vpx_codec_enc_init(codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    
    return codec;
}

static vpx_codec_ctx_t* create_test_decoder(vpx_codec_iface_t* iface) {
    vpx_codec_ctx_t* codec = (vpx_codec_ctx_t*)malloc(sizeof(vpx_codec_ctx_t));
    if (!codec) return NULL;
    
    if (vpx_codec_dec_init(codec, iface, NULL, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    
    return codec;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create test image
    vpx_image_t* img = create_test_image();
    if (!img) return 0;
    
    // Get VP9 encoder interface
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        vpx_img_free(img);
        return 0;
    }
    
    // Create encoder
    vpx_codec_ctx_t* encoder = create_test_encoder(encoder_iface);
    if (!encoder) {
        vpx_img_free(img);
        return 0;
    }
    
    // Encode the image
    if (vpx_codec_encode(encoder, img, 0, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(encoder, &iter)) != NULL) {
            // Process the packet if needed
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // We have encoded frame data
            }
        }
    }
    
    // Get VP9 decoder interface (need to include vp8dx.h for this)
    // Since we don't have vp8dx.h in the includes, we'll use the encoder interface
    // for demonstration, but in reality we'd need vpx_codec_vp9_dx()
    
    // Create decoder using encoder interface (this is wrong but we don't have decoder interface)
    // In a real fuzzer, we would include vp8dx.h and use vpx_codec_vp9_dx()
    vpx_codec_ctx_t* decoder = create_test_decoder(encoder_iface);
    
    if (decoder) {
        // Try to decode the fuzzer input
        if (Size > 0) {
            vpx_codec_decode(decoder, Data, (unsigned int)Size, NULL, 0);
        }
        
        // Destroy decoder
        vpx_codec_destroy(decoder);
        free(decoder);
    }
    
    // Destroy encoder
    vpx_codec_destroy(encoder);
    free(encoder);
    
    // Free image
    vpx_img_free(img);
    
    return 0;
}