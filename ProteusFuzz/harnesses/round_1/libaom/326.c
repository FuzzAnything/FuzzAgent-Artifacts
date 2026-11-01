// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_image.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"

/*
 * Fuzz driver for libaom codec control functions.
 * Targets:
 * 1. AV1E_SET_ENABLE_QM
 * 2. AV1E_SET_FORCE_VIDEO_MODE
 * 3. AV1D_SET_IS_ANNEXB
 * 4. AV1E_SET_SUPERBLOCK_SIZE
 * 5. AV1E_SET_LOSSLESS
 * 6. AV1E_SET_CHROMA_SUBSAMPLING_Y
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_326(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Extract control type from input to diversify testing */
    int control_type = (Size > 0) ? (Data[0] % 6) : 0;
    int value = (Size > 1) ? (Data[1] % 2) : 0; /* 0 or 1 for boolean flags */
    
    /* For non-boolean controls, try to use more data */
    int int_value = (Size > 2) ? (Data[2] % 100) : 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_err_t res;

    /*
     * Map control_type to specific API calls.
     * 0: AV1E_SET_ENABLE_QM
     * 1: AV1E_SET_FORCE_VIDEO_MODE
     * 2: AV1D_SET_IS_ANNEXB
     * 3: AV1E_SET_SUPERBLOCK_SIZE
     * 4: AV1E_SET_LOSSLESS
     * 5: AV1E_SET_CHROMA_SUBSAMPLING_Y
     */

    if (control_type == 2) {
        /* Decoder specific: AV1D_SET_IS_ANNEXB */
        const aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        /* Initialize decoder config with defaults */
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        /* Removed invalid fields: allow_highbitdepth, allow_multiref, etc. */
        /* Only set fields that exist in the struct */
        
        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_decoder(&ctx);
            return 0;
        }

        /* Invoke the target: AV1D_SET_IS_ANNEXB */
        aom_codec_control(&ctx, AV1D_SET_IS_ANNEXB, value);

        cleanup_decoder(&ctx);
    } else {
        /* Encoder specific controls */
        const aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg = {0};
        res = aom_codec_enc_config_default(iface, &cfg, 0);
        if (res != AOM_CODEC_OK) return 0;

        /* Set some basic config to ensure valid initialization */
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_encoder(&ctx);
            return 0;
        }

        /* Invoke target based on control_type */
        switch (control_type) {
            case 0:
                /* AV1E_SET_ENABLE_QM */
                aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, value);
                break;
            case 1:
                /* AV1E_SET_FORCE_VIDEO_MODE */
                aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, value);
                break;
            case 3:
                /* AV1E_SET_SUPERBLOCK_SIZE */
                /* Valid values are typically 16, 32, 64, 128. Map int_value to valid range */
                int sb_size = 16;
                if (int_value < 25) sb_size = 16;
                else if (int_value < 50) sb_size = 32;
                else if (int_value < 75) sb_size = 64;
                else sb_size = 128;
                aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
                break;
            case 4:
                /* AV1E_SET_LOSSLESS */
                aom_codec_control(&ctx, AV1E_SET_LOSSLESS, value);
                break;
            case 5:
                /* AV1E_SET_CHROMA_SUBSAMPLING_Y */
                /* Typically 0, 1, 2. Map int_value */
                int subsamp = (int_value % 3);
                aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, subsamp);
                break;
            default:
                break;
        }

        cleanup_encoder(&ctx);
    }

    return 0;
}