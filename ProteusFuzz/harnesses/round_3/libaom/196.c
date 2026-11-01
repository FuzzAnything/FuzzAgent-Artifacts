// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return NULL;

    /* Use fuzz data to configure encoder settings randomly with bounds checking */
    if (size > 1) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
    }
    if (size > 3) {
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
    }
    if (size > 5) {
        cfg.rc_target_bitrate = ((uint32_t)data[4] * 256 + data[5]) + 1000;
    }
    if (size > 6) {
        cfg.g_threads = (data[6] % 8) + 1;
    }
    if (size > 7) {
        cfg.g_error_resilient = (data[7] % 3);
    }
    if (size > 8) {
        cfg.g_lag_in_frames = (data[8] % 10);
    }
    if (size > 9) {
        cfg.g_pass = AOM_RC_ONE_PASS;
        cfg.g_usage = AOM_USAGE_REALTIME;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

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

static void fuzz_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return;
    aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_196(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    /* 1. AV1E_SET_ENABLE_RECT_TX */
    if (Size > 10) {
        int val = Data[10] % 2;
        fuzz_control(ctx, AV1E_SET_ENABLE_RECT_TX, val);
    }

    /* 2. AV1_SET_INSPECTION_CALLBACK */
    if (Size > 11) {
        fuzz_control(ctx, AV1_SET_INSPECTION_CALLBACK, (int)(intptr_t)(Data[11] ? Data : NULL));
    }

    /* 3. AV1E_SET_ENABLE_DUAL_FILTER */
    if (Size > 12) {
        int val = Data[12] % 2;
        fuzz_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val);
    }

    /* 4. AV1E_SET_ENABLE_INTERINTRA_WEDGE */
    if (Size > 13) {
        int val = Data[13] % 2;
        fuzz_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
    }

    /* 5. AV1E_SET_TUNE_CONTENT */
    if (Size > 14) {
        int val = Data[14] % 4; 
        fuzz_control(ctx, AV1E_SET_TUNE_CONTENT, val);
    }

    /* 6. AV1E_SET_ENABLE_FILTER_INTRA */
    if (Size > 15) {
        int val = Data[15] % 2;
        fuzz_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);
    }

    /* Additional fuzzing: Try random control IDs if available or just re-trigger with different values */
    if (Size > 17) {
        int val1 = Data[16] % 2;
        int val2 = Data[17] % 2;
        fuzz_control(ctx, AV1E_SET_ENABLE_RECT_TX, val1);
        fuzz_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val2);
    }

    cleanup_encoder(ctx);
    return 0;
}