// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Use the correct function name for AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to randomize configuration slightly within safe bounds
    // Ensure we check size before accessing data array
    if (size > 0) {
        cfg.g_w = (data[0] % 1920) + 16;
    }
    if (size > 1) {
        cfg.g_h = (data[1] % 1080) + 16;
    }
    if (size > 2) {
        cfg.g_timebase.num = (data[2] % 60) + 1;
    }
    if (size > 3) {
        cfg.g_timebase.den = (data[3] % 60) + 1;
    }
    if (size > 4) {
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_212(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Helper to safely cast control types
    #define TRY_CONTROL(ctx, ctrl, val) \
        do { \
            aom_codec_err_t res = aom_codec_control(ctx, ctrl, val); \
            (void)res; /* Ignore errors for fuzzing exploration */ \
        } while(0)

    // 1. AV1E_SET_MAX_GF_INTERVAL
    int max_gf_interval = (Data[0] % 128) + 1;
    TRY_CONTROL(ctx, AV1E_SET_MAX_GF_INTERVAL, max_gf_interval);

    // 2. AV1E_SET_NOISE_SENSITIVITY
    if (Size > 1) {
        int noise_sensitivity = (Data[1] % 64) + 1;
        TRY_CONTROL(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    }

    // 3. AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    if (Size > 2) {
        int max_drop = (Data[2] % 16) + 1;
        TRY_CONTROL(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop);
    }

    // 4. AV1E_SET_RATE_DISTRIBUTION_INFO
    // This typically requires a struct, but we can try with a dummy pointer or 0
    // Since we don't have the struct definition here, we pass a dummy value
    // In real usage, this would be a pointer to a specific struct.
    // For fuzzing, we attempt to pass a value derived from input.
    if (Size > 3) {
        int rate_dist = (Data[3] % 100);
        TRY_CONTROL(ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, rate_dist);
    }

    // 5. AV1E_SET_SVC_PARAMS
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    if (Size > 4) {
        svc_params.number_spatial_layers = (Data[4] % 3) + 1;
    }
    if (Size > 5) {
        svc_params.number_temporal_layers = (Data[5] % 3) + 1;
    }
    
    // Ensure we don't access out of bounds
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        int idx = (6 + i) % Size;
        svc_params.max_quantizers[i] = (Data[idx] % 63) + 1;
        
        int idx2 = (6 + i + 1) % Size;
        svc_params.min_quantizers[i] = (Data[idx2] % 63) + 1;
        
        int idx3 = (6 + i + 2) % Size;
        svc_params.layer_target_bitrate[i] = (Data[idx3] % 1000) + 100;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        int idx = (6 + i + 3) % Size;
        svc_params.scaling_factor_num[i] = (Data[idx] % 16) + 1;
        
        int idx2 = (6 + i + 4) % Size;
        svc_params.scaling_factor_den[i] = (Data[idx2] % 16) + 1;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        int idx = (6 + i + 5) % Size;
        svc_params.framerate_factor[i] = (Data[idx] % 8) + 1;
    }

    TRY_CONTROL(ctx, AV1E_SET_SVC_PARAMS, &svc_params);

    // 6. AV1E_SET_ENABLE_INTRABC
    if (Size > 7) {
        int enable_intrabc = (Data[7] % 2);
        TRY_CONTROL(ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    }

    cleanup_encoder_context(ctx);
    return 0;
}