// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

/*
 * Fuzz driver for libaom decoder control functions.
 * Target functions:
 * - AOMD_GET_ALTREF_PRESENT
 * - AOMD_GET_TILE_INFO
 * - AOMD_GET_FRAME_FLAGS
 * - AOMD_GET_LAST_REF_UPDATES
 * - AOMD_GET_FRAME_CORRUPTED
 * - AOMD_GET_FWD_KF_PRESENT
 */

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 1;

    aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Feed the input data directly to the decoder */
    aom_codec_decode(ctx, data, size, NULL);

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    /* Try to get a decoded frame to ensure control commands have context */
    aom_codec_iter_t iter = NULL;
    const aom_image_t *img = aom_codec_get_frame(ctx, &iter);
    (void)img; /* Image may be NULL if decoding failed or no frame ready */

    int ret;
    int altref_present;
    aom_tile_info tile_info;
    int frame_flags;
    int last_ref_updates[16]; /* Assuming max refs */
    int frame_corrupted;
    int fwd_kf_present;

    memset(&tile_info, 0, sizeof(tile_info));
    memset(last_ref_updates, 0, sizeof(last_ref_updates));

    /* 1. AOMD_GET_ALTREF_PRESENT */
    ret = aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
    (void)ret; (void)altref_present;

    /* 2. AOMD_GET_TILE_INFO */
    ret = aom_codec_control(ctx, AOMD_GET_TILE_INFO, &tile_info);
    (void)ret; (void)tile_info;

    /* 3. AOMD_GET_FRAME_FLAGS */
    ret = aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    (void)ret; (void)frame_flags;

    /* 4. AOMD_GET_LAST_REF_UPDATES */
    ret = aom_codec_control(ctx, AOMD_GET_LAST_REF_UPDATES, last_ref_updates);
    (void)ret; (void)last_ref_updates;

    /* 5. AOMD_GET_FRAME_CORRUPTED */
    ret = aom_codec_control(ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
    (void)ret; (void)frame_corrupted;

    /* 6. AOMD_GET_FWD_KF_PRESENT */
    ret = aom_codec_control(ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);
    (void)ret; (void)fwd_kf_present;

    cleanup_decoder(ctx);
    return 0;
}