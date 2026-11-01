// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required libvpx headers */
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>
#include <vpx/vpx_codec.h>

/* 
 * Note: The provided input indicates that the target API functions (VP9E_SET_...__value)
 * are actually control constants used with vpx_codec_control, not standalone functions.
 * However, the prompt asks to fuzz these specific identifiers. Since they are constants
 * representing control codes, we will construct a fuzz driver that attempts to invoke
 * vpx_codec_control with these constants using fuzzed data to populate the associated
 * structs (vpx_svc_frame_drop_t, vpx_svc_layer_id_t, etc.).
 *
 * We will initialize a VP9 encoder context to test these controls.
 * Note: The struct vpx_codec_enc_cfg might not have 'svc' or 'svc_layers' fields in all versions.
 * We will use conditional compilation or standard fields available in most versions.
 */

/* Helper to initialize a VP9 encoder context */
static vpx_codec_ctx_t *init_encoder(void) {
    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) {
        free(ctx);
        return NULL;
    }

    /* Initialize with minimal settings to allow SVC controls */
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) < 0) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    
    /* Enable SVC using the correct field name for newer libvpx versions */
    /* Check if svc field exists in vpx_codec_enc_cfg_t */
#ifdef VPX_CODEC_ENC_CFG_SVC
    cfg.svc = 1;
#endif
#ifdef VPX_CODEC_ENC_CFG_SVC_LAYERS
    cfg.svc_layers = 2;
#endif

    if (vpx_codec_enc_init(ctx, codec, &cfg, 0) < 0) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to cleanup encoder context */
static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

/* Helper to create a dummy image for encoding */
static vpx_image_t *create_dummy_image(void) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 160, 160, 16);
    if (!img) return NULL;

    /* Fill with pattern */
    memset(img->planes[0], 0x55, img->d_w * img->d_h);
    memset(img->planes[1], 0xAA, (img->d_w * img->d_h) / 4);
    memset(img->planes[2], 0xAA, (img->d_w * img->d_h) / 4);

    return img;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t *ctx = init_encoder();
    if (!ctx) return 0;

    vpx_svc_frame_drop_t frame_drop = {0};
    vpx_svc_layer_id_t layer_id = {0};
    int svc_mode = 0;
    int inter_layer_pred = 0;
    int spatial_layer = 0;

    /* Parse fuzzed data to populate structures */
    int i;
    for (i = 0; i < VPX_SS_MAX_LAYERS && i < Size; i++) {
        frame_drop.framedrop_thresh[i] = Data[i];
        layer_id.temporal_layer_id_per_spatial[i] = Data[i];
    }

    if (Size > 0) frame_drop.framedrop_mode = Data[0] % 2;
    if (Size > 1) frame_drop.max_consec_drop = Data[1];
    if (Size > 2) layer_id.spatial_layer_id = Data[2];
    if (Size > 3) layer_id.temporal_layer_id = Data[3];
    if (Size > 4) svc_mode = Data[4];
    if (Size > 5) inter_layer_pred = Data[5];
    if (Size > 6) spatial_layer = Data[6];

    /* 
     * Attempt to invoke controls.
     * Note: The "Target API Functions" listed in the prompt are actually control constants.
     * We use vpx_codec_control to invoke them.
     * We check if the constants are defined before using them.
     */

    /* 1. VP9E_SET_SVC_FRAME_DROP */
#ifdef VP9E_SET_SVC_FRAME_DROP
    if (vpx_codec_control(ctx, VP9E_SET_SVC_FRAME_DROP, &frame_drop) < 0) {
        /* Ignore errors, as fuzzing explores invalid states too */
    }
#endif

    /* 2. VP9E_SET_SVC_LAYER_ID */
#ifdef VP9E_SET_SVC_LAYER_ID
    if (vpx_codec_control(ctx, VP9E_SET_SVC_LAYER_ID, &layer_id) < 0) {
        /* Ignore errors */
    }
#endif

    /* 3. VP9E_SET_SVC */
#ifdef VP9E_SET_SVC
    if (vpx_codec_control(ctx, VP9E_SET_SVC, &svc_mode) < 0) {
        /* Ignore errors */
    }
#endif

    /* 4. VP9E_SET_SVC_PARAMETERS */
#ifdef VP9E_SET_SVC_PARAMETERS
    if (vpx_codec_control(ctx, VP9E_SET_SVC_PARAMETERS, &frame_drop) < 0) {
        /* Ignore errors */
    }
#endif

    /* 5. VP9_DECODE_SVC_SPATIAL_LAYER */
#ifdef VP9_DECODE_SVC_SPATIAL_LAYER
    if (vpx_codec_control(ctx, VP9_DECODE_SVC_SPATIAL_LAYER, &spatial_layer) < 0) {
        /* Ignore errors */
    }
#endif

    /* 6. VP9E_SET_SVC_INTER_LAYER_PRED */
#ifdef VP9E_SET_SVC_INTER_LAYER_PRED
    if (vpx_codec_control(ctx, VP9E_SET_SVC_INTER_LAYER_PRED, &inter_layer_pred) < 0) {
        /* Ignore errors */
    }
#endif

    /* Try to encode a frame to ensure the context is still valid after controls */
    vpx_image_t *img = create_dummy_image();
    if (img) {
        vpx_codec_encode(ctx, img, 0, 1, 0, 0);
        vpx_img_free(img);
    }

    cleanup_encoder(ctx);
    return 0;
}