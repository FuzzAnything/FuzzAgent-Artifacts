// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/*
 * Helper to safely initialize an encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    if (!ctx || !iface) {
        return -1;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    // Set some default values to ensure valid configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper to safely destroy an encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

/*
 * Fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Use the first byte to select which control function to test
    int control_id = Data[0] % 6;

    // Use the second byte to determine the value to pass (if applicable)
    int value = Data[1];

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (init_encoder_context(&ctx, iface) != 0) {
        return 0;
    }

    aom_codec_err_t err = AOM_CODEC_OK;

    switch (control_id) {
        case 0:
            // AV1E_SET_QM_Y
            err = aom_codec_control(&ctx, AV1E_SET_QM_Y, value);
            break;
        case 1:
            // AV1E_SET_CDF_UPDATE_MODE
            err = aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, value);
            break;
        case 2:
            // AV1E_SET_DISABLE_TRELLIS_QUANT
            err = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, value);
            break;
        case 3:
            // AV1E_SET_MODE_COST_UPD_FREQ
            err = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, value);
            break;
        case 4:
            // AV1E_SET_AQ_MODE
            err = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, value);
            break;
        case 5:
            // AV1E_SET_QM_MIN
            err = aom_codec_control(&ctx, AV1E_SET_QM_MIN, value);
            break;
        default:
            break;
    }

    // Cleanup
    cleanup_encoder_context(&ctx);

    return 0;
}