// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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

/* Helper to safely initialize an encoder context */
static int init_encoder_context(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!codec || !cfg) return -1;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    /* Use fuzzing data to set some config parameters safely */
    if (size >= 2) {
        cfg->g_w = (data[0] % 1024) + 16;
        cfg->g_h = (data[1] % 1024) + 16;
    } else {
        cfg->g_w = 16;
        cfg->g_h = 16;
    }
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    // Removed cfg->g_frame_padding as it does not exist in the struct

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder_context(&codec, &cfg, Data, Size) != 0) {
        return 0;
    }

    /* Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR */
    if (Size >= 1) {
        int max_drop = Data[0] % 10;
        aom_codec_control(&codec, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop);
    }

    /* Fuzz AV1E_SET_SVC_PARAMS */
    if (Size >= 4) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        svc_params.number_spatial_layers = (Data[0] % 3) + 1;
        svc_params.number_temporal_layers = (Data[1] % 3) + 1;
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            svc_params.max_quantizers[i] = (Data[2] % 64) + 1;
            svc_params.min_quantizers[i] = (Data[3] % 64) + 1;
        }
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            svc_params.scaling_factor_num[i] = 1;
            svc_params.scaling_factor_den[i] = 1;
        }
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            svc_params.layer_target_bitrate[i] = (Data[0] % 1000) + 1;
        }
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            svc_params.framerate_factor[i] = 1;
        }
        aom_codec_control(&codec, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    /* Fuzz AV1E_SET_ENABLE_INTRABC */
    if (Size >= 1) {
        int enable = Data[0] % 2;
        aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, enable);
    }

    /* Fuzz AV1E_SET_RATE_DISTRIBUTION_INFO */
    /* Skipped due to missing struct definition in provided context */

    /* Fuzz AV1E_SET_MAX_REFERENCE_FRAMES */
    if (Size >= 1) {
        int max_ref = (Data[0] % 8) + 1;
        aom_codec_control(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref);
    }

    /* Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX */
    if (Size >= 1) {
        int level_idx = Data[0] % 10;
        aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);
    }

    /* Cleanup */
    aom_codec_destroy(&codec);

    return 0;
}