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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int initialize_encoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t *c = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!c) return 0;
    memset(c, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(c);
        return 0;
    }

    // Fuzz configuration values based on input data if size allows
    if (size > 0) cfg.g_w = (data[0] % 1024) + 16;
    if (size > 1) cfg.g_h = (data[1] % 1024) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 10000) + 100;

    if (aom_codec_enc_init(c, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(c);
        return 0;
    }

    *ctx = c;
    return 1;
}

int LLVMFuzzerTestOneInput_274(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = NULL;
    int initialized = initialize_encoder(&ctx, Data, Size);

    if (!initialized) return 0;

    // Extract control values from input data to fuzz various states
    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    int val2 = (Size > 1) ? (Data[1] % 2) : 0;
    int val3 = (Size > 2) ? (Data[2] % 2) : 0;
    int val4 = (Size > 3) ? (Data[3] % 2) : 0;
    int val5 = (Size > 4) ? (Data[4] % 2) : 0;
    int val6 = (Size > 5) ? (Data[5] % 2) : 0;

    // Invoke target API functions with fuzzed values
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, val2);
    aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val3);
    aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val4);
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, val5);
    aom_codec_control(ctx, AV1E_SET_ROW_MT, val6);

    // Attempt to encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    
    // Fill image with fuzz data
    for (int i = 0; i < 16 * 16; i++) {
        img.planes[0][i] = Data[i % Size];
    }
    // Simple subsampling for chroma
    for (int i = 0; i < 8 * 8; i++) {
        img.planes[1][i] = Data[(i + 1) % Size];
        img.planes[2][i] = Data[(i + 2) % Size];
    }

    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}