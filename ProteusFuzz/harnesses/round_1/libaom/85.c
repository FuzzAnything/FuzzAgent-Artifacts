// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
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

/*
 * Fuzz driver for libaom focusing on aom_codec_get_stream_info.
 * Fixed compilation errors regarding struct members and function signatures.
 */

static int try_decode_and_get_stream_info(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    aom_codec_stream_info_t si;
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame_img = NULL;

    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration with valid members
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    // Removed cfg.cpu_used as it is not present in all versions of aom_codec_dec_cfg

    // Initialize decoder context
    err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Submit data to decoder
    // Signature: aom_codec_err_t aom_codec_decode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size, aom_codec_iter_t *iter);
    err = aom_codec_decode(&ctx, data, size, &iter);
    if (err != AOM_CODEC_OK) {
        // Parsing might fail, but we can still try to get stream info if initialized
    }

    // Try to get stream info
    memset(&si, 0, sizeof(si));
    err = aom_codec_get_stream_info(&ctx, &si);

    // Attempt to decode frames to potentially update stream info further
    // Signature: aom_image_t *aom_codec_get_frame(aom_codec_ctx_t *ctx, aom_codec_iter_t *iter);
    while ((frame_img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Frame processed, no need to check pkt.kind as we get the image directly
        // The frame is owned by the context until the next call or destroy
    }

    // Try to get stream info again after processing frames
    memset(&si, 0, sizeof(si));
    err = aom_codec_get_stream_info(&ctx, &si);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Basic sanity check
    try_decode_and_get_stream_info(Data, Size);

    return 0;
}