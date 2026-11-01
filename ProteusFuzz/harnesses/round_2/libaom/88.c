// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int idx = 0;
    while (idx < size) {
        int val = 0;
        if (idx < size) {
            val = data[idx++];
        }

        // AV1E_SET_ENABLE_AB_PARTITIONS
        if (aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }

        if (idx < size) {
            val = data[idx++];
        }
        // AV1E_SET_ENABLE_DNL_DENOISING
        if (aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val) != AOM_CODEC_OK) {
            // Ignore errors
        }

        if (idx < size) {
            val = data[idx++];
        }
        // AV1E_SET_ENABLE_1TO4_PARTITIONS
        if (aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val) != AOM_CODEC_OK) {
            // Ignore errors
        }

        if (idx < size) {
            val = data[idx++];
        }
        // AV1E_SET_DELTAQ_STRENGTH
        if (aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // AV1E_SET_SVC_PARAMS requires more complex data
        if (idx + sizeof(aom_svc_params_t) <= size) {
            aom_svc_params_t svc_params;
            memcpy(&svc_params, &data[idx], sizeof(aom_svc_params_t));
            idx += sizeof(aom_svc_params_t);

            // Apply some sanitization to avoid obvious invalid states that might crash immediately
            if (svc_params.number_spatial_layers < 1) svc_params.number_spatial_layers = 1;
            if (svc_params.number_spatial_layers > 3) svc_params.number_spatial_layers = 3;
            if (svc_params.number_temporal_layers < 1) svc_params.number_temporal_layers = 1;
            if (svc_params.number_temporal_layers > 3) svc_params.number_temporal_layers = 3;

            if (aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params) != AOM_CODEC_OK) {
                // Ignore errors
            }
        }
    }
    return 1;
}

static int fuzz_global_headers(aom_codec_ctx_t *ctx) {
    if (!ctx) return 0;

    aom_fixed_buf_t *headers = aom_codec_get_global_headers(ctx);
    if (headers) {
        // We don't need to do anything with the data, just ensure we free it
        if (headers->buf) {
            free(headers->buf);
        }
        free(headers);
    }
    return 1;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *codec_iface = aom_codec_av1_cx();
    if (!codec_iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    err = aom_codec_enc_config_default(codec_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some minimal valid config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    err = aom_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control commands
    fuzz_encoder_controls(&ctx, Data, Size);

    // Fuzz global headers retrieval
    fuzz_global_headers(&ctx);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}