// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_decoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_275(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    // Use aom_codec_av1_cx to get the decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;

    aom_codec_flags_t flags = 0;
    int ver = AOM_DECODER_ABI_VERSION;

    // Initialize decoder
    if (aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, ver) != AOM_CODEC_OK) {
        cleanup_decoder(&ctx);
        return 0;
    }

    // Fuzz decoding with provided data
    aom_codec_err_t decode_res = aom_codec_decode(&ctx, Data, Size, NULL);

    // Attempt to get stream info
    aom_codec_stream_info_t si = {0};
    aom_codec_get_stream_info(&ctx, &si);

    // Iterate over decoded frames
    aom_codec_iter_t iter = NULL;
    while (aom_codec_get_frame(&ctx, &iter) != NULL) {
        // Frame processing logic can be added here if needed
        // For fuzzing, we just consume the frame
    }

    // Check for errors
    const char *error_msg = aom_codec_error(&ctx);
    if (error_msg) {
        // Error message retrieved, no action needed for fuzzing
        (void)error_msg;
    }

    // Cleanup
    cleanup_decoder(&ctx);

    return 0;
}