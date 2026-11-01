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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int test_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_262(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    int val = 0;
    if (Size > 2) {
        val = Data[2];
    }

    // Test AV1E_SET_ENABLE_ONESIDED_COMP
    test_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);

    // Test AV1E_SET_ENABLE_SMOOTH_INTRA
    test_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);

    // Test AV1E_SET_ENABLE_DUAL_FILTER
    test_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, val);

    // Test AV1E_SET_POSTENCODE_DROP_RTC
    test_codec_control(&ctx, AV1E_SET_POSTENCODE_DROP_RTC, val);

    // Test AV1E_SET_TUNE_CONTENT
    // This might expect an enum, try val and val+1
    test_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val);
    test_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val + 1);

    // Test AV1E_SET_ENABLE_FILTER_INTRA
    test_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);

    aom_codec_destroy(&ctx);

    return 0;
}