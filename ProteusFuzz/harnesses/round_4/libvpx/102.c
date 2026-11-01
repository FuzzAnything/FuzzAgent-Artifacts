// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static vpx_codec_ctx_t *create_encoder(void) {
    vpx_codec_ctx_t *codec = malloc(sizeof(vpx_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(vpx_codec_ctx_t));
    
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(codec);
        return NULL;
    }

    vpx_codec_err_t err = vpx_codec_enc_init(codec, iface, NULL, 0);
    if (err != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    return codec;
}

static void cleanup_encoder(vpx_codec_ctx_t *codec) {
    if (codec) {
        vpx_codec_destroy(codec);
        free(codec);
    }
}

static vpx_codec_ctx_t *create_decoder(void) {
    vpx_codec_ctx_t *codec = malloc(sizeof(vpx_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(vpx_codec_ctx_t));
    
    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        free(codec);
        return NULL;
    }

    vpx_codec_err_t err = vpx_codec_dec_init(codec, iface, NULL, 0);
    if (err != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    return codec;
}

static void cleanup_decoder(vpx_codec_ctx_t *codec) {
    if (codec) {
        vpx_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
    const vpx_codec_iface_t *dec_iface = vpx_codec_vp8_dx();

    if (!enc_iface || !dec_iface) return 0;

    vpx_codec_ctx_t enc_ctx;
    vpx_codec_ctx_t dec_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    memset(&dec_ctx, 0, sizeof(dec_ctx));

    if (vpx_codec_enc_init(&enc_ctx, enc_iface, NULL, 0) != VPX_CODEC_OK) return 0;
    if (vpx_codec_dec_init(&dec_ctx, dec_iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&enc_ctx);
        return 0;
    }

    int32_t value = 0;
    if (Size >= sizeof(int32_t)) {
        memcpy(&value, Data, sizeof(int32_t));
        if (value > 1000) value = 1000;
        if (value < -1000) value = -1000;
    }

    // Re-initialize encoder with a proper config to allow setting options
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(enc_iface, &cfg, 0) == VPX_CODEC_OK) {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        
        // Destroy previous init and re-init with config
        vpx_codec_destroy(&enc_ctx);
        if (vpx_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != VPX_CODEC_OK) {
            vpx_codec_destroy(&dec_ctx);
            return 0;
        }

        // Prepare a dummy image buffer
        uint8_t *img_data = malloc(64 * 64 * 3 / 2);
        if (img_data) {
            memset(img_data, 0, 64 * 64 * 3 / 2);
            if (Size > 0) {
                size_t copy_size = Size < (64 * 64 * 3 / 2) ? Size : (64 * 64 * 3 / 2);
                memcpy(img_data, Data, copy_size);
            }
            
            vpx_image_t img;
            vpx_img_wrap(&img, VPX_IMG_FMT_I420, 64, 64, 1, img_data);
            img.planes[0] = img_data;
            img.planes[1] = img_data + 64 * 64;
            img.planes[2] = img_data + 64 * 64 + 32 * 32;
            img.stride[0] = 64;
            img.stride[1] = 32;
            img.stride[2] = 32;
            
            vpx_codec_encode(&enc_ctx, &img, 0, 1, 0, 0);
            
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&enc_ctx, &iter)) != NULL) {
                // Process packet (do nothing for fuzzing)
            }
            
            free(img_data);
        }
    }

    // Cleanup
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);

    return 0;
}