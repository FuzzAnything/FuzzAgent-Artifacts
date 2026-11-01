// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int try_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to read safely
    if (Size < 12) return 0;

    // Extract parameters from input data safely
    uint32_t seed = *(const uint32_t *)Data;
    uint32_t width = (Data[4] % 1024) + 16;
    uint32_t height = (Data[5] % 1024) + 16;
    uint32_t fps_num = (Data[6] % 120) + 1;
    uint32_t fps_den = (Data[7] % 120) + 1;
    uint32_t threads = (Data[8] % 8) + 1;
    uint32_t g_error_resilient = (Data[9] % 2);
    uint32_t rc_end_usage = (Data[10] % 3); // 0: VBR, 1: CBR, 2: Q
    uint32_t target_bitrate = (Data[11] % 10000) + 1000;

    // Initialize encoder config
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(NULL, &cfg, 0);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = threads;
    cfg.g_error_resilient = g_error_resilient;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = target_bitrate;
    
    // Fix: Use correct enum values
    if (rc_end_usage == 0) cfg.rc_end_usage = AOM_VBR;
    else if (rc_end_usage == 1) cfg.rc_end_usage = AOM_CBR;
    else cfg.rc_end_usage = AOM_Q;
    
    cfg.g_pass = AOM_RC_ONE_PASS;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Define control IDs based on libaom conventions
    // These correspond to the macros mentioned in the prompt, mapped to their standard integer values
    // AV1E_SET_COEFF_COST_UPD_FREQ = 22
    // AV1E_SET_FRAME_PERIODIC_BOOST = 23
    // AV1E_SET_ENABLE_REF_FRAME_MVS = 24
    // AOME_SET_ENABLEAUTOBWDREF = 25
    // AV1E_SET_ALLOW_WARPED_MOTION = 26
    // AV1E_SET_S_FRAME_MODE = 27
    int controls[] = {
        22, // AV1E_SET_COEFF_COST_UPD_FREQ
        23, // AV1E_SET_FRAME_PERIODIC_BOOST
        24, // AV1E_SET_ENABLE_REF_FRAME_MVS
        25, // AOME_SET_ENABLEAUTOBWDREF
        26, // AV1E_SET_ALLOW_WARPED_MOTION
        27  // AV1E_SET_S_FRAME_MODE
    };

    int num_controls = sizeof(controls) / sizeof(controls[0]);

    for (int i = 0; i < num_controls; i++) {
        int val = (seed + i) % 10; 
        try_codec_control(&ctx, controls[i], val);
    }

    // Try with larger values if the input allows (ensure we don't overflow)
    if (Size > 15) {
        int val_large = *(const int32_t *)(Data + 12);
        for (int i = 0; i < num_controls; i++) {
            try_codec_control(&ctx, controls[i], val_large);
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}