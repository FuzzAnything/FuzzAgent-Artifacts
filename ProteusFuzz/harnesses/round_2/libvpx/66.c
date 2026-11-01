// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx_decoder.h"
#include "vpx_encoder.h"
#include "vp8cx.h"
#include "vpx_codec.h"
#include "vp8dx.h"

static void init_decoder_context(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface) {
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(ctx, iface, &cfg, 0, VPX_CODEC_ABI_VERSION) != VPX_CODEC_OK) {
        return;
    }
}

static void init_encoder_context(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface) {
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_target_bitrate = 1000;
    
    if (vpx_codec_enc_config_set(ctx, &cfg) != VPX_CODEC_OK) {
        return;
    }
    
    if (vpx_codec_enc_init_ver(ctx, iface, &cfg, 0, VPX_CODEC_ABI_VERSION) != VPX_CODEC_OK) {
        return;
    }
}

static void cleanup_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Determine which codec to use based on input data pattern
    // Even byte -> Decoder, Odd byte -> Encoder
    int use_decoder = (Data[0] % 2 == 0);
    
    const vpx_codec_iface_t *iface = NULL;
    
    if (use_decoder) {
        iface = vpx_codec_vp8_dx();
    } else {
        iface = vpx_codec_vp8_cx();
    }

    if (!iface) {
        return 0;
    }

    if (use_decoder) {
        init_decoder_context(&ctx, iface);
    } else {
        init_encoder_context(&ctx, iface);
    }

    if (ctx.err != VPX_CODEC_OK) {
        cleanup_context(&ctx);
        return 0;
    }

    // Fuzz vpx_codec_error
    const char *err_str = vpx_codec_error(&ctx);
    (void)err_str; // Suppress unused warning

    // Fuzz vpx_codec_decode
    if (use_decoder) {
        vpx_codec_err_t res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
        (void)res;
        
        // Fuzz vpx_codec_get_frame
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            // Do nothing with the image, just iterate
        }
    }

    // Fuzz vpx_codec_encode
    if (!use_decoder) {
        // Prepare a dummy image for encoding
        vpx_image_t img;
        memset(&img, 0, sizeof(img));
        img.fmt = VPX_IMG_FMT_I420;
        img.w = 64;
        img.h = 64;
        img.d_w = 64;
        img.d_h = 64;
        img.bit_depth = 8;
        
        // Allocate planes
        int y_size = 64 * 64;
        int uv_size = 32 * 32;
        img.planes[0] = malloc(y_size);
        img.planes[1] = malloc(uv_size);
        img.planes[2] = malloc(uv_size);
        img.stride[0] = 64;
        img.stride[1] = 32;
        img.stride[2] = 32;
        
        if (img.planes[0] && img.planes[1] && img.planes[2]) {
            // Fill with fuzz data if available, else zeros
            if (Size >= y_size) {
                memcpy(img.planes[0], Data, y_size);
            } else {
                memset(img.planes[0], 0, y_size);
            }
            if (Size >= uv_size) {
                memcpy(img.planes[1], Data + y_size, uv_size);
                memcpy(img.planes[2], Data + y_size + uv_size, uv_size);
            } else {
                memset(img.planes[1], 0, uv_size);
                memset(img.planes[2], 0, uv_size);
            }

            vpx_codec_pts_t pts = 0;
            vpx_codec_err_t res = vpx_codec_encode(&ctx, &img, pts, 1, 0, 0);
            (void)res;

            // Fuzz vpx_codec_get_cx_data
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                // Just iterate
            }
            
            // Flush encoder
            res = vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
            (void)res;
            
            // Get remaining packets
            iter = NULL;
            while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                // Just iterate
            }

            free(img.planes[0]);
            free(img.planes[1]);
            free(img.planes[2]);
        }
    }

    // Fuzz vpx_codec_get_preview_frame (only valid for encoder)
    if (!use_decoder) {
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
        (void)preview;
    }

    cleanup_context(&ctx);
    return 0;
}