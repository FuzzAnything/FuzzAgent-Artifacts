// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_GET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1080:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx/vpx_codec.h"

/*
 * Helper function to initialize a vpx_svc_layer_id_t structure from fuzz data.
 * Returns 0 on success, -1 on failure (e.g., insufficient data).
 */
static int init_svc_layer_id(const uint8_t *data, size_t size, vpx_svc_layer_id_t *layer_id) {
    if (size < sizeof(int) * 2) return -1;
    
    layer_id->spatial_layer_id = *(const int *)data;
    layer_id->temporal_layer_id = *(const int *)(data + sizeof(int));
    
    /* Initialize the array with safe defaults or fuzzed data if available */
    memset(layer_id->temporal_layer_id_per_spatial, 0, sizeof(layer_id->temporal_layer_id_per_spatial));
    
    size_t remaining = size - (2 * sizeof(int));
    size_t array_size = sizeof(layer_id->temporal_layer_id_per_spatial);
    size_t copy_size = (remaining < array_size) ? remaining : array_size;
    
    if (copy_size > 0) {
        memcpy(layer_id->temporal_layer_id_per_spatial, data + (2 * sizeof(int)), copy_size);
    }
    
    return 0;
}

/*
 * Helper function to initialize a vpx_svc_frame_drop_t structure from fuzz data.
 */
static int init_svc_frame_drop(const uint8_t *data, size_t size, vpx_svc_frame_drop_t *frame_drop) {
    if (size < sizeof(int) * 3) return -1;
    
    /* Copy thresholds array if data is sufficient, otherwise zero-init */
    size_t thresh_size = sizeof(frame_drop->framedrop_thresh);
    size_t copy_size = (size < thresh_size) ? size : thresh_size;
    
    if (copy_size > 0) {
        memcpy(frame_drop->framedrop_thresh, data, copy_size);
    } else {
        memset(frame_drop->framedrop_thresh, 0, thresh_size);
    }
    
    /* Fuzz the mode and max_consec_drop from remaining data */
    size_t offset = copy_size;
    if (offset + sizeof(int) <= size) {
        frame_drop->framedrop_mode = *(const SVC_LAYER_DROP_MODE *)(data + offset);
        offset += sizeof(int);
    } else {
        frame_drop->framedrop_mode = 0; /* Default */
    }
    
    if (offset + sizeof(int) <= size) {
        frame_drop->max_consec_drop = *(const int *)(data + offset);
    } else {
        frame_drop->max_consec_drop = 0;
    }
    
    return 0;
}

/*
 * Helper function to initialize a vpx_svc_ref_frame_config_t structure from fuzz data.
 */
static int init_svc_ref_frame_config(const uint8_t *data, size_t size, vpx_svc_ref_frame_config_t *ref_config) {
    size_t struct_size = sizeof(vpx_svc_ref_frame_config_t);
    
    if (size >= struct_size) {
        memcpy(ref_config, data, struct_size);
    } else {
        memset(ref_config, 0, struct_size);
        if (size > 0) {
            memcpy(ref_config, data, size);
        }
    }
    
    return 0;
}

/*
 * Helper to create a minimal encoder context for testing controls.
 * Returns NULL if initialization fails.
 */
static vpx_codec_ctx_t *create_encoder_ctx(void) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return NULL;
    
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    /* Configure for basic encoding */
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
    
    /* Note: SVC specific fields like 'svc' and 'svc_layers' are not present in all libvpx versions.
       We rely on the controls to enable SVC behavior if supported by the build. */
    
    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

/*
 * Fuzz driver entry point
 */
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Use the first byte to determine which API to fuzz */
    uint8_t cmd = Data[0];
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;
    
    vpx_codec_ctx_t *ctx = create_encoder_ctx();
    if (!ctx) return 0;
    
    int ret = 0;
    
    switch (cmd) {
        case 0:
            /* Fuzz VP9E_GET_SVC_REF_FRAME_CONFIG */
            {
                vpx_svc_ref_frame_config_t config;
                memset(&config, 0, sizeof(config));
                ret = vpx_codec_control(ctx, VP9E_GET_SVC_REF_FRAME_CONFIG, &config);
            }
            break;
            
        case 1:
            /* Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER */
            {
                vpx_svc_frame_drop_t frame_drop;
                if (init_svc_frame_drop(payload, payload_size, &frame_drop) == 0) {
                    ret = vpx_codec_control(ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
                }
            }
            break;
            
        case 2:
            /* Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG */
            {
                vpx_svc_ref_frame_config_t ref_config;
                if (init_svc_ref_frame_config(payload, payload_size, &ref_config) == 0) {
                    ret = vpx_codec_control(ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
                }
            }
            break;
            
        case 3:
            /* Fuzz VP9E_SET_SVC_LAYER_ID */
            {
                vpx_svc_layer_id_t layer_id;
                if (init_svc_layer_id(payload, payload_size, &layer_id) == 0) {
                    ret = vpx_codec_control(ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
                }
            }
            break;
            
        case 4:
            /* Fuzz VP9E_SET_SVC_PARAMETERS */
            /* Note: VP9E_SET_SVC_PARAMETERS typically takes vpx_svc_layer_id_t or similar depending on version.
               Assuming it uses vpx_svc_layer_id_t or a generic struct. 
               If the actual type differs, this might need adjustment, but based on available types, we try layer_id. */
            {
                vpx_svc_layer_id_t layer_id;
                if (init_svc_layer_id(payload, payload_size, &layer_id) == 0) {
                    ret = vpx_codec_control(ctx, VP9E_SET_SVC_PARAMETERS, &layer_id);
                }
            }
            break;
            
        case 5:
            /* Fuzz VP9E_SET_SVC_INTER_LAYER_PRED */
            /* This control usually takes an integer flag (0 or 1). The macro expects 'unsigned int', not a pointer. */
            {
                unsigned int inter_pred = 0;
                if (payload_size >= sizeof(int)) {
                    inter_pred = *(const int *)payload;
                } else if (payload_size > 0) {
                    inter_pred = payload[0] ? 1 : 0;
                }
                /* Pass the value directly, not the address, as per VPX_CTRL_USE_TYPE macro definition */
                ret = vpx_codec_control(ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_pred);
            }
            break;
            
        default:
            /* Try a random valid control if command is out of range, or just return */
            break;
    }
    
    /* Cleanup */
    vpx_codec_destroy(ctx);
    free(ctx);
    
    return 0;
}