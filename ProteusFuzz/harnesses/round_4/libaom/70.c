// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_undershoot_pct = 0;
    cfg->rc_overshoot_pct = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 255;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    int ret;

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Extract parameters from input data if possible, otherwise use defaults
    int param_val = 0;
    if (Size > 0) {
        param_val = Data[0];
    }

    // Fuzz AV1E_SET_QM_Y
    ret = aom_codec_control(&ctx, AV1E_SET_QM_Y, &param_val);
    (void)ret;

    // Fuzz AV1E_SET_QM_U
    ret = aom_codec_control(&ctx, AV1E_SET_QM_U, &param_val);
    (void)ret;

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = (Size > 1) ? Data[1] : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, &aq_mode);
    (void)ret;

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    int mode_cost_freq = (Size > 2) ? Data[2] : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, &mode_cost_freq);
    (void)ret;

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    int disable_trellis = (Size > 3) ? Data[3] : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, &disable_trellis);
    (void)ret;

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    int cdf_update_mode = (Size > 4) ? Data[4] : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, &cdf_update_mode);
    (void)ret;

    cleanup_codec(&ctx);
    return 0;
}