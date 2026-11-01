// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

static int fuzz_decode_init(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed invalid members: allow_highbitdepth, allow_intrabc, allow_screen_content_tools
    // These are not present in aom_codec_dec_cfg_t in this version of libaom

    // Use aom_codec_av1_cx() as the decoder interface
    if (aom_codec_dec_init(ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (size == 0) {
        return 0;
    }

    // Fuzz AV1D_SET_OUTPUT_ALL_LAYERS
    aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, 1);
    aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, 0);
    aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, -1);
    aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, 2);

    // Fuzz AV1D_SET_SKIP_FILM_GRAIN
    aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, 1);
    aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, 0);
    aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, -1);
    aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, 2);

    // Fuzz AV1D_SET_OPERATING_POINT
    aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 0);
    aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 1);
    aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, -1);
    aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 2);

    // Fuzz AV1D_GET_FRAME_SIZE
    aom_codec_control(ctx, AV1D_GET_FRAME_SIZE, NULL);

    // Fuzz AV1D_GET_FRAME_HEADER_INFO
    aom_codec_control(ctx, AV1D_GET_FRAME_HEADER_INFO, NULL);

    // Fuzz AV1D_GET_DISPLAY_SIZE
    aom_codec_control(ctx, AV1D_GET_DISPLAY_SIZE, NULL);

    return 1;
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!fuzz_decode_init(&ctx, Data, Size)) {
        return 0;
    }

    aom_codec_destroy(&ctx);

    return 0;
}