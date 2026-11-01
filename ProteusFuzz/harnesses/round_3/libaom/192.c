// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    // Using only valid members for aom_codec_dec_cfg_t
    // allow_lowbitdepth is the valid flag for bitdepth, others are encoder specific or non-existent
    cfg.allow_lowbitdepth = 1; 

    iface = aom_codec_av1_dx();
    if (!iface) {
        return -1;
    }

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    // Attempt to decode a small chunk to initialize stream info if possible
    if (size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_stream_info_t si;
    aom_codec_err_t res;
    int init_success = 0;

    if (Size == 0) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));
    memset(&si, 0, sizeof(si));

    // Initialize decoder
    if (init_decoder(&ctx, Data, Size) == 0) {
        init_success = 1;
    }

    if (init_success) {
        // 1. Fuzz aom_codec_decode
        // Pass the input data as coded data
        res = aom_codec_decode(&ctx, Data, Size, NULL);
        (void)res;

        // Try decoding with zero size
        res = aom_codec_decode(&ctx, NULL, 0, NULL);
        (void)res;

        // Try decoding with a subset of data
        if (Size > 10) {
            res = aom_codec_decode(&ctx, Data + 10, Size - 10, NULL);
            (void)res;
        }

        // 2. Fuzz aom_codec_get_stream_info
        res = aom_codec_get_stream_info(&ctx, &si);
        (void)res;

        // Try with NULL pointer to trigger error handling
        res = aom_codec_get_stream_info(&ctx, NULL);
        (void)res;
    }

    // Cleanup
    if (init_success) {
        aom_codec_destroy(&ctx);
    }

    return 0;
}