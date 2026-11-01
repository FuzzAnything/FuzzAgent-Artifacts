// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return -1;
    
    // Cast value to unsigned int for control functions
    unsigned int uval = (unsigned int)value;
    
    // Invoke the control function
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, uval);
    
    // Return success if no error
    return (ret == AOM_CODEC_OK) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int width, height, bit_depth, fps_num, fps_den;
    int control_id;
    int value;

    // Parse input data to configure encoder
    if (Size < 20) return 0;
    
    width = Data[0] * 2 + 16;
    height = Data[1] * 2 + 16;
    bit_depth = (Data[2] % 3) + 8; // 8, 10, or 12
    fps_num = Data[3] + 1;
    fps_den = Data[4] + 1;
    control_id = Data[5] % 6; // Select one of the 6 control IDs
    value = Data[6];

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Set default configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Configure encoder settings
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 0;
    cfg.rc_overshoot_pct = 0;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    // Use AOM_USAGE_REALTIME if available, otherwise default to 0
    cfg.g_usage = AOM_USAGE_REALTIME;

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Map control_id to actual control constants
    int actual_control_id;
    switch (control_id) {
        case 0:
            actual_control_id = AV1E_SET_INTER_DCT_ONLY;
            break;
        case 1:
            actual_control_id = AV1E_SET_ENABLE_CDEF;
            break;
        case 2:
            actual_control_id = AV1E_SET_ENABLE_KEYFRAME_FILTERING;
            break;
        case 3:
            actual_control_id = AV1E_SET_LOOPFILTER_CONTROL;
            break;
        case 4:
            actual_control_id = AV1E_SET_SKIP_POSTPROC_FILTERING;
            break;
        case 5:
            actual_control_id = AV1E_SET_ENABLE_INTRABC;
            break;
        default:
            actual_control_id = AV1E_SET_INTER_DCT_ONLY;
            break;
    }

    // Fuzz the control function with different values
    for (int i = 0; i < 10; i++) {
        int fuzzed_value = (i < Size) ? Data[i] : (i % 256);
        fuzz_encoder_control(&ctx, actual_control_id, fuzzed_value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}