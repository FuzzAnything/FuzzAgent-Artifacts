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
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aomcx.h"

static int fuzz_codec_control(aom_codec_ctx_t *codec, int ctrl_id, void *arg, size_t arg_size, const uint8_t *data, size_t size) {
    if (!codec || !data || size < 1) return 0;

    // Determine the type of argument based on the control ID
    // Since the specific signatures are missing, we infer from the names and standard libaom patterns.
    
    if (ctrl_id == AV1E_SET_ENABLE_QM) {
        // Expects an int (0 or 1)
        if (size >= sizeof(int)) {
            int val = *(const int*)data;
            // Fuzz: try 0, 1, and random values if size allows
            aom_codec_control(codec, ctrl_id, val);
        }
    } else if (ctrl_id == AV1E_SET_QM_MAX) {
        // Expects an int
        if (size >= sizeof(int)) {
            int val = *(const int*)data;
            aom_codec_control(codec, ctrl_id, val);
        }
    } else if (ctrl_id == AV1E_SET_NOISE_SENSITIVITY) {
        // Expects an int
        if (size >= sizeof(int)) {
            int val = *(const int*)data;
            aom_codec_control(codec, ctrl_id, val);
        }
    } else if (ctrl_id == AV1E_SET_DELTAQ_STRENGTH) {
        // Expects an int
        if (size >= sizeof(int)) {
            int val = *(const int*)data;
            aom_codec_control(codec, ctrl_id, val);
        }
    } else if (ctrl_id == AV1E_SET_SVC_PARAMS) {
        // Expects aom_svc_params_t*
        if (size >= sizeof(aom_svc_params_t)) {
            aom_svc_params_t params;
            memset(&params, 0, sizeof(params));
            memcpy(&params, data, sizeof(params));
            
            // Basic sanity checks to avoid immediate crashes on invalid layer counts
            if (params.number_spatial_layers < 1) params.number_spatial_layers = 1;
            if (params.number_temporal_layers < 1) params.number_temporal_layers = 1;
            if (params.number_spatial_layers > AOM_MAX_SS_LAYERS) params.number_spatial_layers = AOM_MAX_SS_LAYERS;
            if (params.number_temporal_layers > AOM_MAX_TS_LAYERS) params.number_temporal_layers = AOM_MAX_TS_LAYERS;

            aom_codec_control(codec, ctrl_id, &params);
        }
    } else if (ctrl_id == AV1E_ENABLE_RATE_GUIDE_DELTAQ) {
        // Expects an int (0 or 1)
        if (size >= sizeof(int)) {
            int val = *(const int*)data;
            aom_codec_control(codec, ctrl_id, val);
        }
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_224(const uint8_t *Data, size_t Size) {
    if (!Data || Size < 1) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Initialize configuration
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set minimal valid config
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;

    // Initialize codec
    err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // We will split the input data to try multiple control calls or just one
    // Strategy: Use the first byte to select the control ID, rest as arguments
    int ctrl_id = Data[0] % 6;
    
    // Map 0-5 to the specific control IDs
    int target_ids[] = {
        AV1E_SET_ENABLE_QM,
        AV1E_SET_QM_MAX,
        AV1E_SET_NOISE_SENSITIVITY,
        AV1E_SET_DELTAQ_STRENGTH,
        AV1E_SET_SVC_PARAMS,
        AV1E_ENABLE_RATE_GUIDE_DELTAQ
    };

    int selected_id = target_ids[ctrl_id];
    
    // Pass the rest of the data as the argument payload
    fuzz_codec_control(&codec, selected_id, NULL, 0, Data + 1, Size - 1);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}