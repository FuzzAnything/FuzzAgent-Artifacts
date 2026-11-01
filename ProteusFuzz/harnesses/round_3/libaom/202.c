// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

/*
 * Fuzz driver for libaom encoder control functions.
 * Target APIs:
 * - AV1E_SET_ENABLE_GLOBAL_MOTION
 * - AV1E_SET_MODE_COST_UPD_FREQ
 * - AV1E_SET_AQ_MODE
 * - AOME_SET_ARNR_MAXFRAMES
 * - AOME_SET_ARNR_STRENGTH
 * - AV1E_SET_DELTAQ_STRENGTH
 *
 * Note: The provided signatures were empty, but these are standard AV1 encoder
 * control codes available in aom/aom_encoder.h. This driver assumes the standard
 * libaom API where aom_codec_control takes an aom_codec_ctx_t pointer and
 * a control code followed by an argument.
 */

static int initialize_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(codec);
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static int set_control_int(aom_codec_ctx_t *codec, int control_code, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(codec, control_code, value);
    return (ret == AOM_CODEC_OK) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int val1, val2, val3, val4, val5, val6;
    
    if (Size < 24) return 0;

    /* Extract values from input data */
    val1 = *(const int32_t *)(Data);
    val2 = *(const int32_t *)(Data + 4);
    val3 = *(const int32_t *)(Data + 8);
    val4 = *(const int32_t *)(Data + 12);
    val5 = *(const int32_t *)(Data + 16);
    val6 = *(const int32_t *)(Data + 20);

    /* Initialize codec */
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (initialize_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    /*
     * Invoke target control functions with fuzzed values.
     * We clamp or mask values where necessary to stay within reasonable bounds
     * to avoid immediate crashes from invalid ranges, though libaom should handle
     * invalid inputs gracefully.
     */

    /* 1. AV1E_SET_ENABLE_GLOBAL_MOTION (expecting 0 or 1) */
    set_control_int(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, val1 & 1);

    /* 2. AV1E_SET_MODE_COST_UPD_FREQ (expecting uint32_t, typically > 0) */
    uint32_t freq = (uint32_t)val2;
    if (freq == 0) freq = 1;
    set_control_int(&codec, AV1E_SET_MODE_COST_UPD_FREQ, (int)freq);

    /* 3. AV1E_SET_AQ_MODE (expecting enum aom_aq_mode_t, typically 0-5) */
    int aq_mode = val3 % 6;
    if (aq_mode < 0) aq_mode = 0;
    set_control_int(&codec, AV1E_SET_AQ_MODE, aq_mode);

    /* 4. AOME_SET_ARNR_MAXFRAMES (expecting int, typically > 0) */
    int maxframes = val4;
    if (maxframes <= 0) maxframes = 1;
    if (maxframes > 1000) maxframes = 1000;
    set_control_int(&codec, AOME_SET_ARNR_MAXFRAMES, maxframes);

    /* 5. AOME_SET_ARNR_STRENGTH (expecting int, typically 0-6) */
    int strength = val5 % 7;
    if (strength < 0) strength = 0;
    set_control_int(&codec, AOME_SET_ARNR_STRENGTH, strength);

    /* 6. AV1E_SET_DELTAQ_STRENGTH (expecting int, typically 0-63) */
    int deltaq = val6 % 64;
    if (deltaq < 0) deltaq = 0;
    set_control_int(&codec, AV1E_SET_DELTAQ_STRENGTH, deltaq);

    /* Cleanup */
    cleanup_encoder(&codec);

    return 0;
}