// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required libvpx headers */
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>

/* 
 * Helper to initialize a codec context 
 */
static vpx_codec_ctx_t *init_encoder(vpx_codec_iface_t *iface, int width, int height, int fps) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fps;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 56;
    cfg.g_lag_in_frames = 0;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

/* Helper to cleanup codec context */
static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

/* 
 * Fuzz driver entry point
 */
int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    /* Extract parameters from input data */
    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(seed));
    
    int width = (seed % 1280) + 16;
    int height = ((seed >> 8) % 1280) + 16;
    int fps = ((seed >> 16) % 60) + 1;
    
    /* Ensure dimensions are multiples of 16 (common requirement) */
    width = (width / 16) * 16;
    height = (height / 16) * 16;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    /* Try VP8 Encoder */
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    if (iface_vp8) {
        vpx_codec_ctx_t *ctx = init_encoder(iface_vp8, width, height, fps);
        if (ctx) {
            /* 
             * Attempt to fuzz control parameters corresponding to the requested APIs.
             * Using vpx_codec_control_ (the actual function name) instead of the macro-like name.
             */
            
            /* 1. VP8E_SET_MAX_INTRA_BITRATE_PCT */
            int max_intra_pct = (seed >> 24) % 100;
            vpx_codec_control_(ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);

            /* 3. VP8E_SET_NOISE_SENSITIVITY */
            int noise_sens = (seed >> 28) % 16;
            vpx_codec_control_(ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sens);

            /* 4. VP8E_SET_SCREEN_CONTENT_MODE */
            int screen_mode = (seed >> 30) % 2;
            vpx_codec_control_(ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_mode);

            /* 2. VP8_SET_POSTPROC - This usually requires a struct */
            /* We construct a config from the input data if enough size exists */
            if (Size >= sizeof(vp8_postproc_cfg_t)) {
                vp8_postproc_cfg_t pp_cfg;
                memcpy(&pp_cfg, Data + sizeof(seed), sizeof(vp8_postproc_cfg_t));
                /* Clamp values to valid ranges as per struct comments */
                pp_cfg.deblocking_level = pp_cfg.deblocking_level % 17;
                pp_cfg.noise_level = pp_cfg.noise_level % 17;
                /* Note: VP8_SET_POSTPROC is often a decoder side or internal call. 
                 * We attempt to pass it if the API allows. If it's not a valid control 
                 * for the encoder context, the function will return an error, which is safe.
                 */
                vpx_codec_control_(ctx, VP8_SET_POSTPROC, &pp_cfg);
            }

            /* Encode a dummy frame to exercise the state */
            vpx_image_t img;
            vpx_img_alloc(&img, VPX_IMG_FMT_I420, width, height, 1);
            /* Fill image with fuzz data */
            size_t plane_size = Size - sizeof(seed);
            if (plane_size > 0) {
                for (int i = 0; i < 3; i++) {
                    /* Fix: Use 'planes' instead of 'plane' */
                    size_t copy_len = (plane_size / 3) > img.planes[i][0] ? img.planes[i][0] : (plane_size / 3);
                    memcpy(img.planes[i], Data + sizeof(seed), copy_len);
                }
            }
            
            vpx_codec_encode(ctx, &img, 0, 1, 0, 0);
            vpx_img_free(&img);

            cleanup_encoder(ctx);
        }
    }

    /* Try VP9 Encoder */
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
    if (iface_vp9) {
        vpx_codec_ctx_t *ctx = init_encoder(iface_vp9, width, height, fps);
        if (ctx) {
            /* 5. VP9E_SET_FRAME_PARALLEL_DECODING */
            int frame_parallel = (seed >> 24) % 2;
            vpx_codec_control_(ctx, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel);

            /* 6. VP9E_SET_NOISE_SENSITIVITY */
            int vp9_noise = (seed >> 28) % 16;
            vpx_codec_control_(ctx, VP9E_SET_NOISE_SENSITIVITY, vp9_noise);

            /* Encode a dummy frame */
            vpx_image_t img;
            vpx_img_alloc(&img, VPX_IMG_FMT_I420, width, height, 1);
            size_t plane_size = Size - sizeof(seed);
            if (plane_size > 0) {
                for (int i = 0; i < 3; i++) {
                    /* Fix: Use 'planes' instead of 'plane' */
                    size_t copy_len = (plane_size / 3) > img.planes[i][0] ? img.planes[i][0] : (plane_size / 3);
                    memcpy(img.planes[i], Data + sizeof(seed), copy_len);
                }
            }

            vpx_codec_encode(ctx, &img, 0, 1, 0, 0);
            vpx_img_free(&img);

            cleanup_encoder(ctx);
        }
    }

    return 0;
}