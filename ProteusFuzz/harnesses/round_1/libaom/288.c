// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Target APIs:
 * - AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * - AV1E_SET_MATRIX_COEFFICIENTS
 * - AV1E_SET_DELTALF_MODE
 * - AV1E_SET_FILM_GRAIN_TEST_VECTOR
 * - AV1E_SET_COLOR_RANGE
 * - AV1E_SET_MODE_COST_UPD_FREQ
 */

static int fuzzer_init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal configuration to allow initialization
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void fuzzer_cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_288(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    aom_codec_err_t ret;
    int initialized = 0;

    if (!Data || Size == 0) {
        return 0;
    }

    // Initialize encoder
    if (fuzzer_init_encoder(&ctx, &cfg)) {
        initialized = 1;
    } else {
        return 0;
    }

    // Use input data to drive control parameters
    uint32_t val1 = (Size > 0) ? Data[0] : 0;
    uint32_t val2 = (Size > 1) ? Data[1] : 0;
    uint32_t val3 = (Size > 2) ? Data[2] : 0;
    uint32_t val4 = (Size > 3) ? Data[3] : 0;
    uint32_t val5 = (Size > 4) ? Data[4] : 0;
    uint32_t val6 = (Size > 5) ? Data[5] : 0;

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH: expects int
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);
    (void)ret;

    // AV1E_SET_MATRIX_COEFFICIENTS: expects aom_matrix_coefficients_t
    aom_matrix_coefficients_t matrix = (aom_matrix_coefficients_t)val2;
    ret = aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, &matrix);
    (void)ret;

    // AV1E_SET_DELTALF_MODE: expects int
    ret = aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val3);
    (void)ret;

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR: expects void* or specific struct.
    // Since the struct type is not available in the provided headers, 
    // we pass a dummy buffer or NULL to trigger the control path safely.
    // In a real scenario with full headers, we would populate the struct.
    // Here we pass a small buffer on stack to avoid NULL pointer issues if the API expects data.
    uint8_t dummy_fg_data[64] = {0};
    if (Size > 10) {
        memcpy(dummy_fg_data, Data + 10, Size > 64 ? 64 : Size - 10);
    }
    ret = aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, dummy_fg_data);
    (void)ret;

    // AV1E_SET_COLOR_RANGE: expects aom_color_range_t
    aom_color_range_t color_range = (aom_color_range_t)val5;
    ret = aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, &color_range);
    (void)ret;

    // AV1E_SET_MODE_COST_UPD_FREQ: expects int
    ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val6);
    (void)ret;

    // Cleanup
    fuzzer_cleanup_encoder(&ctx);

    return 0;
}