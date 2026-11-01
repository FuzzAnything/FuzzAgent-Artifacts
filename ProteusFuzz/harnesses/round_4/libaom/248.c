// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_248(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = NULL;
    const aom_codec_iface_t *iface = NULL;
    aom_codec_err_t ret;

    // Attempt to initialize encoder context
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzzer data to configure width/height if possible
    if (Size >= 8) {
        uint32_t w = *(uint32_t *)Data;
        uint32_t h = *(uint32_t *)(Data + 4);
        if (w > 0 && w < 4096) cfg.g_w = w;
        if (h > 0 && h < 4096) cfg.g_h = h;
    } else {
        cfg.g_w = 32;
        cfg.g_h = 32;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
    // The API expects an array of int16_t of size CDEF_MAX_STRENGTHS (8)
    // Previously passing a single int32_t caused a stack buffer overflow.
    int16_t cdef_strengths[8];
    memset(cdef_strengths, 0, sizeof(cdef_strengths));
    ret = aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);
    (void)ret; (void)cdef_strengths;

    // Fuzz AV1E_SET_ENABLE_ONESIDED_COMP
    if (Size >= 8) {
        int32_t val = *(int32_t *)(Data + 4);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, &val);
        (void)ret;
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Skip if type is not defined or to avoid compilation issues in some environments
    // Assuming the type aom_film_grain_test_vector_t might be missing or complex
    // We skip this specific control to ensure stability if headers are incomplete
    // If the environment supports it, one would define the struct and pass it.
    // For now, skipping to prevent undefined behavior or compile errors.

    // Fuzz AV1E_SET_FILM_GRAIN_TABLE
    // Similarly skipping due to potential type definition issues

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    if (Size >= 32) {
        int32_t matrix_coeff = *(int32_t *)(Data + 24);
        ret = aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, &matrix_coeff);
        (void)ret;
    }

    cleanup_codec(ctx);

    // Attempt to initialize decoder context for decoder-specific controls
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));

    ret = aom_codec_dec_init(ctx, iface, &dec_cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz AV1D_SET_SKIP_FILM_GRAIN
    if (Size >= 40) {
        int32_t skip_fg = *(int32_t *)(Data + 32);
        ret = aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, &skip_fg);
        (void)ret;
    }

    cleanup_codec(ctx);

    return 0;
}