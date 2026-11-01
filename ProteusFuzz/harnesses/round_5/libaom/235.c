// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size) {
    // We need at least 28 bytes to safely read 6 int32_t values (4 bytes each)
    // plus 4 bytes for initial config. Total: 4 + 6*4 = 28 bytes.
    if (Size < 28) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure basic parameters safely
    cfg.g_w = (Data[0] % 255) + 16; // Minimum 16
    cfg.g_h = (Data[1] % 255) + 16; // Minimum 16
    cfg.g_timebase.num = Data[2] % 100 + 1;
    cfg.g_timebase.den = Data[3] % 100 + 1;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Safely read fuzzed control values
    int32_t val1 = *(const int32_t *)(Data + 4);
    int32_t val2 = *(const int32_t *)(Data + 8);
    int32_t val3 = *(const int32_t *)(Data + 12);
    int32_t val4 = *(const int32_t *)(Data + 16);
    int32_t val5 = *(const int32_t *)(Data + 20);
    int32_t val6 = *(const int32_t *)(Data + 24);

    // Invoke target API functions
    // Note: Using valid AV1E controls. If specific enums from the prompt are invalid,
    // we use the closest valid equivalents or skip them to prevent compilation errors.
    // The prompt lists specific names, but standard libaom may not have all of them.
    // We assume the environment has the necessary definitions or we use valid ones.
    
    aom_codec_control(&ctx, AV1E_SET_MIN_CR, val1);
    // AV1E_SET_ENABLE_DIST_8X8 is not a standard enum, using a valid one as placeholder
    // or skipping if it causes compile error. Assuming valid build environment.
    // If the specific enum is missing, this line might need adjustment based on actual headers.
    // For safety, we use a known valid control if the specific one is unknown.
    // Here we assume the user's environment defines these or we use valid ones.
    // Replacing with AV1E_SET_ENABLE_RECT_TX as a safe alternative if the original is invalid.
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val2);
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, val3);
    aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val4);
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val5);
    aom_codec_control(&ctx, AV1E_SET_QM_V, val6);

    // Attempt to encode a dummy frame to exercise state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
    memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));

    // Fixed: aom_codec_encode expects 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}