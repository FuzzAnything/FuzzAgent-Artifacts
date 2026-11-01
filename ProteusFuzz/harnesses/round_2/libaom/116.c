// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize default encoder configuration
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration from fuzz data
    cfg.g_w = (Size > 0) ? Data[0] + 16 : 16; // Ensure minimum width
    cfg.g_h = (Size > 1) ? Data[1] + 16 : 16; // Ensure minimum height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;

    // Initialize the codec context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fuzz various control functions
    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTRA
    if (Size > 2) {
        int val = Data[2];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE
    if (Size > 3) {
        int val = Data[3];
        aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, val);
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT
    if (Size > 4) {
        int val = Data[4];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, val);
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (Size > 5) {
        int val = Data[5];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
    if (Size > 6) {
        int val = Data[6];
        aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (Size > 7) {
        int val = Data[7];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}