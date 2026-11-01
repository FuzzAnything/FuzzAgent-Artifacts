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

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;

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

int LLVMFuzzerTestOneInput_205(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    int val;
    if (Size >= sizeof(int)) {
        memcpy(&val, Data, sizeof(int));
    } else {
        val = (int)Data[0];
    }

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
    
    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);

    // Fuzz AV1E_SET_ENABLE_MASKED_COMP
    aom_codec_control(ctx, AV1E_SET_ENABLE_MASKED_COMP, val);

    // Fuzz AV1E_SET_TUNE_CONTENT
    aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, val);

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val);

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);

    cleanup_encoder_context(ctx);
    return 0;
}