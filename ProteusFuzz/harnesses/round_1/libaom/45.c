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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * 2. AV1E_SET_DISABLE_TRELLIS_QUANT
 * 3. AV1E_SET_DELTALF_MODE
 * 4. AV1E_SET_ENABLE_TPL_MODEL
 * 5. AV1E_SET_CDF_UPDATE_MODE
 * 6. AV1E_SET_MODE_COST_UPD_FREQ
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;
    uint8_t w_val, h_val;

    if (!ctx || !data || size == 0) {
        return -1;
    }

    /* Get the AV1 encoder interface using the correct function name */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    /* Safely use fuzzed data to set config parameters with bounds checking */
    w_val = (size > 0) ? data[0] : 0;
    h_val = (size > 1) ? data[1] : 0;

    cfg.g_w = (w_val % 16) + 16;
    cfg.g_h = (h_val % 16) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    uint8_t val;

    if (Size < 1) {
        return 0;
    }

    /* Initialize encoder */
    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    val = Data[0];

    /*
     * 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
     * Expected: integer (0 or 1)
     */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val & 1);
    (void)ret;

    /*
     * 2. AV1E_SET_DISABLE_TRELLIS_QUANT
     * Expected: integer (0 or 1)
     */
    ret = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val & 1);
    (void)ret;

    /*
     * 3. AV1E_SET_DELTALF_MODE
     * Expected: enum aom_deltalf_mode_t (0-2)
     */
    ret = aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val % 3);
    (void)ret;

    /*
     * 4. AV1E_SET_ENABLE_TPL_MODEL
     * Expected: integer (0 or 1)
     */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val & 1);
    (void)ret;

    /*
     * 5. AV1E_SET_CDF_UPDATE_MODE
     * Expected: enum aom_cdf_update_mode_t (0-2)
     */
    ret = aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, val % 3);
    (void)ret;

    /*
     * 6. AV1E_SET_MODE_COST_UPD_FREQ
     * Expected: integer (0-30)
     */
    ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val % 31);
    (void)ret;

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}