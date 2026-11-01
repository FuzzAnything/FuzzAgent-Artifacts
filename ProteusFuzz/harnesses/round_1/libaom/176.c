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

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"

static aom_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
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

    // Fuzz some configuration parameters if data is available
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16; // Ensure width is at least 16
        cfg.g_h = (data[1 % size] % 256) + 16; // Ensure height is at least 16
        cfg.g_timebase.num = (data[2 % size] % 256) + 1;
        cfg.g_timebase.den = (data[3 % size] % 256) + 1;
        cfg.rc_target_bitrate = (data[4 % size] % 10000) + 100;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
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

static int fuzz_control(aom_codec_ctx_t *ctx, int control, int value) {
    return aom_codec_control(ctx, control, value);
}

int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    int val = (Data[0] % 2); // 0 or 1 for boolean-like controls

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    fuzz_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    int mode = (Data[1 % Size] % 3); // 0, 1, or 2
    fuzz_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, mode);

    // Fuzz AV1E_SET_S_FRAME_MODE
    int s_frame = (Data[2 % Size] % 2);
    fuzz_control(ctx, AV1E_SET_S_FRAME_MODE, s_frame);

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    fuzz_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    fuzz_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);

    cleanup_encoder_context(ctx);
    return 0;
}