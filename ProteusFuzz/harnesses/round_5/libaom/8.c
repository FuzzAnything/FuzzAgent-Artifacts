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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets specific AV1 encoder control IDs.
 */

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *data) {
    if (!ctx || !ctx->priv) return 0;
    
    /* 
     * Invoke the control function. 
     * The data pointer is passed directly. The internal implementation 
     * will interpret the bytes based on the control_id.
     */
    aom_codec_err_t res = aom_codec_control(ctx, control_id, data);
    (void)res; 
    return 1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Initialize encoder context */
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    /* Use fuzzing input to configure basic parameters */
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    
    /* Fix: Use g_timebase instead of g_time_base */
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    cfg.g_threads = (Size % 8) + 1;
    cfg.rc_target_bitrate = (Size % 1000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* 
     * Map the requested control IDs.
     * Fix: Use AV1E_... identifiers as indicated by the compiler errors.
     */
    int control_ids[] = {
        AV1E_SET_FP_MT_UNIT_TEST,           /* Corresponds to AV1E_SET_FP_MT_UNIT_TEST */
        AV1E_SET_FRAME_PERIODIC_BOOST,      /* Corresponds to AV1E_SET_FRAME_PERIODIC_BOOST */
        AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, /* Corresponds to AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE */
        AV1E_SET_SVC_FRAME_DROP_MODE,       /* Corresponds to AV1E_SET_SVC_FRAME_DROP_MODE */
        AV1E_SET_ENABLE_FILTER_INTRA,       /* Corresponds to AV1E_SET_ENABLE_FILTER_INTRA */
        AV1E_SET_QM_MAX                     /* Corresponds to AV1E_SET_QM_MAX */
    };
    
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);
    
    /* 
     * Create a buffer for arguments. 
     * We prepare an int64_t value derived from the input data to pass to the controls.
     */
    int64_t arg_val = 0;
    if (Size >= 8) {
        memcpy(&arg_val, Data, 8);
    } else if (Size >= 4) {
        int32_t tmp;
        memcpy(&tmp, Data, 4);
        arg_val = tmp;
    } else {
        arg_val = (int64_t)(Data[0] * 256 + Data[1]);
    }

    /* Fuzz loop: try each control with the generated value */
    for (int i = 0; i < num_controls; ++i) {
        fuzz_encoder_control(&ctx, control_ids[i], (void*)&arg_val);
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);
    
    return 0;
}