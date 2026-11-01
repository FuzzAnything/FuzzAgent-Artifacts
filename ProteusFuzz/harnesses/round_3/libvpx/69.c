// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/*
 * Fuzz driver for libvpx VP8 encoder control APIs.
 * Targets: VP8E_SET_ARNR_TYPE, VP8E_SET_ARNR_STRENGTH, VP8E_SET_CPUUSED,
 *          VP8E_SET_NOISE_SENSITIVITY, VP8E_SET_ARNR_MAXFRAMES via vpx_codec_control_.
 */

static int fuzz_init_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    vpx_codec_err_t err;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) {
        return 0;
    }

    // Initialize config with defaults
    err = vpx_codec_enc_config_default(iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz some configuration parameters based on input data
    if (size > 0) cfg->g_w = (data[0] % 16 + 1) * 16;
    if (size > 1) cfg->g_h = (data[1] % 16 + 1) * 16;
    if (size > 2) cfg->g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg->g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg->g_error_resilient = data[4] % 3;
    if (size > 5) cfg->rc_target_bitrate = (data[5] % 100) + 1;

    // Initialize the encoder context
    err = vpx_codec_enc_init(ctx, iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    return 1;
}

static int fuzz_control_api(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_err_t err;
    int ctrl_id;
    int value;

    // Select a control ID based on input
    if (size > 0) {
        ctrl_id = data[0] % 6;
    } else {
        ctrl_id = 0;
    }

    // Map ctrl_id to actual VP8E_* control constants
    switch (ctrl_id) {
        case 0: ctrl_id = VP8E_SET_ARNR_TYPE; break;
        case 1: ctrl_id = VP8E_SET_ARNR_STRENGTH; break;
        case 2: ctrl_id = VP8E_SET_CPUUSED; break;
        case 3: ctrl_id = VP8E_SET_NOISE_SENSITIVITY; break;
        case 4: ctrl_id = VP8E_SET_ARNR_MAXFRAMES; break;
        case 5: ctrl_id = VP8E_SET_ARNR_TYPE; break; // Fallback
        default: ctrl_id = VP8E_SET_CPUUSED; break;
    }

    // Generate a value based on input data
    if (size > 1) {
        value = data[1] % 10; // Limit value range for safety
    } else {
        value = 0;
    }

    // Invoke the control API directly using vpx_codec_control_
    err = vpx_codec_control_(ctx, ctrl_id, value);

    // Return success/failure status (fuzzer doesn't care about the return value much, but we track it)
    return (err == VPX_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int initialized = 0;

    // Clear context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder
    initialized = fuzz_init_encoder(&ctx, &cfg, Data, Size);
    if (!initialized) {
        return 0;
    }

    // Fuzz control APIs
    fuzz_control_api(&ctx, Data, Size);

    // Cleanup: destroy the encoder context
    vpx_codec_destroy(&ctx);

    return 0;
}