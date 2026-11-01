// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder if size allows
    // Ensure bounds checking before accessing data array
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
        if (size > 1) {
            cfg->g_h = (data[1] % 1024) + 16;
        }
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        if (size > 2) {
            cfg->g_lag_in_frames = (data[2] % 100);
        }
        if (size > 3) {
            cfg->rc_target_bitrate = ((data[3] % 10000) + 100);
        }
        if (size > 4) {
            cfg->rc_undershoot_pct = ((data[4] % 50) + 50);
        }
        if (size > 5) {
            cfg->rc_overshoot_pct = ((data[5] % 50) + 50);
        }
        if (size > 6) {
            cfg->rc_buf_initial_sz = ((data[6] % 1000) + 100);
        }
        if (size > 7) {
            cfg->rc_buf_optimal_sz = ((data[7] % 1000) + 100);
        }
        if (size > 8) {
            cfg->rc_buf_sz = ((data[8] % 1000) + 100);
        }
        if (size > 9) {
            cfg->g_error_resilient = (data[9] % 2);
        }
        if (size > 10) {
            cfg->g_pass = (data[10] % 2);
        }
        if (size > 11) {
            cfg->rc_end_usage = (data[11] % 3);
        }
        if (size > 15) {
            cfg->g_threads = ((data[15] % 8) + 1);
        }
        if (size > 16) {
            cfg->g_profile = (data[16] % 4);
        }
        if (size > 17) {
            cfg->g_bit_depth = ((data[17] % 2) + 8);
        }
        if (size > 19) {
            cfg->g_input_bit_depth = ((data[19] % 2) + 8);
        }
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static int prepare_fuzz_data(const uint8_t *data, size_t size, 
                             aom_svc_ref_frame_config_t *ref_config,
                             aom_svc_params_t *svc_params,
                             int *overlay_enable,
                             int *svc_frame_drop_mode,
                             int *num_spatial_layers) {
    if (!data || size == 0) return 0;

    // Prepare reference frame config
    memset(ref_config, 0, sizeof(*ref_config));
    for (int i = 0; i < 7; i++) {
        if (size > i) {
            ref_config->reference[i] = (data[i] % 2);
        } else {
            ref_config->reference[i] = 0;
        }
        if (size > i + 7) {
            ref_config->ref_idx[i] = (data[i + 7] % 8);
        } else {
            ref_config->ref_idx[i] = 0;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (size > i + 14) {
            ref_config->refresh[i] = (data[i + 14] % 2);
        } else {
            ref_config->refresh[i] = 0;
        }
    }

    // Prepare SVC params
    memset(svc_params, 0, sizeof(*svc_params));
    if (size > 21) {
        svc_params->number_spatial_layers = ((data[21] % 3) + 1);
    } else {
        svc_params->number_spatial_layers = 1;
    }
    if (size > 22) {
        svc_params->number_temporal_layers = ((data[22] % 3) + 1);
    } else {
        svc_params->number_temporal_layers = 1;
    }
    
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        if (size > 23 + i) {
            svc_params->max_quantizers[i] = (data[23 + i] % 63);
        } else {
            svc_params->max_quantizers[i] = 32;
        }
        if (size > 23 + i + AOM_MAX_LAYERS) {
            svc_params->min_quantizers[i] = (data[23 + i + AOM_MAX_LAYERS] % 63);
        } else {
            svc_params->min_quantizers[i] = 32;
        }
        if (size > 23 + i + 2*AOM_MAX_LAYERS) {
            svc_params->layer_target_bitrate[i] = ((data[23 + i + 2*AOM_MAX_LAYERS] % 1000) + 100);
        } else {
            svc_params->layer_target_bitrate[i] = 500;
        }
    }
    
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        if (size > 23 + i + 3*AOM_MAX_LAYERS) {
            svc_params->scaling_factor_num[i] = (data[23 + i + 3*AOM_MAX_LAYERS] % 10);
        } else {
            svc_params->scaling_factor_num[i] = 1;
        }
        if (size > 23 + i + 3*AOM_MAX_LAYERS + AOM_MAX_SS_LAYERS) {
            svc_params->scaling_factor_den[i] = (data[23 + i + 3*AOM_MAX_LAYERS + AOM_MAX_SS_LAYERS] % 10);
        } else {
            svc_params->scaling_factor_den[i] = 1;
        }
    }
    
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        if (size > 23 + i + 3*AOM_MAX_LAYERS + 2*AOM_MAX_SS_LAYERS) {
            svc_params->framerate_factor[i] = (data[23 + i + 3*AOM_MAX_LAYERS + 2*AOM_MAX_SS_LAYERS] % 10);
        } else {
            svc_params->framerate_factor[i] = 1;
        }
    }

    // Prepare other parameters
    if (size > 30) {
        *overlay_enable = (data[30] % 2);
    } else {
        *overlay_enable = 0;
    }
    if (size > 31) {
        *svc_frame_drop_mode = (data[31] % 3);
    } else {
        *svc_frame_drop_mode = 0;
    }
    if (size > 32) {
        *num_spatial_layers = ((data[32] % 3) + 1);
    } else {
        *num_spatial_layers = 1;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_208(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    aom_svc_ref_frame_config_t ref_config;
    aom_svc_params_t svc_params;
    int overlay_enable, svc_frame_drop_mode, num_spatial_layers;

    if (!prepare_fuzz_data(Data, Size, &ref_config, &svc_params, 
                           &overlay_enable, &svc_frame_drop_mode, &num_spatial_layers)) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Test AV1E_SET_SVC_REF_FRAME_CONFIG
    aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &ref_config);

    // Test AV1E_SET_ENABLE_OVERLAY
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, &overlay_enable);

    // Test AV1E_GET_NUM_OPERATING_POINTS
    int num_operating_points;
    aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_operating_points);

    // Test AV1E_SET_SVC_FRAME_DROP_MODE
    aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &svc_frame_drop_mode);

    // Test AOME_SET_NUMBER_SPATIAL_LAYERS
    aom_codec_control(&ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, &num_spatial_layers);

    // Test AV1E_SET_SVC_PARAMS
    aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);

    cleanup_codec(&ctx);
    return 0;
}