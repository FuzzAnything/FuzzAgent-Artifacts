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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

int LLVMFuzzerTestOneInput_304(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    const aom_codec_iface_t *iface;

    // Get AV1 encoder interface first
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize configuration with interface
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Use fuzzer data to configure encoder settings
    cfg.g_w = (Data[0] % 128) + 16; // Width
    cfg.g_h = (Data[1] % 128) + 16; // Height
    cfg.g_timebase.num = (Size > 2) ? Data[2] % 100 : 1;
    cfg.g_timebase.den = (Size > 3) ? Data[3] % 100 : 1;
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1;
    
    cfg.rc_target_bitrate = (Size > 4) ? Data[4] * 1000 : 1000;
    cfg.g_error_resilient = (Size > 5) ? Data[5] % 2 : 0;

    // Initialize the encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Extract control IDs from input to fuzz various states
    int control_id = 0;
    int value = 0;

    if (Size > 6) {
        int selector = Data[6] % 6;
        switch (selector) {
            case 0:
                // AV1E_SET_ENABLE_OVERLAY
                control_id = AV1E_SET_ENABLE_OVERLAY;
                value = (Size > 7) ? Data[7] % 2 : 0;
                break;
            case 1:
                // AV1E_SET_MV_COST_UPD_FREQ
                control_id = AV1E_SET_MV_COST_UPD_FREQ;
                value = (Size > 7) ? (Data[7] % 10) + 1 : 1;
                break;
            case 2:
                // AV1E_SET_FP_MT
                control_id = AV1E_SET_FP_MT;
                value = (Size > 7) ? Data[7] % 2 : 0;
                break;
            case 3:
                // AV1E_SET_CHROMA_SUBSAMPLING_X
                control_id = AV1E_SET_CHROMA_SUBSAMPLING_X;
                value = (Size > 7) ? Data[7] % 2 : 0;
                break;
            case 4:
                // AV1E_SET_ENABLE_PALETTE
                control_id = AV1E_SET_ENABLE_PALETTE;
                value = (Size > 7) ? Data[7] % 2 : 0;
                break;
            case 5:
                // AV1E_SET_ENABLE_DIST_8X8 does not exist in standard libaom headers.
                // Using a valid alternative or skipping to avoid compilation error.
                // We will use AV1E_SET_ENABLE_RECT_TX as a fallback for this slot 
                // to maintain the fuzzing logic structure, or simply skip if strict adherence is needed.
                // Given the task is to fuzz the specific list, but the constant is missing,
                // we map it to a valid existing control to ensure the driver compiles and runs.
                control_id = AV1E_SET_ENABLE_RECT_TX; 
                value = (Size > 7) ? Data[7] % 2 : 0;
                break;
            default:
                break;
        }
    }

    // Invoke the control function
    ret = aom_codec_control(&ctx, control_id, value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}