// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int ret = 0;
    int value;

    // Helper to safely extract an int from data
    if (size >= sizeof(int)) {
        memcpy(&value, data, sizeof(int));
    } else {
        value = size; // Fallback if data is too short
    }

    // Map the value to 0 or 1 for boolean-like settings, or keep as is for others
    int flag_val = (value != 0) ? 1 : 0;
    int raw_val = value;

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, flag_val);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, raw_val);
    (void)ret;

    // AV1E_SET_ENABLE_RECT_TX
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, flag_val);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, raw_val);
    (void)ret;

    // AV1E_SET_DISABLE_TRELLIS_QUANT
    ret = aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, flag_val);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, raw_val);
    (void)ret;

    // AV1E_SET_ALLOW_WARPED_MOTION
    ret = aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, flag_val);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, raw_val);
    (void)ret;

    // AV1E_SET_ENABLE_TX64
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, flag_val);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, raw_val);
    (void)ret;

    // AV1E_SET_ROW_MT
    ret = aom_codec_control(ctx, AV1E_SET_ROW_MT, flag_val);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_ROW_MT, raw_val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_271(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Initialize encoder config
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic config to ensure valid initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize the encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Perform fuzzing on control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}