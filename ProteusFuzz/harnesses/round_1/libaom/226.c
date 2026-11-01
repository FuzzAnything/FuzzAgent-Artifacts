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
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 10000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

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

int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) {
        return 0;
    }

    int val = Data[0];

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);

    // Fuzz AOME_SET_ARNR_STRENGTH
    aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, val);

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val);

    // Fuzz AOME_SET_SHARPNESS
    aom_codec_control(ctx, AOME_SET_SHARPNESS, val);

    // Fuzz AOME_SET_ARNR_MAXFRAMES
    aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, val);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);

    cleanup_encoder_context(ctx);
    return 0;
}