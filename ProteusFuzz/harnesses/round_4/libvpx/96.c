// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// VP9_DECODE_SVC_SPATIAL_LAYER__value at vp8dx.h:212:1 in vp8dx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>

static int fuzzer_initialize_encoder(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface) {
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.kf_mode = VPX_KF_AUTO;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }
    return 0;
}

static int fuzzer_initialize_decoder(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface) {
    if (vpx_codec_dec_init(ctx, iface, NULL, 0) != VPX_CODEC_OK) {
        return -1;
    }
    return 0;
}

static void fuzzer_cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

static void fuzzer_cleanup_decoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t enc_ctx = {0};
    vpx_codec_ctx_t dec_ctx = {0};
    
    // Use the standard API to get the interfaces instead of referencing global symbols
    vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();

    if (!enc_iface || !dec_iface) return 0;

    if (fuzzer_initialize_encoder(&enc_ctx, enc_iface) != 0) return 0;
    if (fuzzer_initialize_decoder(&dec_ctx, dec_iface) != 0) {
        fuzzer_cleanup_encoder(&enc_ctx);
        return 0;
    }

    uint32_t control_type = *(uint32_t *)Data;
    uint8_t *payload = (uint8_t *)(Data + 4);
    size_t payload_size = Size - 4;

    int ret = 0;

    switch (control_type) {
        case VP9E_SET_SVC: {
            // VP9E_SET_SVC expects an int (0 or 1) to enable/disable SVC
            if (payload_size >= sizeof(int)) {
                int val = *(int *)payload;
                // Clamp to 0 or 1
                val = (val != 0) ? 1 : 0;
                ret = vpx_codec_control(&enc_ctx, VP9E_SET_SVC, val);
            }
            break;
        }
        case VP9E_SET_SVC_INTER_LAYER_PRED: {
            // Typically expects an integer value (0 or 1)
            if (payload_size >= sizeof(int)) {
                int val = *(int *)payload;
                val = (val != 0) ? 1 : 0;
                ret = vpx_codec_control(&enc_ctx, VP9E_SET_SVC_INTER_LAYER_PRED, val);
            }
            break;
        }
        case VP9E_SET_SVC_FRAME_DROP_LAYER: {
            if (payload_size >= sizeof(vpx_svc_frame_drop_t)) {
                vpx_svc_frame_drop_t drop_cfg;
                memcpy(&drop_cfg, payload, sizeof(vpx_svc_frame_drop_t));
                // Sanitize thresholds
                for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
                    if (drop_cfg.framedrop_thresh[i] < 0) drop_cfg.framedrop_thresh[i] = 0;
                    if (drop_cfg.framedrop_thresh[i] > 1000) drop_cfg.framedrop_thresh[i] = 1000;
                }
                if (drop_cfg.max_consec_drop < 0) drop_cfg.max_consec_drop = 0;
                if (drop_cfg.framedrop_mode < 0) drop_cfg.framedrop_mode = 0;
                if (drop_cfg.framedrop_mode > 1) drop_cfg.framedrop_mode = 1;
                
                ret = vpx_codec_control(&enc_ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_cfg);
            }
            break;
        }
        case VP9E_SET_SVC_PARAMETERS: {
            // This control usually takes a vpx_svc_layer_id_t or similar
            if (payload_size >= sizeof(vpx_svc_layer_id_t)) {
                vpx_svc_layer_id_t layer_id;
                memcpy(&layer_id, payload, sizeof(vpx_svc_layer_id_t));
                // Sanitize IDs
                if (layer_id.spatial_layer_id < 0) layer_id.spatial_layer_id = 0;
                if (layer_id.spatial_layer_id > 3) layer_id.spatial_layer_id = 3;
                if (layer_id.temporal_layer_id < 0) layer_id.temporal_layer_id = 0;
                if (layer_id.temporal_layer_id > 7) layer_id.temporal_layer_id = 7;
                for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
                    if (layer_id.temporal_layer_id_per_spatial[i] < 0) layer_id.temporal_layer_id_per_spatial[i] = 0;
                    if (layer_id.temporal_layer_id_per_spatial[i] > 7) layer_id.temporal_layer_id_per_spatial[i] = 7;
                }
                ret = vpx_codec_control(&enc_ctx, VP9E_SET_SVC_PARAMETERS, &layer_id);
            }
            break;
        }
        case VP9E_SET_SVC_LAYER_ID: {
            if (payload_size >= sizeof(vpx_svc_layer_id_t)) {
                vpx_svc_layer_id_t layer_id;
                memcpy(&layer_id, payload, sizeof(vpx_svc_layer_id_t));
                // Sanitize
                if (layer_id.spatial_layer_id < 0) layer_id.spatial_layer_id = 0;
                if (layer_id.spatial_layer_id > 3) layer_id.spatial_layer_id = 3;
                if (layer_id.temporal_layer_id < 0) layer_id.temporal_layer_id = 0;
                if (layer_id.temporal_layer_id > 7) layer_id.temporal_layer_id = 7;
                ret = vpx_codec_control(&enc_ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
            }
            break;
        }
        case VP9_DECODE_SVC_SPATIAL_LAYER: {
            // Decoder control, expects an int
            if (payload_size >= sizeof(int)) {
                int layer = *(int *)payload;
                if (layer < 0) layer = 0;
                if (layer > 3) layer = 3;
                ret = vpx_codec_control(&dec_ctx, VP9_DECODE_SVC_SPATIAL_LAYER, layer);
            }
            break;
        }
        default:
            break;
    }

    // Cleanup
    fuzzer_cleanup_encoder(&enc_ctx);
    fuzzer_cleanup_decoder(&dec_ctx);

    return 0;
}