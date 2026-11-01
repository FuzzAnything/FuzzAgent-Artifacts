// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;

    if (Size == 0) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize decoder configuration
    cfg.threads = 1;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Decode the input data
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Iterate over decoded frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Access frame properties to exercise code paths
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->planes[0];
        (void)img->stride[0];
        (void)img->bit_depth;
        (void)img->temporal_id;
        
        // Attempt to access metadata if present
        // Note: aom_metadata_array_t is an opaque type in the public API, 
        // so we cannot access its members directly without internal headers.
        // We simply check for existence to ensure the pointer is valid.
        if (img->metadata) {
            (void)img->metadata; 
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}