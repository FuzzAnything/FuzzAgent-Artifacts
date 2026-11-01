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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return -1;

    // Randomly select and invoke control functions
    // AV1E_SET_S_FRAME_MODE
    int s_frame_mode = (data[0] % 3);
    if (aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode) != AOM_CODEC_OK) {
        // Ignore errors as they are expected during fuzzing
    }

    // AV1E_SET_MAX_REFERENCE_FRAMES
    int max_ref_frames = (data[1] % 8) + 1;
    if (aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_REF_FRAME_MVS
    int enable_ref_frame_mvs = (data[2] % 2);
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    int scd_mode = (data[3] % 3);
    if (aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, scd_mode) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC
    int high_motion_screen_rtc;
    if (aom_codec_control(ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &high_motion_screen_rtc) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_RTC_EXTERNAL_RC
    int rtc_external_rc = (data[4] % 2);
    if (aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_external_rc) != AOM_CODEC_OK) {
        // Ignore errors
    }

    return 0;
}

int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder configuration with fuzzed data
    cfg.g_w = (Data[5] % 1920) + 16;
    cfg.g_h = (Data[6] % 1080) + 16;
    cfg.g_timebase.num = (Data[7] % 60) + 1;
    cfg.g_timebase.den = (Data[8] % 1000) + 1;
    cfg.g_threads = (Data[9] % 4) + 1;
    cfg.g_error_resilient = (Data[10] % 2);
    cfg.rc_target_bitrate = (Data[11] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing operations
    fuzz_encoder(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}