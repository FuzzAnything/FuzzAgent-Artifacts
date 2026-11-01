// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include "aom/aom.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_GLOBAL_MOTION
 * - AV1E_SET_DISABLE_TRELLIS_QUANT
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_ALLOW_REF_FRAME_MVS
 * - AV1E_SET_ERROR_RESILIENT_MODE
 * - AV1E_SET_ENABLE_DIFF_WTD_COMP
 */

static void free_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

static int init_codec(aom_codec_ctx_t *ctx, const aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);

    /* Use input data to configure basic encoding parameters */
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 10;
    cfg.rc_overshoot_pct = 10;
    cfg.rc_resize_mode = 0;

    if (!init_codec(&ctx, &cfg)) {
        return 0;
    }

    /* Create a dummy image frame */
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(&img.planes[0][0], 0, img.stride[0] * cfg.g_h);
    memset(&img.planes[1][0], 128, img.stride[1] * ((cfg.g_h + 1) / 2));
    memset(&img.planes[2][0], 128, img.stride[2] * ((cfg.g_h + 1) / 2));

    /* Control IDs to fuzz */
    int control_ids[] = {
        AV1E_SET_ENABLE_GLOBAL_MOTION,
        AV1E_SET_DISABLE_TRELLIS_QUANT,
        AV1E_SET_ENABLE_REF_FRAME_MVS,
        AV1E_SET_ALLOW_REF_FRAME_MVS,
        AV1E_SET_ERROR_RESILIENT_MODE,
        AV1E_SET_ENABLE_DIFF_WTD_COMP
    };
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

    /* Fuzz each control function with values derived from input */
    for (int i = 0; i < num_controls; i++) {
        int val = 0;
        if (Size > (size_t)(i + 2)) {
            val = Data[i + 2] % 2;
        } else if (Size > (size_t)(i + 1)) {
            val = Data[i + 1] % 2;
        } else {
            val = Data[0] % 2;
        }

        /* Invoke the control function */
        aom_codec_err_t ret = aom_codec_control(&ctx, control_ids[i], val);
        (void)ret; /* Suppress unused warning */
    }

    /* Encode one frame to exercise the state */
    /* Fixed: aom_codec_encode expects 5 arguments: ctx, img, pts, duration, flags */
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    /* Cleanup */
    aom_codec_destroy(&ctx);
    free_image(&img);

    return 0;
}