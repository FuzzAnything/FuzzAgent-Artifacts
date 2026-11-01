// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_206(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 4) + 1;
    cfg.g_error_resilient = (Data[6] % 3);

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Control IDs for the target functions
    int control_ids[] = {
        AV1E_SET_ENABLE_SMOOTH_INTERINTRA,
        AV1E_SET_ENABLE_INTERINTER_WEDGE,
        AV1E_SET_ENABLE_MASKED_COMP,
        AV1E_SET_ENABLE_RECT_PARTITIONS,
        AV1E_SET_ENABLE_PALETTE,
        AV1E_SET_QM_MIN
    };
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

    // Fuzz each control with different values
    for (int i = 0; i < num_controls; i++) {
        int value = (Size > (size_t)(i + 8)) ? (int)Data[i + 8] : (i % 2);
        if (control_ids[i] == AV1E_SET_QM_MIN) {
            // Ensure QM_MIN is within valid range [0, 255]
            value = value % 256;
        }
        fuzz_encoder_control(&ctx, control_ids[i], value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}