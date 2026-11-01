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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_OVERLAY
 * - AV1E_SET_ENABLE_SMOOTH_INTERINTRA
 * - AV1E_SET_ENABLE_INTRA_EDGE_FILTER
 * - AV1E_SET_ENABLE_DNL_DENOISING
 * - AV1E_SET_ENABLE_CHROMA_DELTAQ
 * - AV1E_SET_MIN_PARTITION_SIZE
 */

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_214(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = NULL;
    int ret;

    /* Get AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    /* Initialize encoder configuration */
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    /* Set basic config parameters */
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;

    /* Initialize the encoder context */
    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    /*
     * Use input data to drive diverse control calls.
     * We map bytes to control types and values.
     */
    size_t idx = 0;
    while (idx < Size) {
        uint8_t control_type = Data[idx++];
        uint8_t value = (idx < Size) ? Data[idx++] : 0;

        /* Map control_type to specific AV1E controls */
        switch (control_type % 6) {
            case 0:
                /* AV1E_SET_ENABLE_OVERLAY: expects int (0 or 1) */
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, value & 1);
                break;
            case 1:
                /* AV1E_SET_ENABLE_SMOOTH_INTERINTRA: expects int (0 or 1) */
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value & 1);
                break;
            case 2:
                /* AV1E_SET_ENABLE_INTRA_EDGE_FILTER: expects int (0 or 1) */
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value & 1);
                break;
            case 3:
                /* AV1E_SET_ENABLE_DNL_DENOISING: expects int (0 or 1) */
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, value & 1);
                break;
            case 4:
                /* AV1E_SET_ENABLE_CHROMA_DELTAQ: expects int (0 or 1) */
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, value & 1);
                break;
            case 5:
                /* AV1E_SET_MIN_PARTITION_SIZE: expects int (partition size) */
                /* Valid sizes are typically 4, 8, 16, 32, 64, 128, 256 */
                int min_part_size = (value % 7);
                int sizes[] = {4, 8, 16, 32, 64, 128, 256};
                if (min_part_size >= 0 && min_part_size < 7) {
                    ret = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, sizes[min_part_size]);
                } else {
                    ret = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, 4);
                }
                break;
        }

        /* Check for errors, but continue fuzzing to explore more states */
        if (ret != AOM_CODEC_OK) {
            /* Optional: could break or log, but continuing is better for fuzzing */
        }
    }

    cleanup_codec(ctx);
    return 0;
}