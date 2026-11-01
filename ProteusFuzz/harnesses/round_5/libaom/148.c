// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"
#include "aom_integer.h"

static int read_int(const uint8_t *d, size_t s) {
    if (s < sizeof(int)) return 0;
    int v;
    memcpy(&v, d, sizeof(int));
    return v;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val = 0;
    int ret = 0;

    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_DIFF_WTD_COMP
    if (size >= sizeof(int)) {
        val = read_int(data, size);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);
        (void)ret;
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Use aom_film_grain_params_t if available, otherwise skip or use void*
    #ifdef AOM_FILM_GRAIN_PARAMS_T_DEFINED
    if (size >= sizeof(int)) {
        aom_film_grain_params_t fg_params;
        memset(&fg_params, 0, sizeof(fg_params));
        
        fg_params.grain_seed = read_int(data, size);
        if (size > 1) fg_params.grain_scale = (uint8_t)(data[0] % 256);
        if (size > 2) fg_params.overall_scale = (uint8_t)(data[1] % 256);
        fg_params.grain_scale_shift = 0;
        
        ret = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &fg_params);
        (void)ret;
    }
    #else
    // Fallback if struct is not defined in this build
    if (size >= sizeof(int)) {
        // Try to pass a dummy pointer if the macro expects a struct pointer
        // This might fail at runtime if the struct is required, but avoids compile error
        // We allocate a buffer to simulate the struct size roughly
        void *dummy = malloc(256);
        if (dummy) {
            memset(dummy, 0, 256);
            ret = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, dummy);
            (void)ret;
            free(dummy);
        }
    }
    #endif

    // 3. aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS
    if (size >= sizeof(int)) {
        int num_op;
        ret = aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_op);
        (void)ret;
        (void)num_op;
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
    if (size >= sizeof(int)) {
        val = read_int(data, size);
        ret = aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);
        (void)ret;
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (size >= sizeof(int)) {
        val = read_int(data, size);
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
        (void)ret;
    }

    // 6. aom_codec_control_typechecked_AV1_GET_ACCOUNTING
    if (size >= sizeof(int)) {
        void *acc_buf = malloc(256);
        if (acc_buf) {
            memset(acc_buf, 0, 256);
            ret = aom_codec_control(ctx, AV1_GET_ACCOUNTING, acc_buf);
            (void)ret;
            free(acc_buf);
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic for encoder controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}