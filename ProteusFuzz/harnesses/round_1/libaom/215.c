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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 4) {
        return 0;
    }

    uint32_t value = *(const uint32_t *)data;
    int result = 0;

    // Test AV1E_SET_INTER_DCT_ONLY
    result = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, value);
    (void)result;

    // Test AV1E_SET_ENABLE_RECT_TX
    result = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, value);
    (void)result;

    // Test AV1E_SET_ENABLE_DIST_WTD_COMP
    result = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value);
    (void)result;

    // Test AV1E_SET_QUANT_B_ADAPT
    result = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, value);
    (void)result;

    // Test AV1E_SET_ENABLE_TX64
    result = aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, value);
    (void)result;

    // Test AV1E_SET_TUNE_CONTENT
    // TUNE_CONTENT expects an enum aom_tune_content_t, usually 0-2.
    // We mask the value to keep it within reasonable bounds for this specific control.
    uint32_t tune_val = value & 0x3;
    result = aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, tune_val);
    (void)result;

    return 0;
}

int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzz data to configure basic settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] % 100) + 100;

    // Initialize the codec
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}