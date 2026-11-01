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

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    aom_codec_err_t ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_ctx_t *ctx = init_encoder(iface, &cfg);
    if (!ctx) return 0;

    // Fuzz control parameters based on input data
    int32_t qm_max = Data[0];
    int32_t noise_sensitivity = Data[1 % Size];
    int32_t screen_content_mode = Data[2 % Size];
    int32_t s_frame_mode = Data[3 % Size];
    int32_t rtc_external_rc = Data[4 % Size];

    // Invoke target API functions with fuzzed values using correct enum names
    aom_codec_control(ctx, AV1E_SET_QM_MAX, qm_max);
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, screen_content_mode);
    aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);
    aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_external_rc);

    // Attempt to get high motion content screen RTC (if supported)
    int32_t high_motion_content_screen_rtc = 0;
    aom_codec_control(ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &high_motion_content_screen_rtc);

    cleanup_encoder(ctx);
    return 0;
}