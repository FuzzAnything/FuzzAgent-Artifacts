// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
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

int LLVMFuzzerTestOneInput_220(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 1;

    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Test aom_codec_peek_stream_info
    aom_codec_stream_info_t si_peek;
    memset(&si_peek, 0, sizeof(si_peek));
    si_peek.is_annexb = 0; // Try non-annexb first
    err = aom_codec_peek_stream_info(iface, Data, Size, &si_peek);
    (void)err; // Ignore error for fuzzing robustness

    si_peek.is_annexb = 1; // Try annexb
    err = aom_codec_peek_stream_info(iface, Data, Size, &si_peek);
    (void)err;

    // Test aom_codec_decode
    err = aom_codec_decode(&ctx, Data, Size, NULL);
    if (err == AOM_CODEC_OK || err == AOM_CODEC_ERROR) {
        // If decode succeeded or returned error (not fatal), try to get frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            // Frame is valid here, do nothing or inspect
            (void)img;
        }
    }

    // Test aom_codec_get_stream_info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    err = aom_codec_get_stream_info(&ctx, &si);
    (void)err;

    // Test with NULL pointer to trigger INVALID_PARAM
    err = aom_codec_get_stream_info(&ctx, NULL);
    (void)err;

    // Test aom_codec_get_frame with NULL iterator
    aom_image_t *img_null;
    img_null = aom_codec_get_frame(&ctx, NULL);
    (void)img_null;

    // Cleanup
    cleanup_decoder(&ctx);

    return 0;
}