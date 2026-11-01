// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"

/*
 * Helper function to initialize the encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

/*
 * Helper function to create a dummy image frame using aom library functions.
 * Returns allocated image or NULL on failure.
 */
static aom_image_t *create_dummy_image(unsigned int w, unsigned int h) {
    aom_image_t *img = (aom_image_t *)aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 16);
    if (!img) return NULL;

    /* Fill with simple pattern to ensure non-zero data if needed */
    memset(img->planes[0], 0, img->stride[0] * h);
    memset(img->planes[1], 0, img->stride[1] * (h / 2));
    memset(img->planes[2], 0, img->stride[2] * (h / 2));

    return img;
}

/*
 * Helper function to free image resources.
 */
static void free_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

/*
 * Helper function to cleanup encoder context.
 */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    /* Initialize encoder */
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    uint16_t width = (Data[0] % 128) + 16;
    uint16_t height = (Data[1] % 128) + 16;
    if (width < 16 || height < 16) {
        width = 16; height = 16;
    }

    /* Create dummy frame */
    aom_image_t *img = create_dummy_image(width, height);
    if (!img) {
        cleanup_encoder(&ctx);
        return 0;
    }

    /*
     * Invoke the target control functions using the correct enum identifiers
     * found in aomcx.h (AV1E_... prefix).
     */

    /* 1. AV1E_SET_ENABLE_QM */
    int enable_qm = (Size > 2) ? (Data[2] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm);
    (void)ret;

    /* 2. AV1E_SET_QM_V */
    int qm_v = (Size > 3) ? (int)Data[3] : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_QM_V, qm_v);
    (void)ret;

    /* 3. AV1E_SET_TILE_ROWS */
    int tile_rows = (Size > 4) ? (Data[4] % 8) : 1;
    ret = aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, tile_rows);
    (void)ret;

    /* 4. AV1E_SET_SUPERBLOCK_SIZE */
    int sb_size = (Size > 5) ? (Data[5] % 3) : 0; 
    ret = aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
    (void)ret;

    /* 5. AV1E_SET_TILE_COLUMNS */
    int tile_cols = (Size > 6) ? (Data[6] % 8) : 1;
    ret = aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);
    (void)ret;

    /* 6. AV1E_SET_QM_U */
    int qm_u = (Size > 7) ? (int)Data[7] : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_QM_U, qm_u);
    (void)ret;

    /*
     * Attempt to encode a frame to trigger state changes and potential crashes
     * after setting the parameters.
     * Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
     */
    if (aom_codec_encode(&ctx, img, 0, 1, 0) == AOM_CODEC_OK) {
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        /* Fixed: aom_codec_get_cx_data takes 2 arguments (ctx, iter) */
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            /* Consume packet */
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                /* Optional: free frame if needed, but usually handled by libaom internally */
            }
        }
    }

    /* Cleanup */
    free_image(img);
    cleanup_encoder(&ctx);

    return 0;
}