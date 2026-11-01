// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_RENDER_SIZE__value at vp8cx.h:1062:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/* Helper to create a valid encoder context */
static vpx_codec_ctx_t *create_encoder_context(void) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_end_usage = VPX_CBR;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

/* Helper to cleanup encoder context */
static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t *ctx = create_encoder_context();
    if (!ctx) return 0;

    int ret;
    int cmd;
    int value;
    vpx_svc_frame_drop_t frame_drop;
    vpx_svc_layer_id_t layer_id;

    /* Extract control command and value from input data */
    cmd = Data[0] % 6; /* 0 to 5 corresponding to the 6 APIs */
    value = (Size > 1) ? Data[1] : 0;

    /* Initialize structures with safe defaults */
    memset(&frame_drop, 0, sizeof(frame_drop));
    memset(&layer_id, 0, sizeof(layer_id));

    /* Sanitize frame_drop fields */
    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        frame_drop.framedrop_thresh[i] = 0;
    }
    frame_drop.framedrop_mode = 0;
    frame_drop.max_consec_drop = 0;

    /* Sanitize layer_id fields */
    layer_id.spatial_layer_id = 0;
    layer_id.temporal_layer_id = 0;
    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        layer_id.temporal_layer_id_per_spatial[i] = 0;
    }

    /* Dispatch to the target API based on cmd */
    switch (cmd) {
        case 0: /* VP9E_SET_SVC */
            /* Typically sets SVC mode (0 or 1) */
            ret = vpx_codec_control(ctx, VP9E_SET_SVC, value & 1);
            break;

        case 1: /* VP9E_SET_SVC_LAYER_ID */
            ret = vpx_codec_control(ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
            break;

        case 2: /* VP9E_SET_SVC_FRAME_DROP_LAYER */
            ret = vpx_codec_control(ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
            break;

        case 3: /* VP9E_SET_SVC_INTER_LAYER_PRED */
            /* Typically sets inter-layer prediction mode (0 or 1) */
            ret = vpx_codec_control(ctx, VP9E_SET_SVC_INTER_LAYER_PRED, value & 1);
            break;

        case 4: /* VP9E_SET_RENDER_SIZE */
            /* Sets render size (width, height). Requires an int array of size 2. */
            {
                int render_size[2];
                int width = (Size > 2) ? (Data[2] * 8 + (Size > 3 ? Data[3] : 0)) : 32;
                int height = (Size > 4) ? (Data[4] * 8 + (Size > 5 ? Data[5] : 0)) : 32;
                
                /* Sanitize dimensions */
                if (width <= 0) width = 32;
                if (height <= 0) height = 32;
                if (width > 4096) width = 4096;
                if (height > 4096) height = 4096;
                
                render_size[0] = width;
                render_size[1] = height;
                
                ret = vpx_codec_control(ctx, VP9E_SET_RENDER_SIZE, render_size);
            }
            break;

        case 5: /* VP9E_SET_SVC_PARAMETERS */
            /* 
             * VP9E_SET_SVC_PARAMETERS expects a vpx_svc_parameters_t struct, 
             * NOT vpx_svc_frame_drop_t. Passing the wrong struct caused the ASAN error.
             * Since we don't have the full definition of vpx_svc_parameters_t in the prompt,
             * and passing an incomplete/wrong struct causes the crash, we skip this specific
             * control or use a dummy valid structure if we can infer it.
             * However, to be safe and avoid the crash, we will skip this command 
             * unless we can construct a valid vpx_svc_parameters_t.
             * Given the constraints, we will simply not invoke this specific control 
             * to prevent the buffer overflow, as the prompt's provided types are insufficient 
             * to construct the correct struct for this specific API.
             */
            ret = 0; 
            break;

        default:
            ret = 0;
            break;
    }

    /* Handle return value */
    (void)ret;

    cleanup_encoder_context(ctx);
    return 0;
}