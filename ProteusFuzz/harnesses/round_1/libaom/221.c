// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int setup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int ctrl_id, void *arg) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, ctrl_id, arg);
    if (ret != AOM_CODEC_OK) {
        return (int)ret;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret;

    if (setup_encoder(&ctx, &cfg) != 0) return 0;

    uint32_t param_val = 0;
    if (Size >= 4) {
        memcpy(&param_val, Data, 4);
    }

    // Fuzz AV1E_SET_QM_MAX
    int qm_max = (int)(param_val & 0xFFFF);
    ret = fuzz_encoder_control(&ctx, AV1E_SET_QM_MAX, &qm_max);
    (void)ret;

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    int coeff_cost_upd_freq = (int)((param_val >> 16) & 0xFFFF);
    ret = fuzz_encoder_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, &coeff_cost_upd_freq);
    (void)ret;

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    int scd_mode = (int)(param_val & 0x3);
    ret = fuzz_encoder_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, &scd_mode);
    (void)ret;

    // Fuzz AV1E_SET_S_FRAME_MODE
    int s_frame_mode = (int)((param_val >> 2) & 0x3);
    ret = fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, &s_frame_mode);
    (void)ret;

    // Fuzz AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC
    int rtc_value = 0;
    ret = fuzz_encoder_control(&ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &rtc_value);
    (void)ret;

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    int rtc_external_rc = (int)((param_val >> 4) & 0x1);
    ret = fuzz_encoder_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, &rtc_external_rc);
    (void)ret;

    cleanup_encoder(&ctx);
    return 0;
}