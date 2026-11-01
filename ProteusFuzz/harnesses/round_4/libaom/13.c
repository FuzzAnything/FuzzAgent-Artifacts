// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

/* 
 * Fuzz driver for libaom encoder control functions.
 * Since the specific signatures for the typechecked control functions 
 * were not provided in the prompt, this driver uses the standard 
 * aom_codec_control API with the corresponding control IDs.
 * 
 * Target IDs based on function names:
 * - AV1E_SET_AQ_MODE
 * - AV1E_SET_MODE_COST_UPD_FREQ
 * - AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * - AOME_SET_MAX_INTRA_BITRATE_PCT
 * - AV1E_SET_DELTAQ_STRENGTH
 * - AV1E_SET_ENABLE_GLOBAL_MOTION
 */

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t res;
    /* 
     * The typechecked functions typically wrap aom_codec_control.
     * We invoke the generic control function with the specific ID.
     */
    res = aom_codec_control(ctx, control_id, value);
    return (res == AOM_CODEC_OK) ? 0 : 1;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int control_id;
    int value;
    int i;

    /* Initialize context */
    memset(&ctx, 0, sizeof(ctx));
    
    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    /* Initialize encoder config with defaults */
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Set some basic config to ensure valid initialization */
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    /* Initialize the codec context */
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    /* Extract control ID and value from input data */
    /* Use first 2 bytes for control ID selection (0-5) */
    control_id = (Data[0] % 6);
    
    /* Map index to actual control ID constants */
    int actual_id;
    switch (control_id) {
        case 0: actual_id = AV1E_SET_AQ_MODE; break;
        case 1: actual_id = AV1E_SET_MODE_COST_UPD_FREQ; break;
        case 2: actual_id = AV1E_SET_ENABLE_TX_SIZE_SEARCH; break;
        case 3: actual_id = AOME_SET_MAX_INTRA_BITRATE_PCT; break;
        case 4: actual_id = AV1E_SET_DELTAQ_STRENGTH; break;
        case 5: actual_id = AV1E_SET_ENABLE_GLOBAL_MOTION; break;
        default: actual_id = AV1E_SET_AQ_MODE; break;
    }

    /* Use next 2 bytes for value */
    value = (Data[1] << 8) | Data[2];
    
    /* 
     * Explore diverse states by invoking the control multiple times 
     * with slightly modified values derived from the input stream.
     */
    for (i = 0; i < Size; i += 4) {
        int val = (i + 2 < Size) ? ((Data[i] << 8) | Data[i+1]) : value;
        int id = (i + 4 < Size) ? (Data[i+2] % 6) : control_id;
        
        int target_id;
        switch (id) {
            case 0: target_id = AV1E_SET_AQ_MODE; break;
            case 1: target_id = AV1E_SET_MODE_COST_UPD_FREQ; break;
            case 2: target_id = AV1E_SET_ENABLE_TX_SIZE_SEARCH; break;
            case 3: target_id = AOME_SET_MAX_INTRA_BITRATE_PCT; break;
            case 4: target_id = AV1E_SET_DELTAQ_STRENGTH; break;
            case 5: target_id = AV1E_SET_ENABLE_GLOBAL_MOTION; break;
            default: target_id = actual_id; break;
        }

        /* Invoke the control function */
        (void)aom_codec_control(&ctx, target_id, val);
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}