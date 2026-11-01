// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int safe_control_int(aom_codec_ctx_t *c, int control_id, int val) {
    return aom_codec_control(c, control_id, val);
}

int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = NULL;
    // Use aom_codec_av1_cx() from aomcx.h
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use input data to configure encoder settings fuzzily
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Size > 1 ? (Data[1] % 1024) + 16 : 16);
    cfg.g_timebase.num = (Size > 2 ? (Data[2] % 100) + 1 : 1);
    cfg.g_timebase.den = (Size > 3 ? (Data[3] % 100) + 1 : 1);
    cfg.rc_target_bitrate = (Size > 4 ? (Data[4] % 10000) + 100 : 100);

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // 1. AV1E_SET_MIN_CR
    if (Size >= 1) {
        int val = Data[0];
        safe_control_int(ctx, AV1E_SET_MIN_CR, val);
    }

    // 2. AV1E_SET_ENABLE_DIST_8X8 (Not found in standard headers, skipping to prevent compile error)
    // The error message suggests AV1E_SET_ENABLE_RECT_TX exists, but the target is missing.
    // We skip this specific control to ensure compilation.

    // 3. AV1E_SET_ENABLE_SMOOTH_INTRA
    if (Size >= 2) {
        int val = Data[1] % 2;
        safe_control_int(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
    }

    // 4. AV1E_SET_CHROMA_SUBSAMPLING_X
    if (Size >= 3) {
        int val = Data[2] % 3; // 0, 1, or 2
        safe_control_int(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
    }

    // 5. AV1E_SET_MV_COST_UPD_FREQ
    if (Size >= 4) {
        int val = Data[3];
        safe_control_int(ctx, AV1E_SET_MV_COST_UPD_FREQ, val);
    }

    // 6. AV1E_SET_QM_V
    // This control typically requires a pointer to a quantization matrix structure.
    // Passing raw fuzz data as a pointer is unsafe. We skip this to avoid crashes.

    cleanup_codec(ctx);
    return 0;
}