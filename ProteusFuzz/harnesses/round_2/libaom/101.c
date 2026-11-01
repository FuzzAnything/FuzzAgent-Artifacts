// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_MATRIX_COEFFICIENTS
 * - AV1E_SET_COLOR_PRIMARIES
 * - AV1E_SET_COLOR_RANGE
 * - AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * - AV1E_SET_INTRA_DCT_ONLY
 * - AV1E_SET_ENABLE_GLOBAL_MOTION
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    /* Need at least 3 bytes to extract 3 values safely */
    if (Size < 3) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Extract values safely from the input buffer */
    uint8_t val1 = Data[0];
    uint8_t val2 = Data[1];
    uint8_t val3 = Data[2];

    /* AV1E_SET_MATRIX_COEFFICIENTS: expects aom_matrix_coefficients_t */
    aom_matrix_coefficients_t mat_coeff = (aom_matrix_coefficients_t)val1;
    aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, mat_coeff);

    /* AV1E_SET_COLOR_PRIMARIES: expects aom_color_primaries_t */
    aom_color_primaries_t cp = (aom_color_primaries_t)val2;
    aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, cp);

    /* AV1E_SET_COLOR_RANGE: expects aom_color_range_t */
    aom_color_range_t cr = (aom_color_range_t)val3;
    aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, cr);

    /* AV1E_SET_ENABLE_TX_SIZE_SEARCH: expects int (0 or 1) */
    int enable_tx_size_search = (val1 & 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    /* AV1E_SET_INTRA_DCT_ONLY: expects int (0 or 1) */
    int intra_dct_only = (val2 & 1);
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);

    /* AV1E_SET_ENABLE_GLOBAL_MOTION: expects int (0 or 1) */
    int enable_global_motion = (val3 & 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);

    cleanup_encoder(&ctx);

    return 0;
}