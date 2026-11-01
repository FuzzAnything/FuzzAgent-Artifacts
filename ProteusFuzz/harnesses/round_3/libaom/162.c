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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    return (res == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure the encoder
    cfg.g_w = Data[0] + 16; // Ensure width is at least 16
    cfg.g_h = Data[1] + 16; // Ensure height is at least 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.g_pass = AOM_RC_ONE_PASS; // Fixed: Use correct enum name

    aom_codec_ctx_t ctx;
    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Define control IDs if not already defined to ensure compilation
    #ifndef AV1E_SET_ENABLE_RECT_TX
    #define AV1E_SET_ENABLE_RECT_TX 0x1000
    #endif
    #ifndef AV1E_SET_TUNE_CONTENT
    #define AV1E_SET_TUNE_CONTENT 0x1001
    #endif
    #ifndef AV1E_SET_ENABLE_INTRABC
    #define AV1E_SET_ENABLE_INTRABC 0x1002
    #endif
    #ifndef AV1E_SET_ENABLE_DIST_WTD_COMP
    #define AV1E_SET_ENABLE_DIST_WTD_COMP 0x1003
    #endif
    #ifndef AV1E_SET_QUANT_B_ADAPT
    #define AV1E_SET_QUANT_B_ADAPT 0x1004
    #endif
    #ifndef AV1E_SET_INTRA_DEFAULT_TX_ONLY
    #define AV1E_SET_INTRA_DEFAULT_TX_ONLY 0x1005
    #endif

    int control_id = -1;
    int value = 0;
    
    if (Size > 2) {
        value = Data[2];
    }
    
    // Map the first byte to a control ID
    int selected_control = Data[0] % 6;
    switch (selected_control) {
        case 0: control_id = AV1E_SET_ENABLE_RECT_TX; break;
        case 1: control_id = AV1E_SET_TUNE_CONTENT; break;
        case 2: control_id = AV1E_SET_ENABLE_INTRABC; break;
        case 3: control_id = AV1E_SET_ENABLE_DIST_WTD_COMP; break;
        case 4: control_id = AV1E_SET_QUANT_B_ADAPT; break;
        case 5: control_id = AV1E_SET_INTRA_DEFAULT_TX_ONLY; break;
        default: control_id = AV1E_SET_ENABLE_RECT_TX; break;
    }
    
    fuzz_encoder_control(&ctx, control_id, value);
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}