// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/* Helper to initialize an encoder context for fuzzing */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    /* Set minimal configuration to ensure valid state */
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 100;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

/* Helper to cleanup encoder context */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int val;
    int idx;

    if (Size < 1) return 0;

    /* Initialize encoder context */
    if (init_encoder_context(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Use input data to select which control to fuzz and what value to pass */
    idx = Data[0] % 6;

    /* Extract a value from the input buffer, ensuring it's within reasonable bounds */
    if (Size >= 4) {
        val = *(int32_t *)(Data + 1);
    } else {
        val = (int)Data[0];
    }

    /* Fuzz specific control functions based on index */
    switch (idx) {
        case 0:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH */
            /* Typically expects an int (0 or 1) */
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val ? 1 : 0);
            break;
        case 1:
            /* aom_codec_control_typechecked_AOME_SET_ARNR_STRENGTH */
            /* Typically expects an int strength value */
            ret = aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val & 0xFF);
            break;
        case 2:
            /* aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH */
            /* Typically expects an int strength value */
            ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val & 0xFF);
            break;
        case 3:
            /* aom_codec_control_typechecked_AV1E_SET_AQ_MODE */
            /* Typically expects an int mode (0-4) */
            ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, val % 5);
            break;
        case 4:
            /* aom_codec_control_typechecked_AOME_SET_ARNR_MAXFRAMES */
            /* Typically expects an int max frames */
            ret = aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val & 0xFF);
            break;
        case 5:
            /* aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ */
            /* Typically expects an int frequency */
            ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val & 0xFF);
            break;
        default:
            ret = AOM_CODEC_OK;
            break;
    }

    /* Cleanup */
    cleanup_encoder_context(&ctx);

    return 0;
}