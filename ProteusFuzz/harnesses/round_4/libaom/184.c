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
#include <stdio.h>
#include <limits.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings fuzzily
    cfg.g_w = (Data[0] % 4096) + 16;
    cfg.g_h = (Data[1] % 4096) + 16;
    cfg.g_threads = (Size > 2 ? Data[2] % 16 : 1);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Size > 3 ? Data[3] : 1);

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Define the control IDs to fuzz based on the target list
    // Using the standard AOM_CTRL_ macros defined in aom/aomcx.h
    // These macros expand to integers representing the control commands
    int controls[] = {
        AV1E_SET_GF_MAX_PYRAMID_HEIGHT,
        AV1E_SET_GF_MIN_PYRAMID_HEIGHT,
        AV1E_SET_MIN_GF_INTERVAL,
        AV1E_SET_DISABLE_TRELLIS_QUANT,
        AV1E_SET_MAX_GF_INTERVAL,
        AV1E_SET_ENABLE_TPL_MODEL
    };
    int num_controls = sizeof(controls) / sizeof(controls[0]);

    // Iterate through controls and try diverse values
    for (int i = 0; i < num_controls; i++) {
        int control_id = controls[i];
        int32_t value = 0;

        if (Size > (size_t)(i + 1)) {
            // Generate a value from input data
            int raw_val = Data[i + 1];
            if (control_id == AV1E_SET_DISABLE_TRELLIS_QUANT ||
                control_id == AV1E_SET_ENABLE_TPL_MODEL) {
                // Boolean-like settings: 0 or 1
                value = (raw_val % 2);
            } else {
                // Integer settings: use a wider range but keep it reasonable
                if (Size > (size_t)(i + 2)) {
                    value = (Data[i + 1] << 8) | Data[i + 2];
                } else {
                    value = raw_val;
                }
                // Clamp to reasonable bounds to avoid immediate crashes on invalid params
                if (value < 0) value = 0;
                if (value > 1000) value = 1000;
            }
        }

        // Invoke the control
        aom_codec_control(&ctx, control_id, value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}