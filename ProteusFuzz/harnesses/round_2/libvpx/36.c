// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Helper to safely read integers from fuzz data
static int read_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    return (int)data[(*offset)++];
}

static int64_t read_int64(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    int64_t val = (int64_t)data[(*offset)++];
    return val;
}

static void init_fuzz_ref_frame_config(vpx_svc_ref_frame_config_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(*cfg));
    size_t offset = 0;
    for (int i = 0; i < VPX_SS_MAX_LAYERS; ++i) {
        cfg->lst_fb_idx[i] = read_int(data, size, &offset);
        cfg->gld_fb_idx[i] = read_int(data, size, &offset);
        cfg->alt_fb_idx[i] = read_int(data, size, &offset);
        cfg->update_buffer_slot[i] = read_int(data, size, &offset);
        cfg->update_last[i] = read_int(data, size, &offset);
        cfg->update_golden[i] = read_int(data, size, &offset);
        cfg->update_alt_ref[i] = read_int(data, size, &offset);
        cfg->reference_last[i] = read_int(data, size, &offset);
        cfg->reference_golden[i] = read_int(data, size, &offset);
        cfg->reference_alt_ref[i] = read_int(data, size, &offset);
        cfg->duration[i] = read_int64(data, size, &offset);
    }
}

static void init_fuzz_layer_id(vpx_svc_layer_id_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(*cfg));
    size_t offset = 0;
    cfg->spatial_layer_id = read_int(data, size, &offset);
    cfg->temporal_layer_id = read_int(data, size, &offset);
    for (int i = 0; i < VPX_SS_MAX_LAYERS; ++i) {
        cfg->temporal_layer_id_per_spatial[i] = read_int(data, size, &offset);
    }
}

static void init_fuzz_frame_drop(vpx_svc_frame_drop_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(*cfg));
    size_t offset = 0;
    for (int i = 0; i < VPX_SS_MAX_LAYERS; ++i) {
        cfg->framedrop_thresh[i] = read_int(data, size, &offset);
    }
    int mode_val = read_int(data, size, &offset);
    cfg->framedrop_mode = (SVC_LAYER_DROP_MODE)(mode_val % 2);
    cfg->max_consec_drop = read_int(data, size, &offset);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    vpx_codec_err_t err = vpx_codec_enc_init(&ctx, codec, NULL, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz VP9E_SET_SVC
    int svc_val = Data[0];
    vpx_codec_control(&ctx, VP9E_SET_SVC, svc_val);

    // Fuzz VP9E_SET_SVC_LAYER_ID
    vpx_svc_layer_id_t layer_id;
    init_fuzz_layer_id(&layer_id, Data, Size);
    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);

    // Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG
    vpx_svc_ref_frame_config_t ref_cfg;
    init_fuzz_ref_frame_config(&ref_cfg, Data, Size);
    vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);

    // Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
    vpx_svc_frame_drop_t drop_cfg;
    init_fuzz_frame_drop(&drop_cfg, Data, Size);
    vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_cfg);

    // Fuzz VP9E_SET_SVC_INTER_LAYER_PRED
    int inter_pred_val = (Size > 1) ? Data[1] : 0;
    vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_pred_val);

    // Fuzz VP9E_SET_SVC_PARAMETERS
    // This control expects a void* (pointer to struct or data), not a raw int.
    // We pass a pointer to an integer variable to satisfy the type requirement.
    int svc_params_val = (Size > 2) ? Data[2] : 0;
    vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, &svc_params_val);

    vpx_codec_destroy(&ctx);
    return 0;
}