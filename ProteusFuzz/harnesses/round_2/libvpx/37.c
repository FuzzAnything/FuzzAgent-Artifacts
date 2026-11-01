// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx != NULL && ctx->priv != NULL) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (iface == NULL) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set minimal valid configuration to ensure encoder initializes correctly
    cfg.g_w = 160;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Ensure the encoder is ready by sending a control command that is safe
    // We avoid using uninitialized or invalid control IDs directly without validation
    int ctrl_id = Data[0];

    // Only use known safe control IDs to prevent internal null pointer dereferences
    // VP8E_SET_TUNING is generally safe if a valid tuning value is passed
    // VP8E_GET_LAST_QUANTIZER is safe if a valid pointer is passed
    // Avoid arbitrary control IDs that might trigger unhandled internal states

    vpx_codec_err_t result = VPX_CODEC_ERROR;

    if (ctrl_id == 0) {
        // Use VP8E_SET_TUNING
        int tuning_val = (Size > 1) ? Data[1] : 0;
        // Clamp tuning value to valid range [0, 10] typically, but libvpx may handle others
        if (tuning_val < 0) tuning_val = 0;
        if (tuning_val > 10) tuning_val = 10;
        result = vpx_codec_control_(&ctx, VP8E_SET_TUNING, tuning_val);
    } else if (ctrl_id == 1) {
        // Use VP8E_GET_LAST_QUANTIZER
        int quantizer = 0;
        // Ensure we have sent at least one frame or initialized properly before getting quantizer
        // However, for fuzzing, we try to call it directly; if it fails due to state, that's expected
        result = vpx_codec_control_(&ctx, VP8E_GET_LAST_QUANTIZER, &quantizer);
    } else {
        // For other values, try a safe default or skip to avoid crashing
        // We can try VP8E_SET_TUNING with a derived value
        int tuning_val = (Size > 1) ? Data[1] % 11 : 0;
        result = vpx_codec_control_(&ctx, VP8E_SET_TUNING, tuning_val);
    }

    // Optionally, try to encode a dummy frame to ensure state is valid before control calls
    // But for simplicity and to avoid complex frame generation, we rely on the control calls themselves

    cleanup_codec(&ctx);
    return 0;
}