// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encode_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

static int fuzz_encode_control_get(aom_codec_ctx_t *ctx, int control_id, int *out_value) {
    if (ctx == NULL || out_value == NULL) return 0;
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, out_value);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default("av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = Data[2] % 100 + 1;
    cfg.g_timebase.den = Data[3] % 100 + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.g_profile = Data[5] % 4;
    cfg.rc_target_bitrate = (Data[6] * 256 + Data[7]) * 1000;
    cfg.g_error_resilient = Data[8] % 3;
    cfg.g_lag_in_frames = Data[9] % 100;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int enable_filter_intra = Data[10] % 2;
    int screen_content_detection_mode = Data[11] % 3;
    int rtc_external_rc = Data[12] % 2;
    int enable_palette = Data[13] % 2;
    int qm_max = Data[14] % 100;
    int high_motion_content_screen_rtc;

    fuzz_encode_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra);
    fuzz_encode_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, screen_content_detection_mode);
    fuzz_encode_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_external_rc);
    fuzz_encode_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);
    fuzz_encode_control(&ctx, AV1E_SET_QM_MAX, qm_max);
    fuzz_encode_control_get(&ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &high_motion_content_screen_rtc);

    aom_codec_destroy(&ctx);

    return 0;
}