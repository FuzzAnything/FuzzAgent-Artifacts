// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_external_partition.h"
#include "aom_integer.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - aom_codec_control_typechecked_AV1E_SET_TIER_MASK
 * - aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
 * - aom_codec_control_typechecked_AV1E_SET_DV_COST_UPD_FREQ
 * - aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_X
 * - aom_codec_control_typechecked_AV1E_SET_MV_COST_UPD_FREQ
 * - aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC
 *
 * Note: Since the exact signatures were not provided in the prompt, this driver
 * assumes the standard aom_codec_control_t enum values and integer arguments
 * typical for these AV1 encoder settings.
 */

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    /* Set basic configuration to ensure valid initialization */
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 100;

    res = aom_codec_enc_init(codec, iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_218(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int idx;
    int val;

    if (Size < 2) {
        return 0;
    }

    /* Initialize codec context */
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    /* Use input data to drive diverse control calls */
    /* Index to select which control to call */
    idx = Data[0] % 6;
    /* Value to pass to the control function */
    val = (Data[1] << 8) | (Size > 2 ? Data[2] : 0);

    /*
     * Map idx to specific control IDs.
     * These are standard AV1 encoder control IDs.
     * If the typechecked macros are just wrappers around aom_codec_control,
     * we call aom_codec_control directly with the appropriate enum.
     */
    switch (idx) {
        case 0:
            /* AV1E_SET_TIER_MASK */
            res = aom_codec_control(&codec, AV1E_SET_TIER_MASK, val);
            break;
        case 1:
            /* AV1E_SET_COEFF_COST_UPD_FREQ */
            res = aom_codec_control(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, val);
            break;
        case 2:
            /* AV1E_SET_DV_COST_UPD_FREQ */
            res = aom_codec_control(&codec, AV1E_SET_DV_COST_UPD_FREQ, val);
            break;
        case 3:
            /* AV1E_SET_CHROMA_SUBSAMPLING_X */
            /* Chroma subsampling usually expects specific values, but fuzzing tries various */
            res = aom_codec_control(&codec, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
            break;
        case 4:
            /* AV1E_SET_MV_COST_UPD_FREQ */
            res = aom_codec_control(&codec, AV1E_SET_MV_COST_UPD_FREQ, val);
            break;
        case 5:
            /* AV1E_SET_RTC_EXTERNAL_RC */
            res = aom_codec_control(&codec, AV1E_SET_RTC_EXTERNAL_RC, val);
            break;
        default:
            break;
    }

    /*
     * We do not strictly check 'res' for success because invalid inputs
     * are expected during fuzzing. The goal is to trigger internal logic
     * without crashing.
     */

    cleanup_encoder(&codec);

    return 0;
}