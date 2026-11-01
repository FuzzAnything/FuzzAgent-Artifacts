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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Use the standard interface retrieval function for AV1 encoder
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = Data[0] & 0x7F;
    if (cfg.g_w == 0) cfg.g_w = 64;
    cfg.g_h = (Data[0] >> 1) & 0x7F;
    if (cfg.g_h == 0) cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 100;
    cfg.rc_overshoot_pct = 100;
    cfg.rc_max_quantizer = 63;
    cfg.rc_min_quantizer = 0;
    cfg.g_usage = AOM_USAGE_REALTIME;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_TIER_MASK
    if (Size > 0) {
        fuzz_encoder_control(&ctx, AV1E_SET_TIER_MASK, Data[0] & 0x01);
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    if (Size > 1) {
        fuzz_encoder_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, Data[1] & 0xFF);
    }

    // Fuzz AV1E_SET_MIN_CR
    if (Size > 2) {
        fuzz_encoder_control(&ctx, AV1E_SET_MIN_CR, Data[2] & 0xFF);
    }

    // Fuzz AV1E_SET_FP_MT
    if (Size > 3) {
        fuzz_encoder_control(&ctx, AV1E_SET_FP_MT, Data[3] & 0x01);
    }

    // Fuzz AV1E_SET_MV_COST_UPD_FREQ
    if (Size > 4) {
        fuzz_encoder_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, Data[4] & 0xFF);
    }

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    if (Size > 5) {
        fuzz_encoder_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, Data[5] & 0x01);
    }

    aom_codec_destroy(&ctx);
    return 0;
}