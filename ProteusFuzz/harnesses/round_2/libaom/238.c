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

static int fuzz_control_function(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_238(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    // Extract parameters from input data
    int enable_auto_alt_ref = (Data[0] % 2);
    int max_ref_frames = (Data[1] % 8) + 1;
    int s_frame_mode = (Data[2] % 3);
    int noise_sensitivity = (Data[3] % 10);
    int gf_cbr_boost_pct = (Data[4] % 100) + 1;
    int enable_rate_guide_deltaq = (Data[5] % 2);

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 1920;
    cfg.g_h = 1080;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control functions
    fuzz_control_function(&ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    fuzz_control_function(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);
    fuzz_control_function(&ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);
    fuzz_control_function(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    fuzz_control_function(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
    fuzz_control_function(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, enable_rate_guide_deltaq);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}