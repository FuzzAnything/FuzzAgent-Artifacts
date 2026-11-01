// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_GF_TEMPORAL_REF__value at vp8cx.h:1082:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

static int32_t read_int(const uint8_t **ptr, size_t *len) {
    if (*len < sizeof(int32_t)) return 0;
    int32_t val = 0;
    memcpy(&val, *ptr, sizeof(int32_t));
    *ptr += sizeof(int32_t);
    *len -= sizeof(int32_t);
    return val;
}

static int fuzz_vp9_encoder_controls(vpx_codec_ctx_t *encoder, const uint8_t *data, size_t size) {
    if (!encoder) return 0;

    const uint8_t *ptr = data;
    size_t len = size;
    int ret = 0;

    // Fuzz VP9E_SET_SVC
    if (len >= sizeof(int32_t)) {
        int svc_val = read_int(&ptr, &len);
        ret = vpx_codec_control(encoder, VP9E_SET_SVC, svc_val);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_INTER_LAYER_PRED
    if (len >= sizeof(int32_t)) {
        int pred_val = read_int(&ptr, &len);
        ret = vpx_codec_control(encoder, VP9E_SET_SVC_INTER_LAYER_PRED, pred_val);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
    if (len >= sizeof(vpx_svc_frame_drop_t)) {
        vpx_svc_frame_drop_t drop_params;
        memset(&drop_params, 0, sizeof(drop_params));
        memcpy(&drop_params, ptr, sizeof(vpx_svc_frame_drop_t));
        ptr += sizeof(vpx_svc_frame_drop_t);
        len -= sizeof(vpx_svc_frame_drop_t);
        
        // Ensure enums are within reasonable bounds if possible
        if (drop_params.framedrop_mode > 1) drop_params.framedrop_mode = 0;
        
        ret = vpx_codec_control(encoder, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_params);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_PARAMETERS
    // This control expects a void*, so we pass a pointer to the integer value
    if (len >= sizeof(int32_t)) {
        int param_val = read_int(&ptr, &len);
        ret = vpx_codec_control(encoder, VP9E_SET_SVC_PARAMETERS, &param_val);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_LAYER_ID
    if (len >= sizeof(vpx_svc_layer_id_t)) {
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        memcpy(&layer_id, ptr, sizeof(vpx_svc_layer_id_t));
        ptr += sizeof(vpx_svc_layer_id_t);
        len -= sizeof(vpx_svc_layer_id_t);
        
        ret = vpx_codec_control(encoder, VP9E_SET_SVC_LAYER_ID, &layer_id);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_GF_TEMPORAL_REF
    if (len >= sizeof(int32_t)) {
        int ref_val = read_int(&ptr, &len);
        ret = vpx_codec_control(encoder, VP9E_SET_SVC_GF_TEMPORAL_REF, ref_val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize encoder context
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Configure basic settings to allow SVC if needed
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1; 

    if (vpx_codec_enc_init(&encoder, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on controls
    fuzz_vp9_encoder_controls(&encoder, Data, Size);

    // Cleanup
    vpx_codec_destroy(&encoder);

    return 0;
}