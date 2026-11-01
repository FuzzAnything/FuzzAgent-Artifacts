// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 */

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t* init_encoder(int width, int height) {
    aom_codec_ctx_t *ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to read integers from fuzz data safely */
static int get_int(const uint8_t *d, size_t s, int idx) {
    if (idx * 4 + 4 > s) return 0;
    return *(int*)(d + idx * 4);
}

static void run_control_tests(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return;

    /* 1. AV1E_SET_ENABLE_1TO4_PARTITIONS (int: 0 or 1) */
    int val1 = get_int(data, size, 0) % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val1);

    /* 2. AV1E_SET_ENABLE_ANGLE_DELTA (int: 0 or 1) */
    int val2 = get_int(data, size, 1) % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val2);

    /* 3. AV1E_SET_MIN_PARTITION_SIZE (int: 4, 8, 16, 32, 64, 128) */
    int val3 = get_int(data, size, 2);
    int sizes[] = {4, 8, 16, 32, 64, 128};
    int valid_size = sizes[val3 % 6];
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, valid_size);

    /* 4. AV1E_SET_QM_V (int: quantization matrix vertical index or value) */
    int val4 = get_int(data, size, 3);
    aom_codec_control(ctx, AV1E_SET_QM_V, val4);

    /* 5. AV1E_SET_ENABLE_SMOOTH_INTRA (int: 0 or 1) */
    int val5 = get_int(data, size, 4) % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val5);

    /* 6. AV1E_SET_REDUCED_TX_TYPE_SET (int: 0 or 1) */
    int val6 = get_int(data, size, 5) % 2;
    aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val6);
}

int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0; /* Need at least 6 ints */

    /* Extract dimensions from data to vary encoder state */
    int width = (Data[0] % 31) * 32 + 32; /* 32 to 1024 */
    int height = (Data[1] % 31) * 32 + 32; /* 32 to 1024 */

    /* Ensure reasonable limits for fuzzing stability */
    if (width > 2048) width = 2048;
    if (height > 2048) height = 2048;

    aom_codec_ctx_t *ctx = init_encoder(width, height);
    if (!ctx) return 0;

    /* Run the control function tests */
    run_control_tests(ctx, Data, Size);

    /* Cleanup */
    cleanup_codec(ctx);

    return 0;
}