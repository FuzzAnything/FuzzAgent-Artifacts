// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"

static int initialize_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure width and height safely
    unsigned int width = 16;
    unsigned int height = 16;
    
    if (size >= 8) {
        width = *(const uint32_t *)(data);
        height = *(const uint32_t *)(data + 4);
    }
    
    // Clamp dimensions to reasonable limits to avoid OOM
    if (width < 16) width = 16;
    if (height < 16) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!initialize_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Prepare a dummy frame to ensure encoder is in a valid state for control calls
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, ctx.config.enc->g_w, ctx.config.enc->g_h, 1);
    
    // Fill image with fuzzing data if available
    if (Size > 0) {
        for (int p = 0; p < 3; p++) {
            // Corrected: use stride and height to calculate plane size
            size_t plane_size = (size_t)img.stride[p] * img.h;
            size_t copy_size = (Size > plane_size) ? plane_size : Size;
            memcpy(img.planes[p], Data, copy_size);
        }
    }

    // Fuzz various control functions with boolean values derived from input
    int value = (Size > 0) ? (Data[0] & 1) : 0;

    // 1. AV1E_SET_REDUCED_REFERENCE_SET
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, value);

    // 2. AV1E_SET_ENABLE_CFL_INTRA
    value = (Size > 1) ? (Data[1] & 1) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, value);

    // 3. AV1E_SET_ENABLE_REF_FRAME_MVS
    value = (Size > 2) ? (Data[2] & 1) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, value);

    // 4. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    value = (Size > 3) ? (Data[3] & 1) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);

    // 5. AV1E_SET_ENABLE_TPL_MODEL
    value = (Size > 4) ? (Data[4] & 1) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, value);

    // 6. AV1E_SET_ENABLE_FILTER_INTRA
    value = (Size > 5) ? (Data[5] & 1) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, value);

    // Attempt to encode a frame to exercise the state changes
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}