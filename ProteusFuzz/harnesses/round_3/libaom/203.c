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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

// Helper to initialize an encoder context
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
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to vary configuration slightly
    cfg.g_w = (data[0] % 256) + 16;
    cfg.g_h = (data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 100) + 1;
    cfg.rc_target_bitrate = (data[3] % 1000) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to cleanup encoder context
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_203(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    int enable_overlay = Data[0] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    int enable_smooth_interintra = Data[1] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, enable_smooth_interintra);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    int enable_interinter_wedge = Data[2] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_interinter_wedge);

    // Fuzz AV1E_SET_ENABLE_PALETTE
    int enable_palette = Data[3] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);

    // Fuzz AV1E_SET_QM_MIN
    if (Size > 4) {
        int qm_min = Data[4] % 64;
        aom_codec_control(ctx, AV1E_SET_QM_MIN, qm_min);
    }

    // Fuzz AV1E_SET_RENDER_SIZE
    if (Size > 6) {
        int render_w = (Data[5] % 256) + 16;
        int render_h = (Data[6] % 256) + 16;
        aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, render_w, render_h);
    }

    // Cleanup
    cleanup_encoder_context(ctx);

    return 0;
}