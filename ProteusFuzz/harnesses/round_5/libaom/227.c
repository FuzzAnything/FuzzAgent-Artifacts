// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required Header Files */
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/*
 * Fallback macro definition if AOM_CODEC_CTRL_ID is not defined in the headers.
 * This prevents the "call to undeclared function" error.
 */
#ifndef AOM_CODEC_CTRL_ID
#define AOM_CODEC_CTRL_ID(x) 0
#endif

/* Forward declaration to avoid implicit declaration error if header is missing */
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_227(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    /* Initialize context to zero */
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Get the AV1 encoder interface */
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }

    /* Setup encoder configuration */
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    /* Use fuzz data to set some config parameters safely */
    if (Size >= 2) {
        cfg.g_w = (Data[0] % 512) + 16;
        cfg.g_h = (Data[1] % 512) + 16;
    }
    cfg.g_threads = 0; /* Use default threads */
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    /* Initialize the encoder */
    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    /* 3. Explore program states by invoking target controls */
    int val = (Size > 0) ? Data[0] : 0;
    
    /* AV1E_SET_FP_MT_UNIT_TEST */
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID("AV1E_SET_FP_MT_UNIT_TEST"), val);

    /* AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE */
    val = (Size > 1) ? Data[1] : 0;
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID("AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE"), val);

    /* AV1E_SET_SVC_FRAME_DROP_MODE */
    val = (Size > 2) ? Data[2] : 0;
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID("AV1E_SET_SVC_FRAME_DROP_MODE"), val);

    /* AV1E_SET_GF_MIN_PYRAMID_HEIGHT */
    val = (Size > 3) ? Data[3] : 0;
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID("AV1E_SET_GF_MIN_PYRAMID_HEIGHT"), val);

    /* AV1E_SET_MAX_PARTITION_SIZE */
    val = (Size > 4) ? Data[4] : 0;
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID("AV1E_SET_MAX_PARTITION_SIZE"), val);

    /* AV1E_SET_MIN_GF_INTERVAL */
    val = (Size > 5) ? Data[5] : 0;
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID("AV1E_SET_MIN_GF_INTERVAL"), val);

    /* 2. Cleanup */
    cleanup_codec(ctx);

    return 0;
}