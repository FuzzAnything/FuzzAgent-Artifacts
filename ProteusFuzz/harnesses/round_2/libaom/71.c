// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomdx.h"

// Helper function to initialize a decoder context
static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_dec_cfg_t cfg;
    // Initialize config structure manually
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed field names based on compiler error: allow_highbitdepth -> allow_lowbitdepth
    // Removed allow_profile_2 and allow_profile_3 as they do not exist in this version
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Feed data to the decoder if available
    if (data && size > 0) {
        // Use the correct control IDs from aomdx.h
        aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 0); 
        aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, 0); 
        
        aom_codec_err_t err = aom_codec_decode(ctx, data, size, NULL);
        // Decoding might fail with random data, which is expected in fuzzing
        // We continue to test control functions even if decode failed
        (void)err;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize decoder with the fuzzed data
    if (init_decoder(&ctx, Data, Size) != 0) {
        // Even if init/decode fails, we can try to test control functions on the partially initialized context
        // or just return. However, aom_codec_control usually requires a valid context.
        // We proceed to test control calls if the context was at least allocated by init.
    }

    // Fuzz AV1D_SET_OUTPUT_ALL_LAYERS
    // Takes an int value (0 or 1 usually)
    if (Size > 0) {
        int val = Data[0] & 0x01;
        aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, val);
    }

    // Fuzz AV1D_SET_SKIP_FILM_GRAIN
    // Takes an int value (0 or 1)
    if (Size > 1) {
        int val = (Data[1] >> 1) & 0x01;
        aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, val);
    }

    // Fuzz AV1D_SET_OPERATING_POINT
    // Takes an int value (operating point index)
    if (Size > 2) {
        int val = Data[2] & 0x7F; // Limit to reasonable range
        aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, val);
    }

    // Fuzz AV1D_GET_FRAME_HEADER_INFO
    // Returns aom_frame_header_info_t *
    // We just call it to trigger the code path.
    aom_codec_control(&ctx, AV1D_GET_FRAME_HEADER_INFO, NULL);

    // Fuzz AV1D_GET_DISPLAY_SIZE
    // Returns aom_codec_err_t, usually expects a pointer to aom_image_t or similar to fill.
    if (Size >= sizeof(void*)) {
        // Create a dummy buffer to act as the target for display size
        aom_image_t *img = malloc(sizeof(aom_image_t));
        if (img) {
            memset(img, 0, sizeof(aom_image_t));
            aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, img);
            free(img);
        }
    } else {
        aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, NULL);
    }

    // Fuzz AV1D_GET_FRAME_SIZE
    // Similar to display size
    if (Size >= sizeof(void*)) {
        aom_image_t *img = malloc(sizeof(aom_image_t));
        if (img) {
            memset(img, 0, sizeof(aom_image_t));
            aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, img);
            free(img);
        }
    } else {
        aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, NULL);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}