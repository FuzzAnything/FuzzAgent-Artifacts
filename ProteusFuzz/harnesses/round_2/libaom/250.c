// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/* Helper function to initialize encoder context */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int w, h;

    /* Parse width and height from input if available, otherwise default */
    if (size >= 4) {
        w = (data[0] % 64) + 16;
        h = (data[1] % 64) + 16;
    } else {
        w = 32;
        h = 32;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper function to cleanup encoder context */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_250(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx;
    aom_svc_params_t svc_params;
    int val;
    int ret;

    if (Size < 2) return 0;

    ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    /* 1. AV1E_SET_S_FRAME_MODE */
    val = Data[0] % 2;
    ret = aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, val);
    (void)ret; /* Ignore return for fuzzing stability */

    /* 2. AV1E_SET_NOISE_SENSITIVITY */
    val = Data[1] % 16;
    ret = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
    (void)ret;

    /* 3. AV1E_SET_SVC_PARAMS */
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = (Data[0] % 3) + 1;
    svc_params.number_temporal_layers = (Data[1] % 3) + 1;
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = Data[i % Size] % 64 + 1;
        svc_params.min_quantizers[i] = Data[(i + 1) % Size] % 64 + 1;
        svc_params.layer_target_bitrate[i] = (Data[(i + 2) % Size] % 100) + 10;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        svc_params.scaling_factor_num[i] = (Data[(i + 3) % Size] % 10) + 1;
        svc_params.scaling_factor_den[i] = (Data[(i + 4) % Size] % 10) + 1;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        svc_params.framerate_factor[i] = (Data[(i + 5) % Size] % 10) + 1;
    }
    ret = aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    (void)ret;

    /* 4. AV1E_SET_ENABLE_FLIP_IDTX */
    val = Data[0] % 2;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);
    (void)ret;

    /* 5. AV1E_SET_REDUCED_TX_TYPE_SET */
    val = Data[1] % 2;
    ret = aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    (void)ret;

    /* 6. AV1E_SET_ENABLE_PAETH_INTRA */
    val = Data[0] % 2;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    (void)ret;

    cleanup_encoder(ctx);
    return 0;
}