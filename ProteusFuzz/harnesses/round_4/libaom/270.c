// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *data, size_t size) {
    if (!ctx) return 0;
    
    // For SET operations, we might need to pass a value
    // For GET operations, we might need to pass a pointer to store the result
    int ret = 0;
    
    switch (control_id) {
        case AV1E_SET_SCREEN_CONTENT_DETECTION_MODE: {
            int mode = 0;
            if (size >= sizeof(int)) {
                memcpy(&mode, data, sizeof(int));
            }
            ret = aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, mode);
            break;
        }
        case AV1E_SET_ENABLE_DIST_WTD_COMP: {
            int enable = 0;
            if (size >= sizeof(int)) {
                memcpy(&enable, data, sizeof(int));
            }
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, enable);
            break;
        }
        case AV1E_SET_FILM_GRAIN_TEST_VECTOR: {
            // This typically takes a pointer to a struct
            // We'll just pass the data pointer directly if size is sufficient
            if (size > 0) {
                ret = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, data);
            }
            break;
        }
        case AV1E_SET_ENABLE_DUAL_FILTER: {
            int enable = 0;
            if (size >= sizeof(int)) {
                memcpy(&enable, data, sizeof(int));
            }
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, enable);
            break;
        }
        case AV1E_SET_ENABLE_ANGLE_DELTA: {
            int enable = 0;
            if (size >= sizeof(int)) {
                memcpy(&enable, data, sizeof(int));
            }
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, enable);
            break;
        }
        case AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC: {
            int result = 0;
            ret = aom_codec_control(ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &result);
            break;
        }
        default:
            break;
    }
    
    return ret;
}

int LLVMFuzzerTestOneInput_270(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzz data to set some config parameters
    if (Size >= 2) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1] % 1024) + 16;
    }
    
    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    
    // Try to invoke various control functions
    int control_ids[] = {
        AV1E_SET_SCREEN_CONTENT_DETECTION_MODE,
        AV1E_SET_ENABLE_DIST_WTD_COMP,
        AV1E_SET_FILM_GRAIN_TEST_VECTOR,
        AV1E_SET_ENABLE_DUAL_FILTER,
        AV1E_SET_ENABLE_ANGLE_DELTA,
        AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC
    };
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);
    
    // Use fuzz data to select which control to invoke
    int selected_control = 0;
    if (Size >= 1) {
        selected_control = Data[0] % num_controls;
    }
    
    // Invoke the selected control
    fuzz_encoder_control(&ctx, control_ids[selected_control], 
                        Size > 1 ? (void*)&Data[1] : NULL, 
                        Size > 1 ? Size - 1 : 0);
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}