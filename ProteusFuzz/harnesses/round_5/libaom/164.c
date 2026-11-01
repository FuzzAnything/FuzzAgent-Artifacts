// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom_external_partition.h>
#include <aom_decoder.h>
#include <aom_frame_buffer.h>
#include <aom_encoder.h>
#include <aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;
    aom_codec_stream_info_t si;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;

    // Initialize decoder configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    // Removed invalid members: cpu_used, error_resilient, low_delay
    // These fields do not exist in the current aom_codec_dec_cfg_t structure

    // Initialize decoder context
    res = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_get_stream_info with uninitialized data (should return UNSUP_BITSTREAM)
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&ctx, &si);

    // Fuzz aom_codec_decode with provided input data
    // We pass the entire input buffer as coded data
    res = aom_codec_decode(&ctx, Data, Size, NULL);
    (void)res; // Ignore result, we just want to exercise the code path

    // Try to get stream info after decode
    memset(&si, 0, sizeof(si));
    res = aom_codec_get_stream_info(&ctx, &si);
    (void)res;

    // Iterate over decoded frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Just access some fields to ensure the frame is valid
        (void)img->w;
        (void)img->h;
        (void)img->fmt;
        // Do not free the image, it is managed by the codec context
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}