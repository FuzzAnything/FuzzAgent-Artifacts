// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure basic encoder settings
    cfg.g_w = (Data[0] % 2048) + 16;
    cfg.g_h = (Data[1] % 2048) + 16;
    cfg.g_timebase.num = (Data[2] % 60) + 1;
    cfg.g_timebase.den = (Data[3] % 60) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    cfg.g_error_resilient = 0;

    aom_codec_ctx_t *ctx = init_encoder(iface, &cfg);
    if (!ctx) return 0;

    // Extract values from fuzz data to use as arguments for control functions
    int32_t val1 = *(int32_t *)(Data + 4);
    int32_t val2 = *(int32_t *)(Data + 8);
    int32_t val3 = *(int32_t *)(Data + 12);
    int32_t val4 = *(int32_t *)(Data + 16);
    int32_t val5 = *(int32_t *)(Data + 20);
    int32_t val6 = *(int32_t *)(Data + 24);

    // Invoke the target control functions using the correct enum names from aomcx.h
    // 1. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val1);
    
    // 2. AV1E_SET_FILM_GRAIN_TEST_VECTOR
    aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val2);
    
    // 3. AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
    aom_codec_control(ctx, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, val3);
    
    // 4. AV1E_SET_SVC_FRAME_DROP_MODE
    aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val4);
    
    // 5. AV1E_SET_FP_MT_UNIT_TEST
    aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, val5);
    
    // 6. AV1E_ENABLE_SB_QP_SWEEP
    aom_codec_control(ctx, AV1E_ENABLE_SB_QP_SWEEP, val6);

    // Perform a dummy encode to exercise the state
    aom_codec_iface_t *iface_enc = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg_enc;
    aom_codec_enc_config_default(iface_enc, &cfg_enc, 0);
    cfg_enc.g_w = 64;
    cfg_enc.g_h = 64;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;
    
    aom_codec_ctx_t *ctx_enc = init_encoder(iface_enc, &cfg_enc);
    if (ctx_enc) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
        memset(img.planes[0], 128, 64*64);
        memset(img.planes[1], 128, 32*32);
        memset(img.planes[2], 128, 32*32);
        
        aom_codec_encode(ctx_enc, &img, 0, 1, 0);
        aom_img_free(&img);
        cleanup_encoder(ctx_enc);
    }

    cleanup_encoder(ctx);
    return 0;
}