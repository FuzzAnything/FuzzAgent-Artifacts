// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_GET_SVC_LAYER_ID__value at vp8cx.h:1046:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzzer_test_vp9e_set_svc_layer_id(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_svc_layer_id_t layer_id;
    
    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }
    
    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) {
        return 0;
    }

    memset(&layer_id, 0, sizeof(layer_id));
    if (size >= sizeof(layer_id)) {
        memcpy(&layer_id, data, sizeof(layer_id));
    } else if (size > 0) {
        memcpy(&layer_id, data, size);
    }

    if (layer_id.spatial_layer_id < 0) layer_id.spatial_layer_id = 0;
    if (layer_id.spatial_layer_id > 3) layer_id.spatial_layer_id = 3;
    if (layer_id.temporal_layer_id < 0) layer_id.temporal_layer_id = 0;
    if (layer_id.temporal_layer_id > 7) layer_id.temporal_layer_id = 7;

    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        if (layer_id.temporal_layer_id_per_spatial[i] < 0) layer_id.temporal_layer_id_per_spatial[i] = 0;
        if (layer_id.temporal_layer_id_per_spatial[i] > 7) layer_id.temporal_layer_id_per_spatial[i] = 7;
    }

    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzzer_test_vp9e_set_svc_ref_frame_config(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_svc_ref_frame_config_t config;

    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }
    
    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) {
        return 0;
    }

    memset(&config, 0, sizeof(config));
    if (size >= sizeof(config)) {
        memcpy(&config, data, sizeof(config));
    } else if (size > 0) {
        memcpy(&config, data, size);
    }

    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        if (config.lst_fb_idx[i] < 0) config.lst_fb_idx[i] = 0;
        if (config.lst_fb_idx[i] > 15) config.lst_fb_idx[i] = 15;
        if (config.gld_fb_idx[i] < 0) config.gld_fb_idx[i] = 0;
        if (config.gld_fb_idx[i] > 15) config.gld_fb_idx[i] = 15;
        if (config.alt_fb_idx[i] < 0) config.alt_fb_idx[i] = 0;
        if (config.alt_fb_idx[i] > 15) config.alt_fb_idx[i] = 15;
        if (config.update_buffer_slot[i] < 0) config.update_buffer_slot[i] = 0;
        if (config.update_buffer_slot[i] > 1) config.update_buffer_slot[i] = 1;
        if (config.update_last[i] < 0) config.update_last[i] = 0;
        if (config.update_last[i] > 1) config.update_last[i] = 1;
        if (config.update_golden[i] < 0) config.update_golden[i] = 0;
        if (config.update_golden[i] > 1) config.update_golden[i] = 1;
        if (config.update_alt_ref[i] < 0) config.update_alt_ref[i] = 0;
        if (config.update_alt_ref[i] > 1) config.update_alt_ref[i] = 1;
        if (config.reference_last[i] < 0) config.reference_last[i] = 0;
        if (config.reference_last[i] > 1) config.reference_last[i] = 1;
        if (config.reference_golden[i] < 0) config.reference_golden[i] = 0;
        if (config.reference_golden[i] > 1) config.reference_golden[i] = 1;
        if (config.reference_alt_ref[i] < 0) config.reference_alt_ref[i] = 0;
        if (config.reference_alt_ref[i] > 1) config.reference_alt_ref[i] = 1;
        if (config.duration[i] < 0) config.duration[i] = 1;
    }

    vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &config);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzzer_test_vp9e_set_svc_parameters(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_svc_layer_id_t params;

    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }
    
    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) {
        return 0;
    }

    memset(&params, 0, sizeof(params));
    if (size >= sizeof(params)) {
        memcpy(&params, data, sizeof(params));
    } else if (size > 0) {
        memcpy(&params, data, size);
    }

    if (params.spatial_layer_id < 0) params.spatial_layer_id = 0;
    if (params.spatial_layer_id > 3) params.spatial_layer_id = 3;
    if (params.temporal_layer_id < 0) params.temporal_layer_id = 0;
    if (params.temporal_layer_id > 7) params.temporal_layer_id = 7;

    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        if (params.temporal_layer_id_per_spatial[i] < 0) params.temporal_layer_id_per_spatial[i] = 0;
        if (params.temporal_layer_id_per_spatial[i] > 7) params.temporal_layer_id_per_spatial[i] = 7;
    }

    vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, &params);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzzer_test_vp9e_set_row_mt(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int value = 0;

    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }
    
    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) {
        return 0;
    }

    if (size > 0) {
        value = data[0];
    }

    if (value < 0) value = 0;
    if (value > 1) value = 1;

    vpx_codec_control(&ctx, VP9E_SET_ROW_MT, value);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzzer_test_vp9e_set_svc_inter_layer_pred(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int value = 0;

    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }
    
    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) {
        return 0;
    }

    if (size > 0) {
        value = data[0];
    }

    if (value < 0) value = 0;
    if (value > 1) value = 1;

    vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, value);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzzer_test_vp9e_get_svc_layer_id(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_svc_layer_id_t layer_id;

    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }
    
    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) {
        return 0;
    }

    memset(&layer_id, 0, sizeof(layer_id));
    if (size >= sizeof(layer_id)) {
        memcpy(&layer_id, data, sizeof(layer_id));
    } else if (size > 0) {
        memcpy(&layer_id, data, size);
    }

    vpx_codec_control(&ctx, VP9E_GET_SVC_LAYER_ID, &layer_id);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int choice = Data[0] % 6;

    switch (choice) {
        case 0:
            fuzzer_test_vp9e_set_svc_layer_id(Data, Size);
            break;
        case 1:
            fuzzer_test_vp9e_set_svc_ref_frame_config(Data, Size);
            break;
        case 2:
            fuzzer_test_vp9e_set_svc_parameters(Data, Size);
            break;
        case 3:
            fuzzer_test_vp9e_set_row_mt(Data, Size);
            break;
        case 4:
            fuzzer_test_vp9e_set_svc_inter_layer_pred(Data, Size);
            break;
        case 5:
            fuzzer_test_vp9e_get_svc_layer_id(Data, Size);
            break;
        default:
            break;
    }

    return 0;
}