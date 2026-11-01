// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_234(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    aom_codec_err_t ret;
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));

    // Feed data to the decoder
    ret = aom_codec_decode(&ctx, Data, Size, &iter);

    // Test aom_codec_get_stream_info
    ret = aom_codec_get_stream_info(&ctx, &si);

    // Test control functions
    // AV1D_GET_IMG_FORMAT
    int img_fmt = 0;
    ret = aom_codec_control(&ctx, AV1D_GET_IMG_FORMAT, &img_fmt);
    
    // AV1D_GET_DISPLAY_SIZE
    // The API expects a pointer to an array of 2 integers (width, height)
    int display_size[2] = {0, 0};
    ret = aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, display_size);
    
    // AV1D_SET_OUTPUT_ALL_LAYERS
    int output_all_layers = 1;
    ret = aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, &output_all_layers);
    
    // AV1D_SET_OPERATING_POINT
    int operating_point = 0;
    ret = aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, &operating_point);
    
    // AV1D_GET_FRAME_SIZE
    // The API expects a pointer to an array of 2 integers (width, height)
    int frame_size[2] = {0, 0};
    ret = aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, frame_size);

    cleanup_codec(&ctx);

    return 0;
}