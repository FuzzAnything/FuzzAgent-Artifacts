// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

/*
 * Helper function to initialize an encoder context for fuzzing.
 * Returns a valid aom_codec_ctx_t pointer or NULL on failure.
 */
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to vary configuration slightly if possible
    // Ensure bounds checking before accessing data array
    if (size > 0) {
        cfg.g_w = 64 + (data[0] % 128);
        if (size > 1) {
            cfg.g_h = 64 + (data[1] % 128);
        }
        if (size > 2) {
            cfg.g_timebase.num = 1 + (data[2] % 10);
        }
        if (size > 3) {
            cfg.g_timebase.den = 1 + (data[3] % 10);
        }
        if (size > 4) {
            cfg.rc_target_bitrate = 100 + (data[4] % 1000);
        }
    }

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to initialize a decoder context for fuzzing.
 * Returns a valid aom_codec_ctx_t pointer or NULL on failure.
 */
static aom_codec_ctx_t *init_decoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: use allow_lowbitdepth instead of allow_highbitdepth
    cfg.allow_lowbitdepth = 1;

    err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Randomly choose between encoder and decoder context
    aom_codec_ctx_t *ctx = NULL;
    int is_encoder = (Data[0] % 2);

    if (is_encoder) {
        ctx = init_encoder_context(Data, Size);
    } else {
        ctx = init_decoder_context(Data, Size);
    }

    if (!ctx) return 0;

    // Fuzz various control commands
    // 1. AV1E_SET_DENOISE_NOISE_LEVEL
    if (is_encoder) {
        int noise_level = (Size > 1) ? (Data[1] % 100) : 0;
        aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, &noise_level);
    }

    // 2. AV1E_SET_LOOPFILTER_CONTROL
    if (is_encoder) {
        int lf_level = (Size > 2) ? (Data[2] % 64) : 0;
        aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, &lf_level);
    }

    // 3. AV1E_SET_SKIP_POSTPROC_FILTERING
    if (is_encoder) {
        int skip = (Size > 3) ? (Data[3] % 2) : 0;
        aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip);
    }

    // 4. AV1E_SET_SVC_PARAMS
    if (is_encoder) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        svc_params.number_spatial_layers = 1 + ((Size > 4) ? (Data[4] % 3) : 0);
        svc_params.number_temporal_layers = 1 + ((Size > 5) ? (Data[5] % 3) : 0);
        
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            int idx1 = 6 + i;
            int idx2 = 10 + i;
            int idx3 = 14 + i;
            svc_params.max_quantizers[i] = 1 + ((Size > idx1) ? (Data[idx1] % 63) : 0);
            svc_params.min_quantizers[i] = 1 + ((Size > idx2) ? (Data[idx2] % 63) : 0);
            svc_params.layer_target_bitrate[i] = 100 + ((Size > idx3) ? (Data[idx3] % 1000) : 0);
        }
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            int idx = 18 + i;
            svc_params.framerate_factor[i] = 1 + ((Size > idx) ? (Data[idx] % 10) : 0);
        }
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            int idx_num = 22 + i;
            int idx_den = 26 + i;
            svc_params.scaling_factor_num[i] = 1 + ((Size > idx_num) ? (Data[idx_num] % 10) : 0);
            svc_params.scaling_factor_den[i] = 1 + ((Size > idx_den) ? (Data[idx_den] % 10) : 0);
        }
        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    // 5. AV1_SET_SKIP_LOOP_FILTER (Decoder)
    if (!is_encoder) {
        int skip = (Size > 30) ? (Data[30] % 2) : 0;
        aom_codec_control(ctx, AV1_SET_SKIP_LOOP_FILTER, &skip);
    }

    // 6. AOME_GET_LOOPFILTER_LEVEL (Encoder)
    if (is_encoder) {
        int lf_level_out = 0;
        aom_codec_control(ctx, AOME_GET_LOOPFILTER_LEVEL, &lf_level_out);
        (void)lf_level_out; // Suppress unused variable warning
    }

    // Cleanup
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}