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

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_MAX_REFERENCE_FRAMES
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_RENDER_SIZE
 * - AV1E_SET_ENABLE_DNL_DENOISING
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int width, height;

    if (!data || size < 4) {
        return NULL;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Extract width and height from fuzz input to vary test cases */
    width = (data[0] % 128) + 16;
    height = (data[1] % 128) + 16;
    /* Ensure alignment */
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_usage = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_pass = 0;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t err;
    int val_int;
    int width, height;
    uint8_t mode;
    int render_w, render_h;

    if (Size < 4) {
        return 0;
    }

    ctx = init_encoder(Data, Size);
    if (!ctx) {
        return 0;
    }

    /* Extract parameters from input data */
    val_int = Data[0] % 16; /* Max reference frames: 0-15 */
    mode = Data[1] % 4;     /* S-Frame mode: 0-3 */
    width = (Data[2] % 128) + 16;
    height = (Data[3] % 128) + 16;
    /* Align dimensions */
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;

    /* 1. AV1E_SET_RTC_EXTERNAL_RC */
    /* Typically takes a struct or int. Assuming int for fuzzing variation. */
    if (Size > 4) {
        err = aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, Data[4] % 2);
        (void)err;
    }

    /* 2. AV1E_SET_MAX_REFERENCE_FRAMES */
    err = aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val_int);
    (void)err;

    /* 3. AV1E_SET_ENABLE_REF_FRAME_MVS */
    /* Boolean flag */
    if (Size > 5) {
        err = aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, Data[5] % 2);
        (void)err;
    }

    /* 4. AV1E_SET_S_FRAME_MODE */
    err = aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, mode);
    (void)err;

    /* 5. AV1E_SET_RENDER_SIZE */
    /* AV1E_SET_RENDER_SIZE expects a pointer to an int array [width, height] or similar.
     * Based on libaom source, it often takes an int array of size 2.
     */
    int render_dims[2];
    render_dims[0] = width;
    render_dims[1] = height;
    err = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, render_dims);
    (void)err;

    /* 6. AV1E_SET_ENABLE_DNL_DENOISING */
    /* Boolean flag */
    if (Size > 6) {
        err = aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, Data[6] % 2);
        (void)err;
    }

    /* Cleanup */
    cleanup_encoder(ctx);

    return 0;
}