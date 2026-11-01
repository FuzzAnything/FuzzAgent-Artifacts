// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE at aomcx.h:1957:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_QM at aomcx.h:2032:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY at aomcx.h:1969:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_AQ_MODE at aomcx.h:1963:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH at aomcx.h:2294:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_ENABLE_RATE_GUIDE_DELTAQ at aomcx.h:2330:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices used below
    if (Size < 13) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Initialize configuration from input data to explore diverse states
    // Bounds checking is implicit due to the Size check at the start
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.rc_target_bitrate = ((uint32_t)Data[5] * 256 + Data[6]) * 1000;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    int mode = Data[7] % 2;
    ret = aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, mode);
    (void)ret; 

    // Fuzz AV1E_SET_ENABLE_QM
    unsigned int enable = Data[8] % 2;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_QM(&ctx, AV1E_SET_ENABLE_QM, enable);
    (void)ret;

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    unsigned int sensitivity = Data[9] % 256;
    ret = aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY(&ctx, AV1E_SET_NOISE_SENSITIVITY, sensitivity);
    (void)ret;

    // Fuzz AV1E_SET_AQ_MODE
    unsigned int aq_mode = Data[10] % 4; 
    ret = aom_codec_control_typechecked_AV1E_SET_AQ_MODE(&ctx, AV1E_SET_AQ_MODE, aq_mode);
    (void)ret;

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    unsigned int strength = Data[11] % 100;
    ret = aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH(&ctx, AV1E_SET_DELTAQ_STRENGTH, strength);
    (void)ret;

    // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
    unsigned int enable_rate = Data[12] % 2;
    ret = aom_codec_control_typechecked_AV1E_ENABLE_RATE_GUIDE_DELTAQ(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, enable_rate);
    (void)ret;

    cleanup_codec(&ctx);
    return 0;
}