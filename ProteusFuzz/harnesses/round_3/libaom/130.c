// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
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
    // Removed invalid members: cpu_used, allow_highbitdepth, allow_intrabc, allow_screencontenttools
    // Using valid members based on error log
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Removed invalid type: aom_codec_frame_info_t
    
    // Prepare a small packet to feed the decoder to initialize internal state
    // Ensure we have enough data
    size_t payload_size = Size > 1024 ? 1024 : Size;
    if (payload_size > 0) {
        // Feed the decoder directly with fuzzer data
        // We pass NULL for the frame pointer as we are just interested in state changes for control calls
        aom_codec_decode(&ctx, Data, payload_size, NULL);
    }

    // 1. aom_codec_control_typechecked_AV1D_GET_FRAME_SIZE
    // Typically corresponds to AV1D_GET_FRAME_SIZE
    int32_t frame_size[2];
    aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, frame_size);

    // 2. aom_codec_control_typechecked_AV1D_GET_DISPLAY_SIZE
    // Typically corresponds to AV1D_GET_DISPLAY_SIZE
    int32_t display_size[2];
    aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, display_size);

    // 3. aom_codec_control_typechecked_AV1D_SET_OUTPUT_ALL_LAYERS
    // Typically corresponds to AV1D_SET_OUTPUT_ALL_LAYERS
    // This usually takes an int (0 or 1)
    int output_all_layers = (Data[0] % 2);
    aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, output_all_layers);

    // 4. aom_codec_control_typechecked_AV1D_GET_IMG_FORMAT
    // Typically corresponds to AV1D_GET_IMG_FORMAT
    aom_img_fmt_t img_fmt;
    aom_codec_control(&ctx, AV1D_GET_IMG_FORMAT, &img_fmt);

    // 5. aom_codec_control_typechecked_AV1D_SET_SKIP_FILM_GRAIN
    // Typically corresponds to AV1D_SET_SKIP_FILM_GRAIN
    int skip_film_grain = (Data[1] % 2);
    aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, skip_film_grain);

    // 6. aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT
    // Typically corresponds to AV1D_SET_OPERATING_POINT
    // Usually takes an int representing the operating point index
    int operating_point = Data[2] % 8; // Limit to a reasonable range
    aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, operating_point);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}