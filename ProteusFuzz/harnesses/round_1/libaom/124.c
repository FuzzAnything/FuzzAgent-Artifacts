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
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (initialize_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    int control_value = 0;
    memcpy(&control_value, Data, sizeof(int));

    // Use the integer values for the control IDs as defined in aomcx.h
    // These correspond to the enum values for AV1 encoder controls
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, control_value);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, control_value);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, control_value);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, control_value);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, control_value);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, control_value);

    // Explore more states with different values if input is large enough
    if (Size >= 8) {
        int val2 = 0;
        memcpy(&val2, Data + 4, sizeof(int));
        aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, val2);
    }

    cleanup_encoder(&ctx);
    return 0;
}