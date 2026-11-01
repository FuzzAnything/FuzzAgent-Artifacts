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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_201(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use the correct function to get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic encoder settings
    cfg.g_w = Data[0] + 1;
    cfg.g_h = Data[1] + 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control ID and value from input data
    int control_id = Data[2] % 10; // Limit to 0-9 for safety
    int value = Data[3] % 256;

    // Map control_id to actual control constants
    // The constants are defined in aom/aomcx.h as AV1E_*
    switch (control_id) {
        case 0:
            // AV1E_SET_ENABLE_INTERINTRA_WEDGE
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);
            break;
        case 1:
            // AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
            fuzz_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, value);
            break;
        case 2:
            // AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
            fuzz_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, value);
            break;
        case 3:
            // AV1E_SET_SVC_FRAME_DROP_MODE
            fuzz_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, value);
            break;
        case 4:
            // AV1E_SET_ENABLE_CHROMA_DELTAQ
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, value);
            break;
        case 5:
            // AV1E_SET_MAX_PARTITION_SIZE
            fuzz_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, value);
            break;
        default:
            // Try other valid controls or ignore
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}