// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    int val;
    int ret;

    // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (size >= 1) {
        val = data[0] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
        if (ret != AOM_CODEC_OK) {
            // Handle error or continue fuzzing
        }
    }

    // AV1E_SET_ENABLE_INTERINTRA_WEDGE
    if (size >= 2) {
        val = data[1] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
        if (ret != AOM_CODEC_OK) {
            // Handle error or continue fuzzing
        }
    }

    // AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (size >= 3) {
        val = data[2] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);
        if (ret != AOM_CODEC_OK) {
            // Handle error or continue fuzzing
        }
    }

    // AV1E_GET_SEQ_LEVEL_IDX
    if (size >= 4) {
        val = data[3]; // Unused for GET, but consume input
        ret = aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &val);
        if (ret != AOM_CODEC_OK) {
            // Handle error or continue fuzzing
        }
    }

    // AV1E_SET_ENABLE_PAETH_INTRA
    if (size >= 5) {
        val = data[4] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
        if (ret != AOM_CODEC_OK) {
            // Handle error or continue fuzzing
        }
    }

    // AV1E_SET_ENABLE_DIAGONAL_INTRA
    if (size >= 6) {
        val = data[5] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
        if (ret != AOM_CODEC_OK) {
            // Handle error or continue fuzzing
        }
    }

    return 0;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Initialize encoder config
    res = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config values
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_end_usage = AOM_CQ; // Fixed: AOM_CQP -> AOM_CQ
    cfg.g_lag_in_frames = 0;
    cfg.g_usage = AOM_USAGE_REALTIME;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}