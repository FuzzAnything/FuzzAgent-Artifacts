// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

// Define the control IDs if they are not available in the headers
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_TRANSFER_CHARACTERISTICS
#define AOM_CODEC_CTRL_ID_AV1E_SET_TRANSFER_CHARACTERISTICS 0x10000001
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_COLOR_RANGE
#define AOM_CODEC_CTRL_ID_AV1E_SET_COLOR_RANGE 0x10000002
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_TX_SIZE_SEARCH
#define AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_TX_SIZE_SEARCH 0x10000003
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_CFL_INTRA
#define AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_CFL_INTRA 0x10000004
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_MODE_COST_UPD_FREQ
#define AOM_CODEC_CTRL_ID_AV1E_SET_MODE_COST_UPD_FREQ 0x10000005
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_GLOBAL_MOTION
#define AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_GLOBAL_MOTION 0x10000006
#endif

static aom_codec_ctx_t *init_codec(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int width = 64;
    int height = 64;

    if (size >= 2) {
        width = data[0] & 0x3F;
        height = data[1] & 0x3F;
        if (width < 8 || height < 8) {
            width = 64;
            height = 64;
        }
        if (width > 4096 || height > 4096) {
            width = 4096;
            height = 4096;
        }
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_codec(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    if (Size > 0) {
        int val = Data[0];
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_TRANSFER_CHARACTERISTICS, val);
    }

    // Fuzz AV1E_SET_COLOR_RANGE
    if (Size > 1) {
        int val = Data[1];
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_COLOR_RANGE, val);
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (Size > 2) {
        int val = Data[2];
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    }

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    if (Size > 3) {
        int val = Data[3];
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_CFL_INTRA, val);
    }

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    if (Size > 4) {
        int val = Data[4];
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_MODE_COST_UPD_FREQ, val);
    }

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    if (Size > 5) {
        int val = Data[5];
        aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    }

    // Attempt to encode a frame to exercise more state
    aom_image_t img;
    // Use fixed dimensions to avoid allocation issues with small inputs
    int w = 64;
    int h = 64;
    
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fill planes safely using their actual strides and dimensions
    // Y plane
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            img.planes[0][y * img.stride[0] + x] = Data[(y * w + x) % Size];
        }
    }
    // U plane (width/2, height/2)
    int uw = w >> 1;
    int uh = h >> 1;
    for (int y = 0; y < uh; y++) {
        for (int x = 0; x < uw; x++) {
            img.planes[1][y * img.stride[1] + x] = 128;
        }
    }
    // V plane (width/2, height/2)
    for (int y = 0; y < uh; y++) {
        for (int x = 0; x < uw; x++) {
            img.planes[2][y * img.stride[2] + x] = 128;
        }
    }

    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}