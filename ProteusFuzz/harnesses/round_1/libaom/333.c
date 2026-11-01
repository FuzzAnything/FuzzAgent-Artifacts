// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void free_image(aom_image_t *img) {
    if (img == NULL) return;
    if (img->planes[0] != NULL && img->img_data != NULL) {
        aom_img_free(img);
    }
}

static aom_codec_ctx_t *init_encoder(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Safely use fuzzing input to vary configuration, checking bounds
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;

    if (Size > 0) {
        cfg.g_w = (Data[0] % 512) + 16;
    }
    if (Size > 1) {
        cfg.g_h = (Data[1] % 512) + 16;
    }
    if (Size > 2) {
        cfg.g_timebase.num = (Data[2] % 30) + 1;
    }
    if (Size > 3) {
        cfg.g_timebase.den = (Data[3] % 30) + 1;
    }
    if (Size > 4) {
        cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    }
    if (Size > 5) {
        cfg.g_profile = Data[5] % 4;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx == NULL) return;
    aom_codec_destroy(ctx);
    free(ctx);
}

int LLVMFuzzerTestOneInput_333(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (ctx == NULL) return 0;

    // Create a valid image for encoding
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                  ctx->config.enc->g_w, ctx->config.enc->g_h, 16);
    
    if (img.planes[0] == NULL) {
        cleanup_encoder(ctx);
        return 0;
    }

    // Fill image planes with fuzzing data safely
    size_t plane_size = img.stride[0] * img.h;
    size_t copy_size = (Size < plane_size) ? Size : plane_size;
    memset(img.planes[0], 0, plane_size);
    if (copy_size > 0) {
        memcpy(img.planes[0], Data, copy_size);
    }

    // Initialize PTS and duration
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;

    // Encode a few frames with varying parameters
    for (int i = 0; i < 5 && Size > 0; i++) {
        // Vary flags and duration based on input safely
        aom_enc_frame_flags_t flags = 0;
        unsigned long dur = 1;
        
        if (Size > (size_t)(i % Size + 1)) {
             flags = (Data[i % Size] & 0x0F);
             dur = (Data[(i + 1) % Size] % 100) + 1;
        }
        
        aom_codec_encode(ctx, &img, pts, dur, flags);
        
        pts += dur;
    }

    // Flush the encoder
    while (1) {
        aom_codec_err_t res = aom_codec_encode(ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;
        
        aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(ctx, NULL);
        if (pkt == NULL) break;
        
        // Consume the packet to avoid memory leak
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            aom_codec_iter_t iter = NULL;
            while (aom_codec_get_cx_data(ctx, &pkt) != NULL) {
                // Just consume
            }
        }
    }

    free_image(&img);
    cleanup_encoder(ctx);
    
    return 0;
}