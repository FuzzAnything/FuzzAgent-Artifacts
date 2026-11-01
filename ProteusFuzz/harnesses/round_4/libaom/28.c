// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/*
 * Note: The specific type-checked control functions mentioned in the prompt
 * (e.g., aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE) are not standard
 * public API symbols in libaom. They appear to be internal or generated macros.
 * In a standard fuzzing scenario, we target the underlying generic control
 * function `aom_codec_control` with the specific control IDs (AV1E_SET_...).
 *
 * This driver initializes an AV1 encoder context and attempts to invoke
 * the relevant control commands using the standard aom_codec_control API.
 * It uses the input data to seed configuration parameters and randomize
 * the values passed to the control functions.
 */

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int val;

    /* Initialize codec context */
    memset(&codec, 0, sizeof(codec));

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    /* Initialize encoder configuration with defaults */
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    /* Use input data to seed configuration parameters */
    if (Size >= 4) {
        uint32_t seed = *(const uint32_t *)Data;
        cfg.g_w = (seed % 1920) + 16;
        cfg.g_h = ((seed >> 8) % 1080) + 16;
        cfg.g_timebase.num = (seed & 0xFF) + 1;
        cfg.g_timebase.den = ((seed >> 8) & 0xFF) + 1;
        cfg.g_threads = ((seed >> 16) % 16) + 1;
        cfg.rc_target_bitrate = ((seed >> 24) % 10000) + 100;
    }

    /* Initialize the encoder */
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    /* Extract values from input to use as arguments for control functions */
    int arg_val = 0;
    if (Size > 0) {
        arg_val = Data[0];
    }
    if (Size > 1) {
        arg_val |= (Data[1] << 8);
    }

    /*
     * Invoke the target control functions.
     * Since the specific type-checked wrappers are not standard public APIs,
     * we use aom_codec_control with the corresponding AV1E_ constants.
     */

    /* 1. AV1E_SET_ENABLE_PALETTE */
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_PALETTE, arg_val);
    (void)ret; /* Ignore return for fuzzing stability, crash on invalid state is desired */

    /* 2. AV1E_SET_MIN_GF_INTERVAL */
    ret = aom_codec_control(&codec, AV1E_SET_MIN_GF_INTERVAL, arg_val);
    (void)ret;

    /* 3. AV1E_GET_BASELINE_GF_INTERVAL */
    /* GET operations typically return via a pointer or the control function itself */
    ret = aom_codec_control(&codec, AV1E_GET_BASELINE_GF_INTERVAL, &arg_val);
    (void)ret;

    /* 4. AV1E_SET_INTRA_DEFAULT_TX_ONLY */
    ret = aom_codec_control(&codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, arg_val);
    (void)ret;

    /* 5. AV1E_SET_ENABLE_DIFF_WTD_COMP */
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIFF_WTD_COMP, arg_val);
    (void)ret;

    /* 6. AV1E_SET_ENABLE_GLOBAL_MOTION */
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, arg_val);
    (void)ret;

    /* Cleanup */
    cleanup_codec(&codec);

    return 0;
}