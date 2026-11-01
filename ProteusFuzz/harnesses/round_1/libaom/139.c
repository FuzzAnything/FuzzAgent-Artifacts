// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static int invoke_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t err;
    err = aom_codec_control(ctx, control_id, value);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_iface_t *iface;
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int control_id;
    int value;

    // Get the AV1 encoder interface first
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config with defaults
    memset(&cfg, 0, sizeof(cfg));
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize the codec context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Use the input data to select a control ID and value
    int idx = Data[0] % 6;
    
    // Define the control IDs based on the task description
    int control_ids[] = {
        AV1E_SET_QM_MAX,
        AOME_SET_ENABLEAUTOBWDREF,
        AV1E_SET_FORCE_VIDEO_MODE,
        AV1E_SET_COEFF_COST_UPD_FREQ,
        AV1E_SET_DELTAQ_MODE,
        AOME_SET_CQ_LEVEL
    };

    // Ensure we have a valid control ID
    if (idx < 0 || idx >= 6) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    control_id = control_ids[idx];

    // Generate a value from the input data
    if (Size > 1) {
        value = Data[1];
    } else {
        value = 0;
    }

    // Try various values to explore program states
    invoke_control(&ctx, control_id, value);
    invoke_control(&ctx, control_id, value & 1); 
    invoke_control(&ctx, control_id, 0);        
    invoke_control(&ctx, control_id, -1);       

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}