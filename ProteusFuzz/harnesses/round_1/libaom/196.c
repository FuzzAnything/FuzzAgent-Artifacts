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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int32_t value;
    if (size >= sizeof(int32_t)) {
        memcpy(&value, data, sizeof(int32_t));
    } else {
        value = (int32_t)data[0];
    }

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, value) != AOM_CODEC_OK) {
        // Ignore errors during fuzzing
    }

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    if (aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, value) != AOM_CODEC_OK) {
        // Ignore errors during fuzzing
    }

    // Fuzz AV1E_SET_TIMING_INFO_TYPE
    if (aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, value) != AOM_CODEC_OK) {
        // Ignore errors during fuzzing
    }

    // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
    if (aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, value) != AOM_CODEC_OK) {
        // Ignore errors during fuzzing
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    if (aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, value) != AOM_CODEC_OK) {
        // Ignore errors during fuzzing
    }

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, value) != AOM_CODEC_OK) {
        // Ignore errors during fuzzing
    }

    return 0;
}

int LLVMFuzzerTestOneInput_196(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    // Initialize configuration
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some config parameters
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[3] % 100) + 1;
    cfg.g_timebase.den = (Data[4] % 100) + 1;
    
    // Set bitrate using the correct field name rc_target_bitrate
    int32_t bitrate = (Data[2] % 100) + 100;
    cfg.rc_target_bitrate = bitrate;

    // Initialize encoder
    res = aom_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control functions
    fuzz_encoder_control(&ctx, Data + 5, Size - 5);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}