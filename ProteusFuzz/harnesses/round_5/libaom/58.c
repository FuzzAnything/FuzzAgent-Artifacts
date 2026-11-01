// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to set some config parameters safely
    if (size > 0) cfg.g_w = (data[0] % 1024) + 16;
    if (size > 1) cfg.g_h = (data[1] % 1024) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    int val = 0;
    int ret;

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    val = (Data[0] % 2);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
    (void)ret; // Ignore return for fuzzing state exploration

    // Fuzz AV1E_SET_ENABLE_INTRABC
    val = (Size > 1 ? Data[1] % 2 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)ret;

    // Fuzz AV1E_SET_SVC_LAYER_ID
    if (Size > 3) {
        aom_svc_layer_id_t svc_id;
        svc_id.spatial_layer_id = Data[2] % 4;
        svc_id.temporal_layer_id = Data[3] % 4;
        ret = aom_codec_control(ctx, AV1E_SET_SVC_LAYER_ID, &svc_id);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    val = (Size > 4 ? Data[4] % 2 : 1);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
    (void)ret;

    // Fuzz AV1E_SET_SVC_PARAMS
    if (Size > 5) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        svc_params.number_spatial_layers = (Data[5] % 3) + 1;
        svc_params.number_temporal_layers = (Size > 6 ? Data[6] % 3 : 1) + 1;
        
        // Ensure we don't access out of bounds
        int max_idx = Size - 1;
        
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            svc_params.max_quantizers[i] = (Data[(i + 7) % max_idx] % 64) + 1;
            svc_params.min_quantizers[i] = (Data[(i + 8) % max_idx] % 64) + 1;
            svc_params.layer_target_bitrate[i] = (Data[(i + 9) % max_idx] % 100) + 100;
        }
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            svc_params.scaling_factor_num[i] = (Data[(i + 10) % max_idx] % 16) + 1;
            svc_params.scaling_factor_den[i] = (Data[(i + 11) % max_idx] % 16) + 1;
        }
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            svc_params.framerate_factor[i] = (Data[(i + 12) % max_idx] % 16) + 1;
        }
        ret = aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
        (void)ret;
    }

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    val = (Size > 13 ? Data[13] % 2 : 1);
    ret = aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    (void)ret;

    cleanup_codec(ctx);
    return 0;
}