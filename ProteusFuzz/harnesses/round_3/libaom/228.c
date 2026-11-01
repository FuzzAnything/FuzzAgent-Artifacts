// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_228(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    // Initialize encoder config
    ret = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some configuration parameters safely
    cfg.g_w = (Data[0] % 256) + 16; // Ensure width is at least 16
    cfg.g_h = (Data[1] % 256) + 16; // Ensure height is at least 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1; // Ensure denominator is at least 1

    // Initialize codec context
    ret = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Prepare dummy image for encoding
    aom_image_t img;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    
    // Allocate planes
    img.planes[0] = malloc(img.w * img.h);
    img.planes[1] = malloc((img.w / 2) * (img.h / 2));
    img.planes[2] = malloc((img.w / 2) * (img.h / 2));
    
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        cleanup_codec(&ctx);
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        return 0;
    }
    
    img.stride[0] = img.w;
    img.stride[1] = img.w / 2;
    img.stride[2] = img.w / 2;
    
    // Initialize planes
    img.planes[0][0] = 0; 
    img.planes[1][0] = 0;
    img.planes[2][0] = 0;
    
    img.bit_depth = 8;
    // Removed img.cpu_used = 0; as it is not a member of struct aom_image in newer versions

    // Fill image with fuzzing data
    size_t plane0_size = img.w * img.h;
    size_t copy_size = (Size > plane0_size) ? plane0_size : Size;
    memcpy(img.planes[0], Data, copy_size);
    memset(img.planes[1], 0, (img.w / 2) * (img.h / 2));
    memset(img.planes[2], 0, (img.w / 2) * (img.h / 2));

    // Invoke the target API functions with fuzzing data
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;
    int val5 = (Size > 4) ? (int)Data[4] : 0;
    int val6 = (Size > 5) ? (int)Data[5] : 0;

    // Attempt to call the target functions
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val3);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val4);
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val5);
    aom_codec_control(&ctx, AV1E_SET_MIN_CR, val6);

    // Encode a frame to exercise the codec
    aom_enc_frame_flags_t flags = 0;
    aom_codec_err_t encode_ret = aom_codec_encode(&ctx, &img, 0, 1, flags);
    if (encode_ret != AOM_CODEC_OK) {
        // Encoding failed, but we continue to cleanup
    }

    // Cleanup
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    cleanup_codec(&ctx);

    return 0;
}