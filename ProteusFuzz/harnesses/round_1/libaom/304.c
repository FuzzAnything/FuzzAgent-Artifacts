// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int fuzz_decode(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_stream_info_t si;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    const char *err_str;

    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;

    // Get decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Initialize decoder
    memset(&ctx, 0, sizeof(ctx));
    res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Decode data
    if (Size > 0) {
        res = aom_codec_decode(&ctx, Data, Size, NULL);
        // Ignore decode errors, we want to exercise the error paths in other functions
    }

    // Get stream info
    memset(&si, 0, sizeof(si));
    res = aom_codec_get_stream_info(&ctx, &si);
    // Check return value but continue to exercise other APIs

    // Get error string
    err_str = aom_codec_error(&ctx);
    (void)err_str; // Suppress unused variable warning

    // Iterate frames
    iter = NULL;
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Frame processed, do nothing or inspect
        (void)img;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_304(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Basic fuzzing logic
    fuzz_decode(Data, Size);

    // Test with NULL context for error handling (if safe)
    // Note: aom_codec_error handles NULL, others might not.
    // We avoid calling functions that crash on NULL context directly without init.
    const char *err_null = aom_codec_error(NULL);
    (void)err_null;

    return 0;
}