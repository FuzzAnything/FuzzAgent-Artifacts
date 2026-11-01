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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to read safely
    if (Size < 3) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to configure basic parameters safely
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Map fuzzer data to control IDs and values
    int control_id;
    int value;

    // Select one of the target controls based on input
    switch (Data[0] % 6) {
        case 0:
            control_id = AV1E_SET_ENABLE_DIRECTIONAL_INTRA;
            break;
        case 1:
            control_id = AV1E_SET_ENABLE_INTERINTRA_WEDGE;
            break;
        case 2:
            control_id = AV1E_SET_ENABLE_SMOOTH_INTERINTRA;
            break;
        case 3:
            control_id = AV1E_SET_ENABLE_PAETH_INTRA;
            break;
        case 4:
            control_id = AV1E_SET_ENABLE_DIAGONAL_INTRA;
            break;
        case 5:
            control_id = AV1E_SET_ENABLE_INTERINTRA_COMP;
            break;
        default:
            control_id = AV1E_SET_ENABLE_DIRECTIONAL_INTRA;
    }

    // Use next byte to determine the value (0 or 1 for boolean flags)
    value = (Data[1] % 2);

    // Invoke the control
    fuzz_encoder_controls(&ctx, control_id, value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}