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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    if (size >= sizeof(int)) {
        int max_ref_frames = *(const int *)data;
        aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);
    }

    // Fuzz AOME_SET_ENABLEAUTOALTREF
    if (size >= sizeof(int) + 4) {
        int enable_auto_altref = *(const int *)(data + 4);
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_altref);
    }

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (size >= sizeof(int) + 8) {
        int seq_level_idx = *(const int *)(data + 8);
        aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, seq_level_idx);
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    if (size >= sizeof(int) + 12) {
        int noise_sensitivity = *(const int *)(data + 12);
        aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    }

    // Fuzz AV1E_SET_SVC_PARAMS
    if (size >= sizeof(aom_svc_params_t) + 16) {
        aom_svc_params_t svc_params;
        memcpy(&svc_params, data + 16, sizeof(aom_svc_params_t));
        // Ensure valid ranges for spatial and temporal layers
        if (svc_params.number_spatial_layers < 1 || svc_params.number_spatial_layers > 3) {
            svc_params.number_spatial_layers = 1;
        }
        if (svc_params.number_temporal_layers < 1 || svc_params.number_temporal_layers > 3) {
            svc_params.number_temporal_layers = 1;
        }
        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    // Fuzz AV1E_SET_QM_MAX
    if (size >= sizeof(int) + sizeof(aom_svc_params_t) + 16) {
        int qm_max = *(const int *)(data + sizeof(aom_svc_params_t) + 16);
        aom_codec_control(ctx, AV1E_SET_QM_MAX, qm_max);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 32) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}