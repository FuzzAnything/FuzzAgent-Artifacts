// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

/* Helper functions defined outside LLVMFuzzerTestOneInput to avoid C syntax errors */
static inline aom_codec_err_t call_control_int(aom_codec_ctx_t *ctx, int ctrl, int val) {
    return aom_codec_control(ctx, ctrl, val);
}

static inline aom_codec_err_t call_control_ptr(aom_codec_ctx_t *ctx, int ctrl, void *val) {
    return aom_codec_control(ctx, ctrl, val);
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_338(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface = NULL;

    /* 1. Prepare the environment */
    /* Initialize encoder configuration */
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Use fuzz data to set some config parameters to explore states */
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (Data[2] % 10000);

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    /* Allocate context */
    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    /* Initialize the codec context */
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    /* 2. Invoke target functions diversely */
    
    /* Attempt 1: AV1E_SET_INTER_DCT_ONLY */
    #ifdef AV1E_SET_INTER_DCT_ONLY
    {
        int val = (Data[3] % 2); // 0 or 1
        call_control_int(ctx, AV1E_SET_INTER_DCT_ONLY, val);
    }
    #endif

    /* Attempt 2: AV1E_SET_REDUCED_TX_TYPE_SET */
    #ifdef AV1E_SET_REDUCED_TX_TYPE_SET
    {
        int val = (Data[4] % 2);
        call_control_int(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    }
    #endif

    /* Attempt 3: AV1E_SET_ENABLE_INTERINTER_WEDGE */
    #ifdef AV1E_SET_ENABLE_INTERINTER_WEDGE
    {
        int val = (Data[5] % 2);
        call_control_int(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
    }
    #endif

    /* Attempt 4: AV1E_SET_MIN_PARTITION_SIZE */
    #ifdef AV1E_SET_MIN_PARTITION_SIZE
    {
        int val = (Data[6] % 128) + 4; // Min partition size usually >= 4
        call_control_int(ctx, AV1E_SET_MIN_PARTITION_SIZE, val);
    }
    #endif

    /* Attempt 5: AV1E_GET_SEQ_LEVEL_IDX */
    #ifdef AV1E_GET_SEQ_LEVEL_IDX
    {
        int dummy;
        call_control_int(ctx, AV1E_GET_SEQ_LEVEL_IDX, &dummy);
    }
    #endif

    /* Attempt 6: AV1E_SET_ALLOW_REF_FRAME_MVS */
    #ifdef AV1E_SET_ALLOW_REF_FRAME_MVS
    {
        int val = (Data[7] % 2);
        call_control_int(ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, val);
    }
    #endif

    /* Additional exploration: Try random control codes if specific ones are missing or to trigger error paths */
    if (Size > 8) {
        int random_ctrl = Data[8] | (Data[9] << 8);
        int random_val = Data[10] | (Data[11] << 8);
        /* This might trigger unknown command handling */
        call_control_int(ctx, random_ctrl, random_val);
    }

    /* 3. Cleanup */
    cleanup_codec(ctx);

    return 0;
}