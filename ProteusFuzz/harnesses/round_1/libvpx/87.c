// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP8E_SET_RTC_EXTERNAL_RATECTRL__value at vp8cx.h:1105:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_EXTERNAL_RATE_CONTROL__value at vp8cx.h:1097:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx/vpx_encoder.h"

// Dummy callback functions matching expected signatures
static enum vpx_rc_status dummy_create_model(void *priv, const struct vpx_rc_config *cfg, void **model) {
    (void)priv; (void)cfg; (void)model;
    return VPX_RC_OK;
}

static enum vpx_rc_status dummy_send_firstpass_stats(void *priv, const struct vpx_rc_firstpass_stats *stats) {
    (void)priv; (void)stats;
    return VPX_RC_OK;
}

static enum vpx_rc_status dummy_send_tpl_gop_stats(void *priv, const struct VpxTplGopStats *stats) {
    (void)priv; (void)stats;
    return VPX_RC_OK;
}

// Fixed signature: added 'const int' parameter
static enum vpx_rc_status dummy_get_encodeframe_decision(void *priv, const int frame_idx, struct vpx_rc_encodeframe_decision *decision) {
    (void)priv; (void)frame_idx; (void)decision;
    return VPX_RC_OK;
}

static enum vpx_rc_status dummy_update_encodeframe_result(void *priv, const struct vpx_rc_encodeframe_result *result) {
    (void)priv; (void)result;
    return VPX_RC_OK;
}

// Fixed signature: struct name corrected to vpx_rc_key_frame_decision
static enum vpx_rc_status dummy_get_key_frame_decision(void *priv, struct vpx_rc_key_frame_decision *decision) {
    (void)priv; (void)decision;
    return VPX_RC_OK;
}

static enum vpx_rc_status dummy_get_gop_decision(void *priv, struct vpx_rc_gop_decision *decision) {
    (void)priv; (void)decision;
    return VPX_RC_OK;
}

// Fixed signature: matches vpx_rc_get_frame_rdmult_cb_fn_t
static enum vpx_rc_status dummy_get_frame_rdmult(void *priv, const struct vpx_rc_encodeframe_info *info, int *rdmult) {
    (void)priv; (void)info; (void)rdmult;
    return VPX_RC_OK;
}

// Fixed signature: returns enum vpx_rc_status
static enum vpx_rc_status dummy_delete_model(void *priv) {
    (void)priv;
    return VPX_RC_OK;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    // Initialize configuration
    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) return 0;
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    // Initialize context
    if (vpx_codec_enc_init(&ctx, NULL, &cfg, 0) != 0) return 0;

    // Prepare external rate control functions structure
    vpx_rc_funcs_t rc_funcs = {0};
    rc_funcs.rc_type = (vpx_rc_type_t)(Data[0] & 0x07);
    rc_funcs.create_model = dummy_create_model;
    rc_funcs.send_firstpass_stats = dummy_send_firstpass_stats;
    rc_funcs.send_tpl_gop_stats = dummy_send_tpl_gop_stats;
    rc_funcs.get_encodeframe_decision = dummy_get_encodeframe_decision;
    rc_funcs.update_encodeframe_result = dummy_update_encodeframe_result;
    rc_funcs.get_key_frame_decision = dummy_get_key_frame_decision;
    rc_funcs.get_gop_decision = dummy_get_gop_decision;
    rc_funcs.get_frame_rdmult = dummy_get_frame_rdmult;
    rc_funcs.delete_model = dummy_delete_model;
    rc_funcs.rate_ctrl_log_path = "./dummy_file";
    rc_funcs.priv = NULL;

    // Fuzz VP9E_SET_EXTERNAL_RATE_CONTROL
    ret = vpx_codec_control(&ctx, VP9E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
    (void)ret;

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    int quantizer = (Data[0] << 8) | (Size > 1 ? Data[1] : 0);
    quantizer &= 0xFF;
    ret = vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, quantizer);
    (void)ret;

    // Fuzz VP9E_SET_ALT_REF_AQ
    int alt_ref_aq = (Size > 2 ? Data[2] : 0) & 0x01;
    ret = vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, alt_ref_aq);
    (void)ret;

    // Fuzz VP9E_SET_COLOR_RANGE
    int color_range = (Size > 3 ? Data[3] : 0) & 0x01;
    ret = vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, color_range);
    (void)ret;

    // Fuzz VP8E_SET_RTC_EXTERNAL_RATECTRL
    // This control expects an int (0 or 1), not a struct pointer.
    int rtc_external = (Size > 4 ? Data[4] : 0) & 0x01;
    ret = vpx_codec_control(&ctx, VP8E_SET_RTC_EXTERNAL_RATECTRL, rtc_external);
    (void)ret;

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}