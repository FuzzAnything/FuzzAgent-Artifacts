// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_TARGET_SEQ_LEVEL_IDX
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_MAX_REFERENCE_FRAMES
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_ENABLE_INTERINTRA_WEDGE
 *
 * Note: The functions listed in the prompt as "aom_codec_control_typechecked_..."
 * are not standard public API names. The standard way to invoke these controls
 * is via aom_codec_control() with the corresponding AV1E_* control IDs.
 * This driver uses aom_codec_control() to exercise these features.
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int val_int;
    int val_bool;
    int val_enum;

    if (Size < 3) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Extract values from input data to diversify control parameters */
    val_int = (int)(Data[0] & 0xFF);
    val_bool = (Data[1] & 0xFF) ? 1 : 0;
    val_enum = (int)(Data[2] & 0xFF);

    /*
     * 1. AV1E_SET_TARGET_SEQ_LEVEL_IDX
     * Sets the target sequence level index.
     * Valid range typically 0-63 for AV1.
     */
    ret = aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val_int);
    (void)ret; /* Ignore return for fuzzing, just exercise the call */

    /*
     * 2. AV1E_SET_ENABLE_REF_FRAME_MVS
     * Enables/Disables reference frame motion vector search.
     * Expects a boolean (0 or 1).
     */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val_bool);
    (void)ret;

    /*
     * 3. AV1E_SET_MAX_REFERENCE_FRAMES
     * Sets the maximum number of reference frames.
     * Valid range typically 0-7.
     */
    int max_ref = val_int % 8; /* Clamp to 0-7 */
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref);
    (void)ret;

    /*
     * 4. AV1E_SET_RTC_EXTERNAL_RC
     * Sets the external rate control mode.
     * Expects an integer (0 or 1 usually).
     */
    ret = aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val_bool);
    (void)ret;

    /*
     * 5. AV1E_SET_S_FRAME_MODE
     * Sets the S-frame mode.
     * Expects an enum value (0, 1, 2, etc.).
     */
    int s_frame_mode = val_enum % 3; /* Assume 0-2 valid for fuzzing */
    ret = aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);
    (void)ret;

    /*
     * 6. AV1E_SET_ENABLE_INTERINTRA_WEDGE
     * Enables/Disables interintra wedge partitioning.
     * Expects a boolean (0 or 1).
     */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val_bool);
    (void)ret;

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}