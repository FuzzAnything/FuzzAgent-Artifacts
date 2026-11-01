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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_206(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid buffer overflows
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic settings safely
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control ID and value from input data
    int control_id = Data[4] % 6; // Select one of the 6 target functions
    int value = (Data[5] % 100) - 50; // Generate a value in range [-50, 49]

    // Map control_id to actual control constants
    int actual_control;
    switch (control_id) {
        case 0:
            actual_control = AV1E_SET_ENABLE_TX_SIZE_SEARCH;
            break;
        case 1:
            actual_control = AOME_SET_ARNR_STRENGTH;
            break;
        case 2:
            actual_control = AOME_SET_MAX_INTRA_BITRATE_PCT;
            break;
        case 3:
            actual_control = AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS;
            break;
        case 4:
            actual_control = AOME_SET_SHARPNESS;
            break;
        case 5:
            actual_control = AV1E_SET_MODE_COST_UPD_FREQ;
            break;
        default:
            actual_control = AV1E_SET_ENABLE_TX_SIZE_SEARCH;
            break;
    }

    // Invoke the target function
    fuzz_codec_control(&ctx, actual_control, value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}