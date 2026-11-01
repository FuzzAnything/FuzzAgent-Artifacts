// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"

/*
 * Helper function to safely initialize an AV1 encoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_encoder_context(int width, int height) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to cleanup encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_191(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Extract parameters from input data */
    uint32_t width = Data[0];
    uint32_t height = Data[1];
    uint32_t value = ((uint32_t)Data[2] << 8) | Data[3];

    /* Sanity checks for dimensions */
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }

    aom_codec_ctx_t *ctx = init_encoder_context(width, height);
    if (!ctx) {
        return 0;
    }

    /*
     * The provided function names in the prompt (e.g., aom_codec_control_typechecked_...)
     * are not standard public API functions in libaom. They appear to be internal
     * wrappers or hypothetical names. The standard way to set these options is via
     * aom_codec_control() with the specific control constants (AV1E_SET_...).
     */

    /*
     * 1. AV1E_SET_ENABLE_INTERINTRA_COMP
     * Corresponds to enabling interintra compound prediction.
     * Value: 0 or 1
     */
    int enable_interintra_comp = (value & 0x1) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, &enable_interintra_comp);

    /*
     * 2. AV1E_SET_ENABLE_RECT_TX
     * Corresponds to enabling rectangular transform.
     * Value: 0 or 1
     */
    int enable_rect_tx = (value & 0x2) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, &enable_rect_tx);

    /*
     * 3. AV1E_SET_ENABLE_DUAL_FILTER
     * Corresponds to enabling dual filter mode.
     * Value: 0 or 1
     */
    int enable_dual_filter = (value & 0x4) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, &enable_dual_filter);

    /*
     * 4. AV1E_SET_ENABLE_INTERINTRA_WEDGE
     * Corresponds to enabling wedge partitioning for interintra.
     * Value: 0 or 1
     */
    int enable_interintra_wedge = (value & 0x8) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, &enable_interintra_wedge);

    /*
     * 5. AV1E_SET_TUNE_CONTENT
     * Corresponds to tuning content type (e.g., AOM_CONTENT_DEFAULT, AOM_CONTENT_SCREEN).
     * Value: Map input to valid enum range.
     * Fixed type name from aom_tune_content_t to aom_tune_content.
     */
    aom_tune_content tune_content;
    /* Valid values are typically 0 to 2 (AOM_CONTENT_DEFAULT, AOM_CONTENT_SCREEN, AOM_CONTENT_VIDEO) */
    tune_content = (aom_tune_content)(value % 3);
    aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, &tune_content);

    /*
     * 6. AV1E_SET_ENABLE_SMOOTH_INTERINTRA
     * Corresponds to enabling smooth interintra.
     * Value: 0 or 1
     */
    int enable_smooth_interintra = (value & 0x10) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, &enable_smooth_interintra);

    /*
     * Additional exploration: Try setting invalid values or toggling flags multiple times
     * to trigger different internal states or error handling paths.
     */
    int invalid_val = -1;
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, &invalid_val);
    
    /* Toggle a flag on and off */
    int toggle = 1;
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, &toggle);
    toggle = 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, &toggle);

    /* Cleanup */
    cleanup_encoder_context(ctx);

    return 0;
}