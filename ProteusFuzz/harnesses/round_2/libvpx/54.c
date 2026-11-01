// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_GET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1080:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void initialize_fuzz_data(const uint8_t *Data, size_t Size,
                                 vpx_svc_layer_id_t *layer_id,
                                 vpx_svc_frame_drop_t *frame_drop,
                                 vpx_svc_ref_frame_config_t *ref_config) {
    memset(layer_id, 0, sizeof(*layer_id));
    memset(frame_drop, 0, sizeof(*frame_drop));
    memset(ref_config, 0, sizeof(*ref_config));

    if (Size >= 2) {
        layer_id->spatial_layer_id = (int)Data[0];
        layer_id->temporal_layer_id = (int)Data[1];
    }
    if (Size >= 4) {
        layer_id->temporal_layer_id_per_spatial[0] = (int)Data[2];
        layer_id->temporal_layer_id_per_spatial[1] = (int)Data[3];
    }

    if (Size >= 6) {
        frame_drop->framedrop_thresh[0] = (int)Data[4];
        frame_drop->framedrop_thresh[1] = (int)Data[5];
    }
    if (Size >= 8) {
        frame_drop->framedrop_mode = (SVC_LAYER_DROP_MODE)(Data[6] % 2);
        frame_drop->max_consec_drop = (int)Data[7];
    }

    if (Size >= 10) {
        ref_config->lst_fb_idx[0] = (int)Data[8];
        ref_config->gld_fb_idx[0] = (int)Data[9];
    }
    if (Size >= 12) {
        ref_config->alt_fb_idx[0] = (int)Data[10];
        ref_config->update_buffer_slot[0] = (int)Data[11];
    }
    if (Size >= 14) {
        ref_config->update_last[0] = (int)Data[12];
        ref_config->update_golden[0] = (int)Data[13];
    }
    if (Size >= 16) {
        ref_config->update_alt_ref[0] = (int)Data[14];
        ref_config->reference_last[0] = (int)Data[15];
    }
    if (Size >= 18) {
        ref_config->reference_golden[0] = (int)Data[16];
        ref_config->reference_alt_ref[0] = (int)Data[17];
    }
    if (Size >= 20) {
        ref_config->duration[0] = (int64_t)Data[18] | ((int64_t)Data[19] << 8);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_svc_layer_id_t layer_id;
    vpx_svc_frame_drop_t frame_drop;
    vpx_svc_ref_frame_config_t ref_config;

    initialize_fuzz_data(Data, Size, &layer_id, &frame_drop, &ref_config);

    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 120;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 0;
    cfg.g_pass = 0;

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Test VP9E_SET_SVC_LAYER_ID
    vpx_svc_layer_id_t test_layer_id = layer_id;
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &test_layer_id);
    (void)res;

    // Test VP9E_SET_SVC_FRAME_DROP_LAYER
    vpx_svc_frame_drop_t test_frame_drop = frame_drop;
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &test_frame_drop);
    (void)res;

    // Test VP9E_SET_SVC_REF_FRAME_CONFIG
    vpx_svc_ref_frame_config_t test_ref_config = ref_config;
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &test_ref_config);
    (void)res;

    // Test VP9E_SET_SVC_INTER_LAYER_PRED
    int inter_layer_pred = (Size > 20) ? (int)Data[20] % 2 : 0;
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_layer_pred);
    (void)res;

    // Test VP9E_SET_NOISE_SENSITIVITY
    int noise_sensitivity = (Size > 21) ? (int)Data[21] % 100 : 0;
    res = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    (void)res;

    // Test VP9E_GET_SVC_REF_FRAME_CONFIG
    vpx_svc_ref_frame_config_t out_ref_config;
    memset(&out_ref_config, 0, sizeof(out_ref_config));
    res = vpx_codec_control(&ctx, VP9E_GET_SVC_REF_FRAME_CONFIG, &out_ref_config);
    (void)res;

    vpx_codec_destroy(&ctx);
    return 0;
}