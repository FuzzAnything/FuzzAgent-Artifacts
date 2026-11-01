// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    int last_quantizer = 0;
    int frame_corrupted = 0;
    int tile_count = 0;
    int sb_size = 0;
    int last_ref_updates = 0;

    /* Initialize configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0; /* Use default threads */
    cfg.w = 0;
    cfg.h = 0;
    /* Removed invalid fields: allow_highbitdepth, allow_multiscale */
    cfg.allow_lowbitdepth = 1;

    /* Get the AV1 decoder interface */
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    /* Initialize the decoder context */
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    /*
     * Attempt to decode the input data.
     * The input data is treated as a stream of encoded frames.
     */
    res = aom_codec_decode(&ctx, Data, Size, NULL);
    
    /* Try to get control values */
    /* AOMD_GET_LAST_QUANTIZER */
    if (aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer) == AOM_CODEC_OK) {
        /* Value retrieved */
    }

    /* AOMD_GET_FRAME_CORRUPTED */
    if (aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted) == AOM_CODEC_OK) {
        /* Value retrieved */
    }

    /* AV1D_GET_TILE_COUNT */
    if (aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count) == AOM_CODEC_OK) {
        /* Value retrieved */
    }

    /* AOMD_GET_SB_SIZE */
    if (aom_codec_control(&ctx, AOMD_GET_SB_SIZE, &sb_size) == AOM_CODEC_OK) {
        /* Value retrieved */
    }

    /* AOMD_GET_LAST_REF_UPDATES */
    if (aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates) == AOM_CODEC_OK) {
        /* Value retrieved */
    }

    /*
     * Drain the decoder to retrieve any buffered frames.
     */
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Frame retrieved */
        (void)img; 
    }

    /* Cleanup */
    cleanup_decoder(&ctx);

    return 0;
}