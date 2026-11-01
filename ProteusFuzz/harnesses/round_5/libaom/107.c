// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Use AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration values based on input data to explore states
    if (size >= 1) cfg->g_w = data[0] % 1920 + 16;
    if (size >= 2) cfg->g_h = data[1] % 1080 + 16;
    if (size >= 3) cfg->g_timebase.num = data[2] % 25 + 1;
    if (size >= 4) cfg->g_timebase.den = data[3] % 1000 + 1;
    if (size >= 5) cfg->g_threads = data[4] % 8 + 1;
    if (size >= 6) cfg->rc_target_bitrate = (data[5] * 256 + data[6]) * 1000;
    if (size >= 7) cfg->rc_min_quantizer = data[7] % 56;
    if (size >= 8) cfg->rc_max_quantizer = data[8] % 56 + cfg->rc_min_quantizer;

    // Initialize codec context
    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int initialized = 0;

    if (Size == 0) {
        return 0;
    }

    // Initialize encoder context
    initialized = init_encoder_context(&ctx, &cfg, Data, Size);
    if (!initialized) {
        return 0;
    }

    // Fuzz AOME_SET_CQ_LEVEL
    if (Size >= 1) {
        int cq_level = Data[0] % 63;
        res = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);
        (void)res; // Suppress unused variable warning
    }

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    if (Size >= 2) {
        int force_video_mode = (Data[1] % 2) ? 1 : 0;
        res = aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);
        (void)res;
    }

    // Fuzz AOME_SET_ENABLEAUTOALTREF
    if (Size >= 3) {
        int enable_auto_alt_ref = (Data[2] % 2) ? 1 : 0;
        res = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
        (void)res;
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    if (Size >= 4) {
        int coeff_cost_upd_freq = Data[3] % 10;
        res = aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);
        (void)res;
    }

    // Fuzz AV1E_SET_DELTAQ_MODE
    if (Size >= 5) {
        int delta_q_mode = Data[4] % 3;
        res = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, delta_q_mode);
        (void)res;
    }

    // Fuzz AOME_SET_ENABLEAUTOBWDREF
    if (Size >= 6) {
        int enable_auto_bwd_ref = (Data[5] % 2) ? 1 : 0;
        res = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, enable_auto_bwd_ref);
        (void)res;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}