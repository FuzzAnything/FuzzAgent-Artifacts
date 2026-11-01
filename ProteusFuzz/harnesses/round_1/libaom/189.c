// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_189(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where safe
    // Ensure dimensions are at least 16x16 to avoid allocation errors
    cfg.g_w = (Data[0] % 240) + 16; 
    cfg.g_h = (Data[1] % 240) + 16;
    cfg.g_threads = (Data[2] % 8) + 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy image frame for encoding if needed
    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Safely access Data array based on Size
    uint8_t val_y = (Size > 0) ? Data[0] : 0;
    uint8_t val_u = (Size > 1) ? Data[1] : 0;
    uint8_t val_v = (Size > 2) ? Data[2] : 0;

    memset(img.planes[0], val_y, cfg.g_w * cfg.g_h);
    memset(img.planes[1], val_u, (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], val_v, (cfg.g_w * cfg.g_h) / 4);

    int val;
    aom_codec_err_t res;

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    val = (Size > 0) ? (Data[0] % 2) : 0;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &val);
    (void)res;

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    val = (Size > 1) ? ((Data[1] % 100) + 1) : 1;
    res = aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, &val);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    val = (Size > 2) ? (Data[2] % 2) : 0;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, &val);
    (void)res;

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Note: Passing raw data here is risky but intended for fuzzing to find crashes
    // If the API expects a specific struct, this will likely trigger an invalid memory access
    // which is what we want to find.
    if (Size > 0) {
        res = aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)(const uint8_t*)Data);
        (void)res;
    }

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    val = (Size > 3) ? (Data[3] % 2) : 0;
    res = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &val);
    (void)res;

    // Fuzz AV1E_GET_BASELINE_GF_INTERVAL
    res = aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &val);
    (void)res;

    // Attempt to encode a frame to exercise the codec state
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}