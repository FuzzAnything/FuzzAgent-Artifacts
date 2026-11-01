// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/*
 * Note: The target functions listed in the prompt (e.g., aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR)
 * are internal or type-checked wrapper functions not typically exposed in public headers with that exact naming convention.
 * The standard public API uses aom_codec_control with enum constants like AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR.
 * This fuzzer implements the standard public API pattern to test the underlying logic safely.
 */

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    int width = 64, height = 64;

    if (data && size >= 4) {
        width = data[0] ? data[0] : 64;
        height = data[1] ? data[1] : 64;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return -1;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 1;

    err = aom_codec_enc_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(codec);
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_199(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg, Data, Size) != 0) {
        return 0;
    }

    /* 
     * Attempt to invoke the control commands corresponding to the target functions.
     * Since the specific typechecked wrappers are not standard public APIs, 
     * we use the standard aom_codec_control with the respective enums.
     */

    /* 1. AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR */
    int max_drop = 0;
    if (Size > 0) max_drop = Data[0] % 10;
    aom_codec_control(&codec, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop);

    /* 2. AV1E_SET_SVC_PARAMS */
    if (Size > 1) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        
        svc_params.number_spatial_layers = (Data[1] % 3) + 1;
        svc_params.number_temporal_layers = (Data[1] % 3) + 1;
        
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            svc_params.max_quantizers[i] = (Data[(i + 2) % Size] * 63) / 255;
            svc_params.min_quantizers[i] = (Data[(i + 3) % Size] * 63) / 255;
            svc_params.layer_target_bitrate[i] = (Data[(i + 4) % Size] * 1000) / 255;
        }
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            svc_params.scaling_factor_num[i] = (Data[(i + 5) % Size] % 10) + 1;
            svc_params.scaling_factor_den[i] = (Data[(i + 6) % Size] % 10) + 1;
        }
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            svc_params.framerate_factor[i] = (Data[(i + 7) % Size] % 10) + 1;
        }

        aom_codec_control(&codec, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    /* 3. AV1E_SET_ENABLE_INTRABC */
    int enable_intrabc = (Size > 2) ? (Data[2] % 2) : 0;
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    /* 4. AV1E_SET_RATE_DISTRIBUTION_INFO */
    if (Size > 3) {
        /* This control usually requires a specific structure, passing a pointer to data as a heuristic */
        /* In real usage, this requires a valid aom_rate_distribution_info_t structure. 
           We pass a pointer to the fuzz data to trigger internal parsing or validation logic. */
        aom_codec_control(&codec, AV1E_SET_RATE_DISTRIBUTION_INFO, (void*)Data);
    }

    /* 5. AV1E_SET_MAX_REFERENCE_FRAMES */
    int max_ref = 0;
    if (Size > 4) max_ref = Data[4] % 8;
    aom_codec_control(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref);

    /* 6. AV1E_SET_TARGET_SEQ_LEVEL_IDX */
    int level_idx = 0;
    if (Size > 5) level_idx = Data[5] % 10;
    aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);

    /* Cleanup */
    cleanup_encoder(&codec);

    return 0;
}