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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16; // Ensure width is at least 16
    cfg.g_h = (Data[1] % 256) + 16; // Ensure height is at least 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input data to fuzz different control parameters
    int noise_sensitivity = Data[2] % 100;
    int aq_mode = Data[3] % 5;
    int qm_max = Data[4] % 256;
    int chroma_subsampling_y = Data[5] % 3;
    int gf_cbr_boost_pct = Data[6] % 100;
    int s_frame_mode = Data[7] % 4;

    // Invoke the target control functions using correct enum names
    fuzz_encoder_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    fuzz_encoder_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);
    fuzz_encoder_control(&ctx, AV1E_SET_QM_MAX, qm_max);
    fuzz_encoder_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, chroma_subsampling_y);
    fuzz_encoder_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
    fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}