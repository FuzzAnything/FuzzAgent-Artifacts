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

/*
 * Fuzz driver for libaom AV1 encoder controls.
 * Targets:
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_ENABLE_DIFF_WTD_COMP
 * - AV1E_SET_ENABLE_DIST_WTD_COMP
 * - AV1E_SET_ENABLE_FLIP_IDTX
 * - AV1E_SET_QUANT_B_ADAPT
 * - AV1E_SET_ENABLE_ANGLE_DELTA
 */

static int try_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, ctrl_id, value) == AOM_CODEC_OK;
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int value;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Use the correct function to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to seed configuration parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_error_resilient = 0;

    // Initialize the encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract a value from the input data to use as the control parameter
    if (Size > 0) {
        value = Data[0] % 2; // 0 or 1 for boolean flags
    } else {
        value = 0;
    }

    // Attempt to invoke the target controls using standard enum constants
    (void)try_control(&ctx, AV1E_SET_INTER_DCT_ONLY, value);
    (void)try_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, value);
    (void)try_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value);
    (void)try_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    (void)try_control(&ctx, AV1E_SET_QUANT_B_ADAPT, value);
    (void)try_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}