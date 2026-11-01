// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vp8cx.h"
#include "vp8dx.h"

static vpx_codec_ctx_t codec_ctx;
static int codec_initialized = 0;

static int init_codec(void) {
    if (codec_initialized) {
        return 0;
    }

    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) {
        return -1;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) < 0) {
        return -1;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_pass = 0;
    cfg.g_usage = VPX_CBR;
    cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;

    if (vpx_codec_enc_init(&codec_ctx, codec, &cfg, 0) < 0) {
        return -1;
    }

    codec_initialized = 1;
    return 0;
}

static void cleanup_codec(void) {
    if (codec_initialized) {
        vpx_codec_destroy(&codec_ctx);
        codec_initialized = 0;
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    if (!codec_initialized) {
        if (init_codec() < 0) {
            return 0;
        }
    }

    uint8_t mode = Data[0] % 4;

    switch (mode) {
        case 0: {
            // Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
            vpx_svc_frame_drop_t drop_cfg;
            memset(&drop_cfg, 0, sizeof(drop_cfg));
            
            if (Size > 1) drop_cfg.framedrop_mode = (SVC_LAYER_DROP_MODE)(Data[1] % 2);
            if (Size > 2) drop_cfg.max_consec_drop = (int)(Data[2] % 100);
            
            for (int i = 0; i < VPX_SS_MAX_LAYERS && i < (int)(Size - 3); i++) {
                drop_cfg.framedrop_thresh[i] = (int)Data[3 + i];
            }

            vpx_codec_control(&codec_ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_cfg);
            break;
        }
        case 1: {
            // Fuzz VP9E_SET_SVC_LAYER_ID
            vpx_svc_layer_id_t layer_id;
            memset(&layer_id, 0, sizeof(layer_id));

            if (Size > 1) layer_id.spatial_layer_id = (int)Data[1] % 4;
            if (Size > 2) layer_id.temporal_layer_id = (int)Data[2] % 8;
            
            for (int i = 0; i < VPX_SS_MAX_LAYERS && i < (int)(Size - 3); i++) {
                layer_id.temporal_layer_id_per_spatial[i] = (int)Data[3 + i] % 8;
            }

            vpx_codec_control(&codec_ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
            break;
        }
        case 2: {
            // Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG
            vpx_svc_ref_frame_config_t ref_cfg;
            memset(&ref_cfg, 0, sizeof(ref_cfg));

            int offset = 1;
            for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
                if (offset < Size) ref_cfg.lst_fb_idx[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.gld_fb_idx[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.alt_fb_idx[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.update_buffer_slot[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.update_last[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.update_golden[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.update_alt_ref[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.reference_last[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.reference_golden[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.reference_alt_ref[i] = (int)Data[offset++];
                if (offset < Size) ref_cfg.duration[i] = (int64_t)Data[offset++];
            }

            vpx_codec_control(&codec_ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);
            break;
        }
        case 3: {
            // Fuzz VP9E_SET_SVC
            int svc_mode = (int)Data[0] % 2;
            // VP9E_SET_SVC expects an int, not a pointer to int
            vpx_codec_control(&codec_ctx, VP9E_SET_SVC, svc_mode);
            break;
        }
    }

    return 0;
}