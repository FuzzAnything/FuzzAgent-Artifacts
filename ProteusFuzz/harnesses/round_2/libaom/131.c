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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzz data to configure encoder settings
    int width = Data[0] + 16;
    int height = Data[1] + 16;
    int fps_num = Data[2] + 1;
    int fps_den = Data[3] + 1;
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000 + (Size % 10000);
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control IDs and values from input data
    int control_id = Data[4] % 1000;
    int value = Data[5] % 100;

    // Map control_id to actual control types based on available functions
    int actual_control = 0;
    int64_t int64_value = value;

    switch (control_id) {
        case 0: // AV1E_SET_MAX_PARTITION_SIZE
            actual_control = AV1E_SET_MAX_PARTITION_SIZE;
            int64_value = (int64_t)(value * 100 + 1);
            break;
        case 1: // AV1E_SET_FP_MT_UNIT_TEST
            actual_control = AV1E_SET_FP_MT_UNIT_TEST;
            int64_value = value;
            break;
        case 2: // AV1E_SET_ENABLE_REF_FRAME_MVS
            actual_control = AV1E_SET_ENABLE_REF_FRAME_MVS;
            int64_value = value % 2;
            break;
        case 3: // AV1E_SET_ENABLE_ONESIDED_COMP
            actual_control = AV1E_SET_ENABLE_ONESIDED_COMP;
            int64_value = value % 2;
            break;
        case 4: // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
            actual_control = AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE;
            int64_value = value % 2;
            break;
        case 5: // AV1E_SET_SVC_FRAME_DROP_MODE
            actual_control = AV1E_SET_SVC_FRAME_DROP_MODE;
            int64_value = value % 3;
            break;
        default:
            actual_control = AV1E_SET_MAX_PARTITION_SIZE;
            int64_value = (int64_t)(value * 100 + 1);
            break;
    }

    // Invoke the control function
    aom_codec_control(&ctx, actual_control, int64_value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}