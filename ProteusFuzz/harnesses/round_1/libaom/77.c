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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to set some config parameters if size is sufficient
    if (size >= 2) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Extract values from input to use as parameters for codec control
    int32_t val1 = 0;
    int32_t val2 = 0;
    int32_t val3 = 0;
    int32_t val4 = 0;
    int32_t val5 = 0;
    int32_t val6 = 0;

    if (Size >= 1) val1 = (int32_t)(Data[0] % 5) + 1;
    if (Size >= 2) val2 = (int32_t)(Data[1] % 100) + 1;
    if (Size >= 3) val3 = (int32_t)(Data[2] % 100) + 1;
    if (Size >= 4) val4 = (int32_t)(Data[3] % 2);
    if (Size >= 5) val5 = (int32_t)(Data[4] % 100) + 1;
    if (Size >= 6) val6 = (int32_t)(Data[5] % 5) + 1;

    // Try to invoke the target API functions with fuzzed values
    // Note: These are type-checked control functions; exact behavior depends on libaom implementation
    aom_codec_control(ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val1);
    aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, val2);
    aom_codec_control(ctx, AV1E_SET_MAX_GF_INTERVAL, val3);
    aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val4);
    aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val5);
    aom_codec_control(ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6);

    cleanup_encoder(ctx);
    return 0;
}