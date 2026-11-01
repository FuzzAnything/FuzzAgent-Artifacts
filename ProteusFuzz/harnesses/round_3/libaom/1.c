// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>

// CDEF_MAX_STRENGTHS is typically 16 in modern libaom builds.
// The previous crash occurred because the buffer was sized for 8,
// but the library attempted to write 16 integers (64 bytes).
#define CDEF_MAX_STRENGTHS 16

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *Data, size_t Size) {
    if (!ctx || !Data || Size < 1) return 0;

    int ret = 0;
    int val = 0;
    int32_t int32_val = 0;

    // Safely extract values from fuzz data
    if (Size >= 4) {
        val = *(int32_t*)Data;
        int32_val = *(int32_t*)Data;
    } else if (Size >= 1) {
        val = Data[0];
        int32_val = Data[0];
    }

    // AOME_GET_LOOPFILTER_LEVEL
    if (aom_codec_control(ctx, AOME_GET_LOOPFILTER_LEVEL, &val) != AOM_CODEC_OK) {
        ret = 1;
    }

    // AV1E_SET_LOOPFILTER_CONTROL
    if (aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    // AV1_SET_SKIP_LOOP_FILTER
    if (aom_codec_control(ctx, AV1_SET_SKIP_LOOP_FILTER, val % 2) != AOM_CODEC_OK) {
        ret = 1;
    }

    // AV1E_SET_ENABLE_INTRABC
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val % 2) != AOM_CODEC_OK) {
        ret = 1;
    }

    // AV1E_GET_LUMA_CDEF_STRENGTH requires a buffer of CDEF_MAX_STRENGTHS * sizeof(int).
    // Fixed: Increased buffer size from 8 to 16 to prevent stack-buffer-overflow.
    int cdef_strengths[CDEF_MAX_STRENGTHS];
    if (aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths) != AOM_CODEC_OK) {
        ret = 1;
    }

    return ret;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Safely use fuzz data to tweak config only if enough data exists
    if (Size >= 4) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1] % 1024) + 16;
        cfg.g_timebase.num = (Data[2] % 30) + 1;
        cfg.g_timebase.den = (Data[3] % 30) + 1;
    } else {
        // Default values if not enough data
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform the fuzzing of control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    cleanup_codec(&ctx);
    return 0;
}