// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

// Helper to safely create an encoder context
static aom_codec_ctx_t *create_encoder_context(void) {
    aom_codec_ctx_t *ctx = NULL;
    const aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set minimal config
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to destroy encoder context
static void destroy_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

// Helper to create a dummy image frame
static aom_image_t *create_dummy_image(int width, int height) {
    aom_image_t *img = malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
        free(img);
        return NULL;
    }
    return img;
}

// Helper to destroy image
static void destroy_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_243(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = create_encoder_context();
    if (!ctx) return 0;

    aom_image_t *img = create_dummy_image(128, 128);
    if (!img) {
        destroy_encoder_context(ctx);
        return 0;
    }

    // Initialize a frame to send to ensure encoder state
    aom_codec_frame_flags_t flags = 0;
    int64_t ts = 0;
    aom_codec_err_t res;

    res = aom_codec_encode(ctx, img, ts, 1, flags);
    (void)res;

    // Extract values from fuzz input to drive the control functions
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    int val4 = 0;
    int val5 = 0;
    int val6 = 0;

    if (Size >= 1) val1 = Data[0];
    if (Size >= 2) val2 = Data[1];
    if (Size >= 3) val3 = Data[2];
    if (Size >= 4) val4 = Data[3];
    if (Size >= 5) val5 = Data[4];
    if (Size >= 6) val6 = Data[5];

    // 1. AV1E_SET_DELTAQ_MODE
    res = aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, val1);
    (void)res;

    // 2. AV1E_SET_TIER_MASK
    res = aom_codec_control(ctx, AV1E_SET_TIER_MASK, val2);
    (void)res;

    // 3. AOME_SET_ENABLEAUTOBWDREF
    res = aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, val3);
    (void)res;

    // 4. AV1E_SET_COEFF_COST_UPD_FREQ
    res = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val4);
    (void)res;

    // 5. AV1E_SET_FORCE_VIDEO_MODE
    res = aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, val5);
    (void)res;

    // 6. AV1E_SET_TIMING_INFO_TYPE
    res = aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, val6);
    (void)res;

    // Cleanup
    destroy_image(img);
    destroy_encoder_context(ctx);

    return 0;
}