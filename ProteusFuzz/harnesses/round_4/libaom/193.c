// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    if (!initialize_encoder(&ctx, &cfg)) {
        return 0;
    }

    int seed = Data[0];
    int enable_val = (seed & 0xFF) ? 1 : 0;
    int svc_val = (seed >> 8) & 0xFF;
    int superres_val = (seed >> 16) & 0xFF;

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (seed & 1) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_val);
    }

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    if (seed & 2) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable_val);
    }

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    if (seed & 4) {
        int level_idx = -1;
        aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);
        (void)level_idx;
    }

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    if (seed & 8) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_val);
    }

    // Fuzz AV1E_SET_SVC_REF_FRAME_CONFIG
    if (seed & 16) {
        aom_svc_ref_frame_config_t ref_cfg = {0};
        for (int i = 0; i < 7; i++) {
            ref_cfg.reference[i] = (svc_val & (1 << i)) ? 1 : 0;
            ref_cfg.ref_idx[i] = svc_val % 8;
        }
        for (int i = 0; i < 8; i++) {
            ref_cfg.refresh[i] = (svc_val & (1 << (i + 7))) ? 1 : 0;
        }
        aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);
    }

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    if (seed & 32) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, superres_val ? 1 : 0);
    }

    cleanup_codec(&ctx);
    return 0;
}