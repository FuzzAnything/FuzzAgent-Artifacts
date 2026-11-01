// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set minimal valid configuration to ensure initialization succeeds
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input data
    int ctrl_id = (int)Data[0];
    int value = (int)Data[1];
    int value2 = (int)Data[2];
    int value3 = (int)Data[3];

    // Define a list of safe, known control IDs to test
    // These are the specific AV1 encoder controls mentioned in the task
    static const int safe_controls[] = {
        AV1E_SET_ENABLE_QM,
        AV1E_SET_DISABLE_TRELLIS_QUANT,
        AV1E_SET_ENABLE_REF_FRAME_MVS,
        AV1E_SET_FRAME_PERIODIC_BOOST,
        AV1E_SET_ENABLE_TPL_MODEL,
        AV1E_SET_ENABLE_FILTER_INTRA,
        0 // Sentinel
    };

    // Map input to a specific control or a random safe one
    int selected_ctrl = 0;
    int selected_val = value;

    if (ctrl_id < 6) {
        selected_ctrl = safe_controls[ctrl_id];
        selected_val = value;
    } else {
        // Pick a random control from the safe list based on input
        int idx = (ctrl_id + value2) % 6;
        selected_ctrl = safe_controls[idx];
        selected_val = value3;
    }

    // Invoke the control
    aom_codec_control(&ctx, selected_ctrl, selected_val);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}