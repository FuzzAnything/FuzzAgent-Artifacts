// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size, int *width, int *height) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (ctx == NULL) return 0;

    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use fuzzing data to vary configuration slightly, with strict bounds checking
    int w = 128;
    int h = 128;

    if (size >= 7) {
        w = (data[0] % 1024) + 16;
        h = (data[1] % 1024) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[2] % 60) + 1;
        cfg.g_threads = (data[3] % 4) + 1;
        cfg.rc_target_bitrate = ((int)data[4] * 256 + (int)data[5]) * 1000;
        cfg.rc_end_usage = AOM_CBR;
        cfg.g_profile = data[6] % 4;
    } else {
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 500000;
        cfg.rc_end_usage = AOM_CBR;
        cfg.g_profile = 0;
    }

    cfg.g_w = w;
    cfg.g_h = h;

    if (width) *width = w;
    if (height) *height = h;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int initialized = 0;
    aom_codec_err_t err;
    int width = 128, height = 128;

    if (Size < 2) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size, &width, &height)) {
        initialized = 1;

        // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
        int val_rate_guide = (Data[0] % 2);
        err = aom_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val_rate_guide);
        (void)err;

        // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
        int val_angle_delta = (Data[1] % 2);
        err = aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val_angle_delta);
        (void)err;

        // Fuzz AV1E_SET_AQ_MODE
        int val_aq_mode = (Size > 2) ? (Data[2] % 4) : 0;
        err = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, val_aq_mode);
        (void)err;

        // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
        int val_1to4 = (Size > 3) ? (Data[3] % 2) : 0;
        err = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val_1to4);
        (void)err;

        // Fuzz AV1E_SET_QM_V
        int val_qm_v = (Size > 5) ? (int)(Data[4] * 256 + Data[5]) : 0;
        err = aom_codec_control(&ctx, AV1E_SET_QM_V, val_qm_v);
        (void)err;

        // Fuzz AV1E_SET_DELTAQ_STRENGTH
        int val_dq_strength = (Size > 6) ? (Data[6] % 64) : 0;
        err = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val_dq_strength);
        (void)err;

        // Try to encode a dummy frame to exercise the state
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 16);
        
        // Fill image with fuzzing data if available
        // Access planes correctly: img.planes[i] is uint8_t*, img.stride[i] is stride
        if (Size > 10 && img.planes[0] != NULL && img.planes[1] != NULL && img.planes[2] != NULL) {
            size_t y_size = (size_t)img.stride[0] * (size_t)img.h;
            size_t uv_size = (size_t)img.stride[1] * (size_t)img.h;
            
            for (size_t i = 0; i < y_size && i < Size; i++) {
                img.planes[0][i] = Data[i % Size];
            }
            for (size_t i = 0; i < uv_size && (i + 10) < Size; i++) {
                img.planes[1][i] = Data[(i + 10) % Size];
            }
            for (size_t i = 0; i < uv_size && (i + 20) < Size; i++) {
                img.planes[2][i] = Data[(i + 20) % Size];
            }
        }

        aom_codec_frame_flags_t flags = 0;
        if (Size > 30) flags = Data[30];
        
        err = aom_codec_encode(&ctx, &img, 0, 1, flags);
        (void)err;

        aom_img_free(&img);
    }

    cleanup_codec(&ctx);
    return 0;
}