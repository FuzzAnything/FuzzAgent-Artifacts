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
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

static int test_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    
    // Cast value to void* as the control API expects a pointer to the value
    // Note: For integer controls, we pass the address of the value
    ret = aom_codec_control(ctx, control_id, &value);
    
    // We don't return error immediately to allow exploration of different states
    // even if some controls fail with invalid values
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int value;
    
    if (Size < 3) {
        return 0;
    }

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to generate control values
    // We'll use the first few bytes to determine which control to test and the value
    int control_choice = Data[0] % 6; // 0-5 for the 6 controls
    int value_choice = (Data[1] << 8) | Data[2];
    
    // Limit value to reasonable range for most controls
    if (value_choice > 255) {
        value_choice = 255;
    }
    
    value = value_choice;

    switch (control_choice) {
        case 0:
            // AV1E_SET_COEFF_COST_UPD_FREQ
            test_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, value);
            break;
        case 1:
            // AV1E_SET_DELTAQ_MODE
            test_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, value);
            break;
        case 2:
            // AV1E_SET_ENABLE_TPL_MODEL
            test_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, value);
            break;
        case 3:
            // AV1E_SET_ENABLE_QM
            test_codec_control(&ctx, AV1E_SET_ENABLE_QM, value);
            break;
        case 4:
            // AV1E_SET_QM_MAX
            test_codec_control(&ctx, AV1E_SET_QM_MAX, value);
            break;
        case 5:
            // AV1E_SET_DENOISE_BLOCK_SIZE
            test_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, value);
            break;
        default:
            break;
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}