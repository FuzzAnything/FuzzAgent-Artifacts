// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_EXTERNAL_RATE_CONTROL__value at vp8cx.h:1097:1 in vp8cx.h
// VP9E_SET_RTC_EXTERNAL_RATECTRL__value at vp8cx.h:1099:1 in vp8cx.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// VP8E_SET_RTC_EXTERNAL_RATECTRL__value at vp8cx.h:1105:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

/*
 * The provided target API signatures were empty in the prompt.
 * Based on libvpx documentation and common usage patterns for these macros:
 * - VP8E_SET_CPUUSED: vpx_codec_control(ctx, VP8E_SET_CPUUSED, int)
 * - VP9E_SET_EXTERNAL_RATE_CONTROL: vpx_codec_control(ctx, VP9E_SET_EXTERNAL_RATE_CONTROL, vpx_rc_funcs_t *)
 * - VP8E_SET_FRAME_FLAGS: vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, uint32_t)
 * - VP9E_SET_RTC_EXTERNAL_RATECTRL: vpx_codec_control(ctx, VP9E_SET_RTC_EXTERNAL_RATECTRL, int)
 * - VP8E_SET_RTC_EXTERNAL_RATECTRL: vpx_codec_control(ctx, VP8E_SET_RTC_EXTERNAL_RATECTRL, int)
 * - VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR: vpx_codec_control(ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, int)
 *
 * This fuzz driver creates a VP9 encoder context (which is more robust for testing)
 * and attempts to invoke these controls using data derived from the fuzzer input.
 */

static vpx_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    // Attempt to get VP9 interface. If VP9 headers are missing, this might fail or we fallback to VP8.
    const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    // Fallback to VP8 if VP9 is not available
    if (!iface) {
        iface = vpx_codec_vp8_cx();
    }

    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    // Use fuzzer data to configure encoder settings safely
    cfg.g_w = (data[0] % 1024) + 16;
    cfg.g_h = (data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 60) + 1;
    cfg.rc_target_bitrate = (data[3] * 256 + data[4]) + 1;
    cfg.g_threads = (data[5] % 4) + 1;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    vpx_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Extract values from input for controls
    int cpu_used = (Data[0] % 9); // 0-8
    uint32_t frame_flags = 0;
    if (Size > 10) frame_flags = *(uint32_t *)(Data + 6);

    // 1. VP8E_SET_CPUUSED
    vpx_codec_control(ctx, VP8E_SET_CPUUSED, cpu_used);

    // 2. VP9E_SET_EXTERNAL_RATE_CONTROL
    static vpx_rc_funcs_t rc_funcs = {0};
    rc_funcs.rc_type = (vpx_rc_type_t)(Data[1] % 7); // 0 to 6 based on enum
    // Leave callbacks as NULL to test library robustness against incomplete setup
    rc_funcs.create_model = NULL;
    rc_funcs.send_firstpass_stats = NULL;
    rc_funcs.send_tpl_gop_stats = NULL;
    rc_funcs.get_encodeframe_decision = NULL;
    rc_funcs.update_encodeframe_result = NULL;
    rc_funcs.get_key_frame_decision = NULL;
    rc_funcs.get_gop_decision = NULL;
    rc_funcs.get_frame_rdmult = NULL;
    rc_funcs.delete_model = NULL;
    rc_funcs.rate_ctrl_log_path = "./dummy_file";
    rc_funcs.priv = NULL;

    vpx_codec_control(ctx, VP9E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);

    // 3. VP9E_SET_RTC_EXTERNAL_RATECTRL
    // Error fix: This control expects an 'int', not a pointer to vpx_rc_funcs_t.
    // We pass an integer derived from the fuzzer input.
    int rtc_external_rate_ctrl_val = (Data[2] % 2);
    vpx_codec_control(ctx, VP9E_SET_RTC_EXTERNAL_RATECTRL, rtc_external_rate_ctrl_val);

    // 4. VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR
    int disable_overshoot = (Data[3] % 2);
    vpx_codec_control(ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot);

    // 5. VP8E_SET_RTC_EXTERNAL_RATECTRL
    // Error fix: This control expects an 'int', not a pointer to vpx_rc_funcs_t.
    int rtc_external_rate_ctrl_val_8 = (Data[4] % 2);
    vpx_codec_control(ctx, VP8E_SET_RTC_EXTERNAL_RATECTRL, rtc_external_rate_ctrl_val_8);

    // 6. VP8E_SET_FRAME_FLAGS
    vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, frame_flags);

    // Cleanup
    cleanup_encoder_context(ctx);

    return 0;
}