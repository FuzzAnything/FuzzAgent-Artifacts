// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_MAX_REFERENCE_FRAMES
 * - AV1E_SET_TARGET_SEQ_LEVEL_IDX
 * - AV1E_SET_MAX_GF_INTERVAL
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AV1E_SET_SVC_PARAMS
 */

static int initialize_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg,
                              const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int width, height;

    if (size < 4) return 0;

    /* Extract width and height from fuzz input to vary encoder config */
    width = data[0] + 16;
    height = data[1] + 16;

    /* Ensure dimensions are reasonable for the encoder */
    if (width > 4096 || height > 4096) return 0;
    if (width < 16 || height < 16) return 0;

    /* Use aom_codec_av1_cx() to get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int param;
    aom_svc_params_t svc_params;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));
    memset(&svc_params, 0, sizeof(svc_params));

    if (!initialize_encoder(&codec, &cfg, Data, Size)) {
        return 0;
    }

    /* 1. AV1E_SET_ENABLE_INTRABC: Set to 0 or 1 based on input */
    if (Size > 0) {
        param = Data[0] % 2;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, param);
        (void)ret; /* Ignore return for fuzzing, just exercise */
    }

    /* 2. AV1E_SET_MAX_REFERENCE_FRAMES: Set to 0-7 */
    if (Size > 1) {
        param = Data[1] % 8;
        ret = aom_codec_control(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, param);
        (void)ret;
    }

    /* 3. AV1E_SET_TARGET_SEQ_LEVEL_IDX: Set to 0-7 */
    if (Size > 2) {
        param = Data[2] % 8;
        ret = aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, param);
        (void)ret;
    }

    /* 4. AV1E_SET_MAX_GF_INTERVAL: Set to 1-127 */
    if (Size > 3) {
        param = (Data[3] % 127) + 1;
        ret = aom_codec_control(&codec, AV1E_SET_MAX_GF_INTERVAL, param);
        (void)ret;
    }

    /* 5. AV1E_SET_NOISE_SENSITIVITY: Set to 0-63 */
    if (Size > 4) {
        param = Data[4] % 64;
        ret = aom_codec_control(&codec, AV1E_SET_NOISE_SENSITIVITY, param);
        (void)ret;
    }

    /* 6. AV1E_SET_SVC_PARAMS: Populate struct and set */
    if (Size > 5) {
        svc_params.number_spatial_layers = (Data[5] % 3) + 1;
        svc_params.number_temporal_layers = (Size > 6) ? ((Data[6] % 3) + 1) : 1;

        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            svc_params.max_quantizers[i] = (Data[(5 + i) % Size] % 64);
            svc_params.min_quantizers[i] = (Data[(6 + i) % Size] % 64);
            svc_params.layer_target_bitrate[i] = (Data[(7 + i) % Size] * 100);
        }

        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            svc_params.scaling_factor_num[i] = (Data[(8 + i) % Size] % 100) + 1;
            svc_params.scaling_factor_den[i] = (Data[(9 + i) % Size] % 100) + 1;
        }

        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            svc_params.framerate_factor[i] = (Data[(10 + i) % Size] % 10) + 1;
        }

        ret = aom_codec_control(&codec, AV1E_SET_SVC_PARAMS, &svc_params);
        (void)ret;
    }

    cleanup_encoder(&codec);
    return 0;
}