// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
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

/*
 * Fuzz driver for libaom encoder control functions.
 * Since the specific signatures for aom_codec_control_typechecked_* were not provided,
 * this driver assumes they are wrappers around aom_codec_control with specific
 * control IDs (AV1E_SET_...).
 *
 * We will initialize an AV1 encoder and attempt to call these controls
 * with values derived from the fuzz input.
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 1;
    }

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int set_control_with_value(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    (void)ret; // Ignore return for fuzzing to explore states, but in real code check it.
    return 0;
}

int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret;

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // We assume the control IDs are defined in aom_encoder.h or similar.
    // Common AV1 encoder controls:
    // AV1E_SET_QM_Y, AV1E_SET_QM_U, AV1E_SET_AQ_MODE, 
    // AV1E_SET_DISABLE_TRELLIS_QUANT, AV1E_SET_QM_MIN, AV1E_SET_CDF_UPDATE_MODE
    
    // Extract values from input data
    int val1 = (Size > 0) ? Data[0] : 0;
    int val2 = (Size > 1) ? Data[1] : 0;
    int val3 = (Size > 2) ? Data[2] : 0;
    int val4 = (Size > 3) ? Data[3] : 0;
    int val5 = (Size > 4) ? Data[4] : 0;
    int val6 = (Size > 5) ? Data[5] : 0;

    // Try to invoke the controls. 
    // Note: The function names in the prompt (aom_codec_control_typechecked_...) 
    // are likely internal or generated wrappers. The standard API is aom_codec_control.
    // We will use the standard API with the expected control IDs.
    // If the specific typechecked functions exist as global symbols, they might be called directly,
    // but without signatures, we rely on the standard control mechanism which these likely wrap.
    
    // Attempt to call standard controls corresponding to the requested functions
    // AV1E_SET_QM_Y
    if (Size >= 1) {
        set_control_with_value(&ctx, AV1E_SET_QM_Y, val1);
    }
    
    // AV1E_SET_QM_U
    if (Size >= 2) {
        set_control_with_value(&ctx, AV1E_SET_QM_U, val2);
    }

    // AV1E_SET_AQ_MODE
    if (Size >= 3) {
        set_control_with_value(&ctx, AV1E_SET_AQ_MODE, val3);
    }

    // AV1E_SET_DISABLE_TRELLIS_QUANT
    if (Size >= 4) {
        set_control_with_value(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val4);
    }

    // AV1E_SET_QM_MIN
    if (Size >= 5) {
        set_control_with_value(&ctx, AV1E_SET_QM_MIN, val5);
    }

    // AV1E_SET_CDF_UPDATE_MODE
    if (Size >= 6) {
        set_control_with_value(&ctx, AV1E_SET_CDF_UPDATE_MODE, val6);
    }

    // Additional exploration: try random combinations or out-of-bounds values
    if (Size > 6) {
        int random_val = *(int*)(Data + 6);
        set_control_with_value(&ctx, AV1E_SET_QM_Y, random_val);
        set_control_with_value(&ctx, AV1E_SET_AQ_MODE, random_val);
    }

    cleanup_encoder(&ctx);
    return 0;
}