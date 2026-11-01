// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target APIs:
 * - aom_codec_control_typechecked_AV1E_SET_QUANT_B_ADAPT
 * - aom_codec_control_typechecked_AV1E_SET_MIN_GF_INTERVAL
 * - aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS
 * - aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY
 * - aom_codec_control_typechecked_AV1E_GET_BASELINE_GF_INTERVAL
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int width, height;

    if (size < 4) return -1;

    width = data[0] + 1;
    height = data[1] + 1;
    if (width > 4096 || height > 4096) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_225(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    int ret;

    if (Size == 0) return 0;

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    /*
     * Fuzz AV1E_SET_QUANT_B_ADAPT
     * Expects an integer value (0 or 1 typically)
     */
    if (Size >= 1) {
        int val = Data[0] & 1;
        ret = aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, val);
        (void)ret;
    }

    /*
     * Fuzz AV1E_SET_MIN_GF_INTERVAL
     * Expects an integer value (frame interval)
     */
    if (Size >= 2) {
        int val = (Data[1] % 32) + 1;
        ret = aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val);
        (void)ret;
    }

    /*
     * Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
     * Expects an enum value (AOM_TRANSFER_CHARACTERISTICS_*)
     */
    if (Size >= 3) {
        int val = Data[2] % 16;
        ret = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
        (void)ret;
    }

    /*
     * Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
     * Expects an integer (0 or 1)
     */
    if (Size >= 4) {
        int val = Data[3] & 1;
        ret = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
        (void)ret;
    }

    /*
     * Fuzz AV1E_GET_BASELINE_GF_INTERVAL
     * Expects a pointer to store the result
     */
    if (Size >= 5) {
        int result = 0;
        ret = aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &result);
        (void)ret;
        (void)result;
    }

    /*
     * Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
     * Expects an integer (0 or 1)
     */
    if (Size >= 6) {
        int val = Data[5] & 1;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);
        (void)ret;
    }

    cleanup_encoder(&ctx);
    return 0;
}