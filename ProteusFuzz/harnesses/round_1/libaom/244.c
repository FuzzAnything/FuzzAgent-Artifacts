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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int ret = 0;
    int value = 0;

    // Attempt to extract a value from the fuzz input for the control
    if (size >= sizeof(int)) {
        memcpy(&value, data, sizeof(int));
    } else if (size > 0) {
        value = (int)data[0];
    }

    // AV1E_SET_INTER_DCT_ONLY
    ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, value);
    (void)ret;

    // AV1E_SET_ENABLE_DIST_WTD_COMP
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value);
    (void)ret;

    // AV1E_SET_ENABLE_RECT_TX
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, value);
    (void)ret;

    // AV1E_SET_QUANT_B_ADAPT
    ret = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, value);
    (void)ret;

    // AV1E_SET_TUNE_CONTENT
    // Cast to int as the specific enum type might not be directly exposed or named differently in some versions
    ret = aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, (int)value);
    (void)ret;

    // AV1E_SET_ALLOW_REF_FRAME_MVS
    ret = aom_codec_control(ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, value);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Initialize configuration
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic config values to ensure valid initialization
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize the codec context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}