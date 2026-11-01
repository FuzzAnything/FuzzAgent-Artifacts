// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    int ret = aom_codec_control(ctx, control_id, value);
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) + 1;
    cfg.g_threads = (Data[6] % 8) + 1;
    cfg.g_error_resilient = (Data[7] % 2);
    cfg.g_lag_in_frames = (Data[8] % 256);

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int state_changes = 0;

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    int er_mode = (Data[9] % 3); // 0, 1, 2
    state_changes += fuzz_encoder_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, er_mode);

    // Fuzz AV1E_SET_S_FRAME_MODE
    int s_frame_mode = (Data[10] % 2); // 0 or 1
    state_changes += fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    int noise_sensitivity = (Data[11] % 100); // 0-99
    state_changes += fuzz_encoder_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
    int enable_deltaq = (Data[12] % 2); // 0 or 1
    state_changes += fuzz_encoder_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, enable_deltaq);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = (Data[13] % 6); // 0-5
    state_changes += fuzz_encoder_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    int deltaq_strength = (Data[14] % 100); // 0-99
    state_changes += fuzz_encoder_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}