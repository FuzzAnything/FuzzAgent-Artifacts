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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/* Helper function to initialize the encoder context */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;

    if (size == 0) {
        return NULL;
    }

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Initialize encoder configuration */
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzzing data to tweak configuration slightly if possible, 
       but keep it valid to ensure initialization succeeds */
    cfg.g_w = (data[0] % 256) + 16;
    cfg.g_h = (data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (data[3] % 10000) + 100;

    /* Initialize the codec context */
    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data for the basic checks */
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) {
        return 0;
    }

    /* 1. Fuzz AV1E_SET_ENABLE_INTRABC */
    /* Typically takes an int (0 or 1) */
    int enable_intrabc = (Data[0] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    /* 2. Fuzz AV1E_SET_RATE_DISTRIBUTION_INFO */
    /* This command typically expects a pointer to a struct or specific data, 
       not a raw integer. Passing an integer directly caused the crash.
       Since the exact struct isn't provided in the prompt, we skip this specific
       control command to avoid undefined behavior, or we would need to construct
       a valid dummy struct if the API signature was known to accept a pointer.
       Given the crash on a string comparison inside the library, it likely expects
       a pointer to a struct containing strings or specific configuration.
       We will omit this specific call to prevent the crash as we cannot safely
       construct the expected argument without the struct definition. */
    // aom_codec_control(ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, rate_dist_val);

    /* 3. Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX */
    /* Takes an int index */
    int level_idx = (Data[2] % 10); /* Valid range usually 0-9 or similar */
    aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);

    /* 4. Fuzz AV1E_SET_ENABLE_FLIP_IDTX */
    /* Takes an int (0 or 1) */
    int enable_flip_idtx = (Data[3] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);

    /* 5. Fuzz AV1E_SET_ENABLE_AB_PARTITIONS */
    /* Takes an int (0 or 1) */
    /* Check size before accessing Data[4] */
    if (Size > 4) {
        int enable_ab_partitions = (Data[4] % 2);
        aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab_partitions);
    }

    /* 6. Fuzz AV1E_SET_SVC_PARAMS */
    /* Takes a pointer to aom_svc_params_t */
    if (Size >= 10) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));

        /* Fill with fuzzing data, ensuring some bounds */
        svc_params.number_spatial_layers = (Data[5] % 3) + 1;
        svc_params.number_temporal_layers = (Data[6] % 3) + 1;

        for (int i = 0; i < AOM_MAX_LAYERS && i < 10; i++) {
            /* Ensure index is within bounds of Data */
            if (7 + i < Size) svc_params.max_quantizers[i] = (Data[7 + i] % 64) + 1;
            if (8 + i < Size) svc_params.min_quantizers[i] = (Data[8 + i] % 64) + 1;
            if (9 + i < Size) svc_params.layer_target_bitrate[i] = (Data[9 + i] % 1000) + 100;
        }

        for (int i = 0; i < AOM_MAX_SS_LAYERS && i < 4; i++) {
            if (10 + i < Size) svc_params.scaling_factor_num[i] = (Data[10 + i] % 10) + 1;
            if (11 + i < Size) svc_params.scaling_factor_den[i] = (Data[11 + i] % 10) + 1;
        }

        for (int i = 0; i < AOM_MAX_TS_LAYERS && i < 4; i++) {
            if (12 + i < Size) svc_params.framerate_factor[i] = (Data[12 + i] % 10) + 1;
        }

        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    cleanup_encoder(ctx);
    return 0;
}