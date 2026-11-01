// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

// Helper to read integers from fuzz data safely
static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

// Helper to read uint8_t from fuzz data safely
static uint8_t read_u8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    uint8_t val = **data;
    *data += 1;
    *size -= 1;
    return val;
}

// Helper to read int8_t from fuzz data safely
static int8_t read_i8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    int8_t val = (int8_t)**data;
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize encoder context
    aom_codec_ctx_t codec;
    // Use the correct function name for AV1 codec interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzz data to configure basic settings
    cfg.g_w = read_int(&Data, &Size);
    cfg.g_h = read_int(&Data, &Size);
    cfg.g_timebase.num = read_u8(&Data, &Size);
    cfg.g_timebase.den = read_u8(&Data, &Size);
    cfg.rc_target_bitrate = read_int(&Data, &Size);

    if (cfg.g_w <= 0 || cfg.g_h <= 0) cfg.g_w = cfg.g_h = 16;
    if (cfg.g_timebase.num <= 0) cfg.g_timebase.num = 1;
    if (cfg.g_timebase.den <= 0) cfg.g_timebase.den = 30;
    if (cfg.rc_target_bitrate <= 0) cfg.rc_target_bitrate = 100;

    aom_codec_err_t err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Prepare a dummy frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(&img.planes[0][0], 128, cfg.g_w * cfg.g_h * 3 / 2);

    // 1. aom_codec_control_typechecked_AV1E_SET_SVC_LAYER_ID
    if (Size >= 4) {
        aom_svc_layer_id_t layer_id;
        layer_id.spatial_layer_id = read_i8(&Data, &Size);
        layer_id.temporal_layer_id = read_i8(&Data, &Size);
        aom_codec_control(&codec, AV1E_SET_SVC_LAYER_ID, &layer_id);
    }

    // 2. aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID
    if (Size >= 2) {
        int spatial_layer_id = read_i8(&Data, &Size);
        aom_codec_control(&codec, AOME_SET_SPATIAL_LAYER_ID, &spatial_layer_id);
    }

    // 3. aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS
    if (Size >= 2) {
        int num_spatial_layers = read_i8(&Data, &Size);
        aom_codec_control(&codec, AOME_SET_NUMBER_SPATIAL_LAYERS, &num_spatial_layers);
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
    if (Size >= 2) {
        int frame_drop_mode = read_i8(&Data, &Size);
        aom_codec_control(&codec, AV1E_SET_SVC_FRAME_DROP_MODE, &frame_drop_mode);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_CONFIG
    if (Size >= 30) {
        aom_svc_ref_frame_config_t ref_config;
        for (int i = 0; i < 7; i++) ref_config.reference[i] = read_u8(&Data, &Size);
        for (int i = 0; i < 7; i++) ref_config.ref_idx[i] = read_u8(&Data, &Size);
        for (int i = 0; i < 8; i++) ref_config.refresh[i] = read_u8(&Data, &Size);
        aom_codec_control(&codec, AV1E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_SVC_PARAMS
    if (Size >= 100) {
        aom_svc_params_t svc_params;
        svc_params.number_spatial_layers = read_i8(&Data, &Size);
        svc_params.number_temporal_layers = read_i8(&Data, &Size);
        for (int i = 0; i < AOM_MAX_LAYERS; i++) svc_params.max_quantizers[i] = read_i8(&Data, &Size);
        for (int i = 0; i < AOM_MAX_LAYERS; i++) svc_params.min_quantizers[i] = read_i8(&Data, &Size);
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) svc_params.scaling_factor_num[i] = read_i8(&Data, &Size);
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) svc_params.scaling_factor_den[i] = read_i8(&Data, &Size);
        for (int i = 0; i < AOM_MAX_LAYERS; i++) svc_params.layer_target_bitrate[i] = read_int(&Data, &Size);
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) svc_params.framerate_factor[i] = read_i8(&Data, &Size);
        aom_codec_control(&codec, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&codec);

    return 0;
}