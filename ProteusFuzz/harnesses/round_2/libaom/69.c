// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static aom_codec_ctx_t *init_encoder_context(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    ctx->iface = iface;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    aom_codec_ctx_t *ctx = init_encoder_context(iface, &cfg);
    if (!ctx) return 0;

    // Helper macro to safely read data
    #define SAFE_READ(index, var) \
        if ((index) < Size) { var = Data[(index)]; } else { var = 0; }

    // Fuzz AV1E_SET_QM_MAX
    int qm_max;
    SAFE_READ(0, qm_max);
    aom_codec_control(ctx, AV1E_SET_QM_MAX, &qm_max);

    // Fuzz AV1E_SET_MIN_CR
    int min_cr;
    SAFE_READ(1, min_cr);
    aom_codec_control(ctx, AV1E_SET_MIN_CR, &min_cr);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode;
    SAFE_READ(2, aq_mode);
    aq_mode = aq_mode % 5; // Assuming valid range 0-4
    aom_codec_control(ctx, AV1E_SET_AQ_MODE, &aq_mode);

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    int deltaq_strength;
    SAFE_READ(3, deltaq_strength);
    aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, &deltaq_strength);

    // Fuzz AV1E_SET_ENABLE_QM
    int enable_qm;
    SAFE_READ(4, enable_qm);
    enable_qm = enable_qm % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_QM, &enable_qm);

    // Fuzz AV1E_SET_QM_V
    int qm_v[16];
    size_t qm_v_count = 0;
    for (size_t i = 5; i < Size && qm_v_count < 16; i++) {
        qm_v[qm_v_count++] = Data[i];
    }
    // Ensure array is fully populated if less data is available
    for (size_t i = qm_v_count; i < 16; i++) {
        qm_v[i] = 0;
    }
    aom_codec_control(ctx, AV1E_SET_QM_V, qm_v);

    cleanup_encoder_context(ctx);
    return 0;
}