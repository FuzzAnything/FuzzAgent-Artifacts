// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
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
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CQ;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    int val1 = Data[0];
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val1);

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // This likely expects a pointer to a struct or array. We use a portion of the input.
    if (Size > 1) {
        int val2 = Data[1];
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &val2);
    }

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    if (Size > 2) {
        int val3 = Data[2];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val3);
    }

    // Fuzz AV1E_SET_POSTENCODE_DROP_RTC
    if (Size > 3) {
        int val4 = Data[3];
        aom_codec_control(&ctx, AV1E_SET_POSTENCODE_DROP_RTC, val4);
    }

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    if (Size > 4) {
        int val5 = Data[4];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, val5);
    }

    // Fuzz AV1E_SET_FP_MT_UNIT_TEST
    if (Size > 5) {
        int val6 = Data[5];
        aom_codec_control(&ctx, AV1E_SET_FP_MT_UNIT_TEST, val6);
    }

    // Attempt to encode a frame to exercise the state changes
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    
    // Safe access to Data with bounds checking
    uint8_t fill_val_0 = (Size > 0) ? Data[0] : 0;
    uint8_t fill_val_1 = (Size > 1) ? Data[1] : 0;
    uint8_t fill_val_2 = (Size > 2) ? Data[2] : 0;

    memset(img.planes[0], fill_val_0, 64 * 64);
    memset(img.planes[1], fill_val_1, 32 * 32);
    memset(img.planes[2], fill_val_2, 32 * 32);

    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}