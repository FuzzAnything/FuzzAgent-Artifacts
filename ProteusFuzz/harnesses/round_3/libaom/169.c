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
#include <stdbool.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"

/* Helper to initialize encoder context */
static bool init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return false;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return false;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return false;
    }
    return true;
}

/* Helper to clean up encoder context */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_169(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    uint32_t param_val = 0;
    memcpy(&param_val, Data, sizeof(param_val));

    int enable_flag = (param_val & 0x1) ? 1 : 0;
    int layer_id = (param_val >> 1) & 0xFF;

    /* Test AV1E_SET_ENABLE_RECT_TX */
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_flag);

    /* Test AV1E_SET_SVC_LAYER_ID */
    aom_svc_layer_id_t svc_id;
    svc_id.spatial_layer_id = (layer_id >> 0) & 0xF;
    svc_id.temporal_layer_id = (layer_id >> 4) & 0xF;
    aom_codec_control(&ctx, AV1E_SET_SVC_LAYER_ID, &svc_id);

    /* Test AV1E_SET_ENABLE_INTERINTRA_COMP */
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, enable_flag);

    /* Test AV1E_SET_ENABLE_DUAL_FILTER */
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, enable_flag);

    /* Test AV1E_SET_ENABLE_INTERINTRA_WEDGE */
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_flag);

    /* Test AV1E_SET_ENABLE_FILTER_INTRA */
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable_flag);

    cleanup_encoder(&ctx);
    return 0;
}