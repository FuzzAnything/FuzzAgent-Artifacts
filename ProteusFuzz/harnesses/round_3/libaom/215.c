// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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
    if (ctx == NULL) return 0;
    
    // Attempt to set the control with the provided value
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    
    // If the control failed, try with a range of values to explore more states
    if (res != AOM_CODEC_OK) {
        for (int i = 0; i < 5; i++) {
            int test_val = value + i;
            res = aom_codec_control(ctx, control_id, test_val);
            if (res == AOM_CODEC_OK) break;
        }
    }
    
    return (res == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices up to 8
    if (Size < 9) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure some parameters with bounds checking
    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = Data[2] ? Data[2] : 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input data to test different control parameters
    int min_gf_interval = (int)Data[3];
    int max_gf_interval = (int)Data[4];
    int disable_trellis_quant = Data[5] % 2;
    int error_resilient_mode = Data[6] % 2;
    int gf_max_pyramid_height = (int)Data[7];
    int gf_min_pyramid_height = (int)Data[8];

    // Test various control functions using correct constants
    test_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, min_gf_interval);
    test_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, max_gf_interval);
    test_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis_quant);
    test_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);
    test_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, gf_max_pyramid_height);
    test_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, gf_min_pyramid_height);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}