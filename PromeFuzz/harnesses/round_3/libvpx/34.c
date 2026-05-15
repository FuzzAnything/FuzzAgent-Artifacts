// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

static vpx_codec_ctx_t *init_decoder(void) {
    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    if (vpx_codec_dec_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static vpx_codec_ctx_t *init_encoder(void) {
    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static vpx_image_t *create_test_image(void) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 320, 240, 1);
    if (!img) return NULL;
    
    for (int plane = 0; plane < 3; plane++) {
        int stride = img->stride[plane];
        int height = (plane == 0) ? 240 : 120;
        for (int y = 0; y < height; y++) {
            memset(img->planes[plane] + y * stride, 128, stride);
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test vpx_codec_decode
    vpx_codec_ctx_t *decoder = init_decoder();
    if (decoder) {
        vpx_codec_decode(decoder, Data, Size, NULL, 0);
        
        // Test vpx_codec_error_detail after decode
        const char *error_detail = vpx_codec_error_detail(decoder);
        (void)error_detail;
        
        // Test vpx_codec_control_ with decoder
        int ctrl_id = Data[0] % 10;
        int value = Data[0];
        vpx_codec_control_(decoder, ctrl_id, value);
        
        vpx_codec_destroy(decoder);
        free(decoder);
    }
    
    // Test vpx_codec_encode and related functions
    vpx_codec_ctx_t *encoder = init_encoder();
    if (encoder) {
        vpx_image_t *img = create_test_image();
        if (img) {
            // Test vpx_codec_encode
            vpx_codec_encode(encoder, img, 0, 1, 0, 0);
            
            // Test vpx_codec_set_cx_data_buf
            vpx_fixed_buf_t buf;
            uint8_t buffer[1024];
            buf.buf = buffer;
            buf.sz = sizeof(buffer);
            vpx_codec_set_cx_data_buf(encoder, &buf, 0, 0);
            
            // Test vpx_codec_get_cx_data
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(encoder, &iter)) != NULL) {
                (void)pkt;
            }
            
            // Test vpx_codec_control_ with encoder
            int ctrl_id = Data[0] % 20;
            int value = Data[0];
            vpx_codec_control_(encoder, ctrl_id, value);
            
            vpx_img_free(img);
        }
        
        // Test vpx_codec_error_detail after encode
        const char *error_detail = vpx_codec_error_detail(encoder);
        (void)error_detail;
        
        vpx_codec_destroy(encoder);
        free(encoder);
    }
    
    return 0;
}