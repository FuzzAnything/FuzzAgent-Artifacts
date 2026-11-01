// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static void init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }

    aom_codec_enc_config_default(iface, cfg, 0);
    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data for the parameters we want to use */
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    init_encoder(&ctx, &cfg);

    if (ctx.err != AOM_CODEC_OK) {
        return 0;
    }

    int param1 = (int)(Data[0]);
    int param2 = (int)(Data[1]);
    int param3 = (int)(Data[2] % 100);
    int param4 = (int)(Data[3] % 100);

    /*
     * The target functions listed in the prompt (e.g., aom_codec_control_typechecked_AV1E_SET_QM_Y)
     * appear to be internal or specific wrapper names not present in standard public headers
     * or the provided context. In standard libaom usage, these correspond to control IDs
     * passed to aom_codec_control().
     *
     * Since the exact function signatures for the "typechecked" variants were not provided
     * and likely do not exist as standalone C functions in the public API (they are usually
     * macros or internal helpers), we use the standard aom_codec_control API with the
     * corresponding AV1E_ control IDs.
     */

    /* 1. AV1E_SET_QM_Y */
    #ifdef AV1E_SET_QM_Y
    aom_codec_control(&ctx, AV1E_SET_QM_Y, param1);
    #endif

    /* 2. AV1E_SET_AQ_MODE */
    #ifdef AV1E_SET_AQ_MODE
    aom_codec_control(&ctx, AV1E_SET_AQ_MODE, param2);
    #endif

    /* 3. AV1E_SET_MIN_GF_INTERVAL */
    #ifdef AV1E_SET_MIN_GF_INTERVAL
    aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, param3);
    #endif

    /* 4. AOME_SET_CQ_LEVEL */
    #ifdef AOME_SET_CQ_LEVEL
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, param4);
    #endif

    /* 5. AV1E_SET_QM_MIN */
    #ifdef AV1E_SET_QM_MIN
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, param1);
    #endif

    /* 6. AV1E_SET_CDF_UPDATE_MODE */
    #ifdef AV1E_SET_CDF_UPDATE_MODE
    aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, param2);
    #endif

    aom_codec_destroy(&ctx);

    return 0;
}