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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

int LLVMFuzzerTestOneInput_228(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    // Use the correct function name provided in the error message
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    // Initialize configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings fuzzily
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.rc_target_bitrate = (Size > 4) ? (Data[4] * 100) : 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    // Initialize codec context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int control_id = 0;
    int value = 0;
    
    if (Size > 0) {
        int selector = Data[0] % 6;
        int val_byte = (Size > 1) ? Data[1] : 0;
        
        switch (selector) {
            case 0: // AV1E_SET_ENABLE_QM
                control_id = AV1E_SET_ENABLE_QM;
                value = val_byte;
                break;
            case 1: // AV1E_SET_QM_MAX
                control_id = AV1E_SET_QM_MAX;
                value = val_byte;
                break;
            case 2: // AV1E_SET_NOISE_SENSITIVITY
                control_id = AV1E_SET_NOISE_SENSITIVITY;
                value = val_byte;
                break;
            case 3: // AV1E_SET_DELTAQ_STRENGTH
                control_id = AV1E_SET_DELTAQ_STRENGTH;
                value = val_byte;
                break;
            case 4: // AV1E_ENABLE_RATE_GUIDE_DELTAQ
                control_id = AV1E_ENABLE_RATE_GUIDE_DELTAQ;
                value = val_byte;
                break;
            case 5: // AV1E_SET_DELTAQ_MODE
                control_id = AV1E_SET_DELTAQ_MODE;
                value = val_byte;
                break;
        }
    }

    // Invoke the control
    aom_codec_err_t err = aom_codec_control(&ctx, control_id, value);
    
    (void)err;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}