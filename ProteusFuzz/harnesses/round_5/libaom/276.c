// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>

// Helper function to safely initialize the decoder context
static aom_codec_ctx_t* init_decoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;

    aom_codec_err_t res = aom_codec_dec_init_ver(ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper function to clean up the decoder context
static void cleanup_decoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_276(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder_context(Data, Size);
    if (!ctx) return 0;

    aom_codec_stream_info_t si = {0};
    aom_codec_iter_t iter = NULL;

    // Fuzz aom_codec_decode
    aom_codec_err_t decode_res = aom_codec_decode(ctx, Data, Size, NULL);
    (void)decode_res; // Ignore result for fuzzing exploration

    // Fuzz aom_codec_get_stream_info
    aom_codec_err_t info_res = aom_codec_get_stream_info(ctx, &si);
    (void)info_res;

    // Fuzz aom_codec_get_frame
    aom_image_t *frame = aom_codec_get_frame(ctx, &iter);
    if (frame) {
        // Optionally inspect frame data if needed
        (void)frame;
    }

    // Fuzz aom_codec_error
    const char *error_msg = aom_codec_error(ctx);
    if (error_msg) {
        // Optionally use error_msg
        (void)error_msg;
    }

    cleanup_decoder_context(ctx);
    return 0;
}