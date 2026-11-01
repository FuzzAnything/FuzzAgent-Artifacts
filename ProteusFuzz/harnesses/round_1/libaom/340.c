// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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

static int try_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, ctrl_id, value);
}

int LLVMFuzzerTestOneInput_340(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    int ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    // Initialize encoder configuration
    ret = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to seed configuration values
    cfg.g_w = (Data[0] % 256) + 16; // Ensure width >= 16
    cfg.g_h = (Data[1] % 256) + 16; // Ensure height >= 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.g_lag_in_frames = (Data[4] % 25);
    cfg.rc_target_bitrate = (Data[5] * 256 + Data[6]) + 100;
    cfg.rc_min_quantizer = (Data[7] % 56) + 1;
    cfg.rc_max_quantizer = (Data[8] % 56) + 1 + cfg.rc_min_quantizer;
    cfg.g_error_resilient = (Data[9] % 2);

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = img.w;
    img.d_h = img.h;
    img.planes[0] = NULL;
    img.planes[1] = NULL;
    img.planes[2] = NULL;
    img.stride[0] = 0;
    img.stride[1] = 0;
    img.stride[2] = 0;
    
    // Allocate image data using correct format constant
    // aom_img_alloc returns a pointer to the allocated image, not an error code
    aom_image_t *img_ptr = aom_img_alloc(&img, AOM_IMG_FMT_I420, img.w, img.h, 16);
    if (!img_ptr) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzzing data using 'planes' instead of 'plane'
    size_t y_size = (size_t)img.w * img.h;
    size_t uv_size = y_size / 4;
    
    if (Size >= y_size + uv_size + uv_size) {
        memcpy(img.planes[0], Data, y_size);
        memcpy(img.planes[1], Data + y_size, uv_size);
        memcpy(img.planes[2], Data + y_size + uv_size, uv_size);
    } else {
        // Fallback if data is too small
        memset(img.planes[0], Data[0], y_size);
        memset(img.planes[1], Data[1], uv_size);
        memset(img.planes[2], Data[2], uv_size);
    }

    // Encode one frame to ensure context is active
    // Fixed signature: aom_codec_encode(ctx, img, pts, duration, flags)
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Attempt to invoke the target control functions
    // Mapping the requested typechecked functions to standard AV1E_* control IDs
    // Values derived from input data to diversify states
    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    int val3 = (Size > 2) ? (Data[2] % 2) : 0;
    int val4 = (Size > 3) ? (Data[3] % 64) : 0;
    int val5 = (Size > 4) ? (Data[4] % 16) : 0;
    int val6 = (Size > 5) ? (Data[5] % 2) : 0;

    // Try AV1E_SET_ENABLE_QM
    try_control(&ctx, AV1E_SET_ENABLE_QM, val1);

    // Try AV1E_SET_ENABLE_INTERINTER_WEDGE
    try_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val3);

    // Try AV1E_SET_MIN_CR
    try_control(&ctx, AV1E_SET_MIN_CR, val4);

    // Try AV1E_SET_COEFF_COST_UPD_FREQ
    try_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val5);

    // Try AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    try_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val6);

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}