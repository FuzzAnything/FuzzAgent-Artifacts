// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // We need at least 1 byte to pick a random value for the control
    if (size < 1) return 0;

    int32_t value = (int32_t)data[0];
    aom_codec_err_t ret;

    // 1. AV1E_SET_QUANT_B_ADAPT
    ret = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, value);
    (void)ret;

    // 2. AOME_SET_CPUUSED
    // Clamp value to valid range [0, 8] typically, but fuzzing allows out of range to test error handling
    ret = aom_codec_control(ctx, AOME_SET_CPUUSED, value);
    (void)ret;

    // 3. AV1E_SET_DISABLE_TRELLIS_QUANT
    ret = aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, value);
    (void)ret;

    // 4. AV1E_SET_BITRATE_ONE_PASS_CBR
    ret = aom_codec_control(ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, value);
    (void)ret;

    // 5. AOME_SET_MAX_INTER_BITRATE_PCT
    // Note: If this macro is not defined in the specific libaom version, 
    // it might be defined as AOME_SET_MAX_INTER_BITRATE_PCT or similar.
    // Checking for existence or using a fallback if needed. 
    // Assuming standard libaom defines.
    #ifdef AOME_SET_MAX_INTER_BITRATE_PCT
    ret = aom_codec_control(ctx, AOME_SET_MAX_INTER_BITRATE_PCT, value);
    (void)ret;
    #endif

    // 6. AOME_SET_MAX_INTRA_BITRATE_PCT
    #ifdef AOME_SET_MAX_INTRA_BITRATE_PCT
    ret = aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, value);
    (void)ret;
    #endif

    return 0;
}

int LLVMFuzzerTestOneInput_230(const uint8_t *Data, size_t Size) {
    // Need at least 7 bytes to safely initialize config parameters
    if (Size < 7) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize configuration with default values
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz some config parameters based on input data
    // Ensure width and height are multiples of 2 (or 16 for AV1) to avoid init errors
    cfg.g_w = ((Data[0] % 512) + 16); 
    if (cfg.g_w % 16 != 0) cfg.g_w += (16 - cfg.g_w % 16);
    
    cfg.g_h = ((Data[1] % 512) + 16);
    if (cfg.g_h % 16 != 0) cfg.g_h += (16 - cfg.g_h % 16);

    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    
    // g_bitrate is not a direct member, use rc_target_bitrate
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    cfg.g_error_resilient = (Data[5] % 3);
    cfg.g_lag_in_frames = (Data[6] % 25);

    // Initialize encoder context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzzing on the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}