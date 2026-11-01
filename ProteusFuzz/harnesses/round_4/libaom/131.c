// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to configure width/height safely
    uint32_t width = Data[0] + 1;
    uint32_t height = Data[1] + 1;
    if (width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Ensure we have enough data to read integers
    int val1 = (Size > 4) ? *(const int32_t *)(Data + 2) : 0;
    int val2 = (Size > 8) ? *(const int32_t *)(Data + 6) : 0;
    int val3 = (Size > 12) ? *(const int32_t *)(Data + 10) : 0;
    int val4 = (Size > 16) ? *(const int32_t *)(Data + 14) : 0;
    int val5 = (Size > 20) ? *(const int32_t *)(Data + 18) : 0;
    int val6 = (Size > 24) ? *(const int32_t *)(Data + 22) : 0;

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    if (aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val1) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing other states
    }

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val2) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    if (aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val3) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val4) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val5) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_S_FRAME_MODE
    if (aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val6) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Attempt to encode a dummy frame to exercise state changes
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(img.planes[0], 0, width * height);
    memset(img.planes[1], 128, (width * height) / 4);
    memset(img.planes[2], 128, (width * height) / 4);

    // Use correct constant AOME_SET_CPUUSED
    aom_codec_control(&ctx, AOME_SET_CPUUSED, 0);

    // Try to encode (fixed argument count: 5 args)
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}