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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = size > 0 ? (size % 64) + 16 : 16;
    cfg.g_h = size > 0 ? (size % 64) + 16 : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    int val = 0;
    if (Size > 0) val = Data[0] % 2;
    if (Size > 1) val = Data[1] % 2;

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val);

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    int mode = 0;
    if (Size > 2) mode = Data[2] % 3;
    aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, mode);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    int smooth = 0;
    if (Size > 3) smooth = Data[3] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, smooth);

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    int dual = 0;
    if (Size > 4) dual = Data[4] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, dual);

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Assuming this takes a pointer to some data, we use the input data directly if available
    if (Size > 5) {
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void *)(intptr_t)Data);
    }

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    int partitions = 0;
    if (Size > 6) partitions = Data[6] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, partitions);

    cleanup_encoder_context(ctx);
    return 0;
}