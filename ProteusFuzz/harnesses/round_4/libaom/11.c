// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_decode_path(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_stream_info_t si;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    if (Size == 0) return 0;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Removed invalid 'allow_highbitdepth' member. 
    // Default behavior allows high bitdepth, or specific flags can be used if needed.
    
    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Feed data to decoder
    ret = aom_codec_decode(&ctx, Data, Size, NULL);
    (void)ret;

    // Try to get stream info
    memset(&si, 0, sizeof(si));
    si.is_annexb = 1;
    ret = aom_codec_get_stream_info(&ctx, &si);
    (void)ret;

    // Iterate frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Frame is valid here, do nothing to avoid allocation issues in fuzzer
        // Just ensure we consume the iterator
    }

    // Check error string
    const char *err = aom_codec_error(&ctx);
    (void)err;

    cleanup_codec(&ctx);
    return 0;
}

static int fuzz_peek_path(const uint8_t *Data, size_t Size) {
    aom_codec_iface_t *iface;
    aom_codec_stream_info_t si;
    aom_codec_err_t ret;

    if (Size == 0) return 0;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&si, 0, sizeof(si));
    si.is_annexb = 1;

    ret = aom_codec_peek_stream_info(iface, Data, Size, &si);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz decoding path
    fuzz_decode_path(Data, Size);

    // Fuzz peek path
    fuzz_peek_path(Data, Size);

    // Fuzz error retrieval with null context (safe per spec)
    const char *err = aom_codec_error(NULL);
    (void)err;

    return 0;
}