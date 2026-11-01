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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int get_int(const uint8_t *d, size_t s, size_t idx, int min, int max) {
    if (s > idx) {
        int val = d[idx];
        return min + (val % (max - min + 1));
    }
    return min;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    // Use the standard function to get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Fuzz some config parameters based on input data if size permits
    if (size > 0) cfg.g_w = (data[0] % 1920) + 16;
    if (size > 1) cfg.g_h = (data[1] % 1080) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    cfg.g_threads = 0; // Let library decide or use 1 for safety in fuzzing
    cfg.rc_target_bitrate = (size > 4) ? (data[4] * 100) + 100 : 1000;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_257(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_QM
    // Assuming signature: aom_codec_control(ctx, AV1E_SET_ENABLE_QM, int)
    int enable_qm = get_int(Data, Size, 0, 0, 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_QM, enable_qm);

    // 2. aom_codec_control_typechecked_AV1E_SET_QM_MAX
    // Assuming signature: aom_codec_control(ctx, AV1E_SET_QM_MAX, int)
    int qm_max = get_int(Data, Size, 1, 0, 63);
    aom_codec_control(ctx, AV1E_SET_QM_MAX, qm_max);

    // 3. aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY
    // Assuming signature: aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, int)
    int noise_sens = get_int(Data, Size, 2, 0, 100);
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sens);

    // 4. aom_codec_control_typechecked_AV1E_SET_SVC_PARAMS
    // Assuming signature: aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, aom_svc_params_t *)
    if (Size > 10) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        svc_params.number_spatial_layers = get_int(Data, Size, 10, 1, 3);
        svc_params.number_temporal_layers = get_int(Data, Size, 11, 1, 3);
        
        // Fill arrays with fuzzed data if possible
        int layers = svc_params.number_spatial_layers * svc_params.number_temporal_layers;
        for (int i = 0; i < layers && i < AOM_MAX_LAYERS; i++) {
            svc_params.max_quantizers[i] = get_int(Data, Size, 12 + i, 0, 63);
            svc_params.min_quantizers[i] = get_int(Data, Size, 12 + i + AOM_MAX_LAYERS, 0, 63);
            svc_params.layer_target_bitrate[i] = get_int(Data, Size, 12 + i + 2 * AOM_MAX_LAYERS, 0, 10000);
        }
        for (int i = 0; i < svc_params.number_spatial_layers && i < AOM_MAX_SS_LAYERS; i++) {
            svc_params.scaling_factor_num[i] = get_int(Data, Size, 12 + i + 3 * AOM_MAX_LAYERS, 1, 100);
            svc_params.scaling_factor_den[i] = get_int(Data, Size, 12 + i + 3 * AOM_MAX_LAYERS + AOM_MAX_SS_LAYERS, 1, 100);
        }
        for (int i = 0; i < svc_params.number_temporal_layers && i < AOM_MAX_TS_LAYERS; i++) {
            svc_params.framerate_factor[i] = get_int(Data, Size, 12 + i + 3 * AOM_MAX_LAYERS + 2 * AOM_MAX_SS_LAYERS, 1, 100);
        }

        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    // 5. aom_codec_control_typechecked_AV1E_ENABLE_RATE_GUIDE_DELTAQ
    // Assuming signature: aom_codec_control(ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, int)
    int enable_deltaq = get_int(Data, Size, 13, 0, 1);
    aom_codec_control(ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, enable_deltaq);

    // 6. aom_codec_control_typechecked_AOME_SET_ENABLEAUTOALTREF
    // Assuming signature: aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, int)
    int enable_auto_altref = get_int(Data, Size, 14, 0, 1);
    aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_altref);

    cleanup_encoder(ctx);
    return 0;
}