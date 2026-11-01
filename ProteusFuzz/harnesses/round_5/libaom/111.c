// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    aom_codec_ctx_t *local_ctx;
    int width, height, bit_depth;

    if (!ctx || !data || size < 12) {
        return 0;
    }

    // Use the correct function name for AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    local_ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!local_ctx) {
        return 0;
    }
    memset(local_ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(local_ctx);
        return 0;
    }

    // Derive parameters from fuzz input
    width = (data[0] % 16 + 1) * 16;
    if (width > 4096) width = 4096;
    height = (data[1] % 16 + 1) * 16;
    if (height > 4096) height = 4096;
    bit_depth = (data[2] % 2) + 8; // 8 or 10

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (data[3] * 256 + data[4]) + 1;
    cfg.g_threads = (data[5] % 4) + 1;
    cfg.g_error_resilient = (data[6] % 3);

    res = aom_codec_enc_init(local_ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(local_ctx);
        return 0;
    }

    *ctx = local_ctx;
    return 1;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;
    int val;
    int32_t int_val;
    void *ptr_val;

    if (Size < 1) return 0;

    // Initialize encoder
    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Ensure we have enough data for various control arguments
    if (Size < 16) {
        cleanup_codec(ctx);
        return 0;
    }

    // 1. AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Typically takes an int or pointer. We try passing an int derived from data.
    val = Data[0] & 0xFF;
    res = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val);
    (void)res;

    // 2. AV1E_SET_LOOPFILTER_CONTROL
    // Usually takes a pointer to loopfilter control struct or int.
    // We try passing an integer value.
    val = (Data[1] << 8) | Data[2];
    res = aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, val);
    (void)res;

    // 3. AV1E_SET_ENABLE_CFL_INTRA
    // Boolean flag (0 or 1)
    val = Data[3] & 0x1;
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    (void)res;

    // 4. AV1E_SET_FILM_GRAIN_TABLE
    // Usually takes a pointer to a table. We pass a pointer to our input data if size allows.
    if (Size >= 32) {
        ptr_val = (void *)(Data + 10);
        res = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TABLE, ptr_val);
        (void)res;
    }

    // 5. AV1E_SET_ENABLE_WARPED_MOTION
    // Boolean flag
    val = Data[4] & 0x1;
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, val);
    (void)res;

    // 6. AV1E_GET_LUMA_CDEF_STRENGTH
    // Usually returns a value via pointer or int. We pass a dummy pointer.
    int_val = 0;
    res = aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &int_val);
    (void)res;

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}