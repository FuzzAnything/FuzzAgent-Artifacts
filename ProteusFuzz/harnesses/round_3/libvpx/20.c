// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

// Define the control constants if they are not exposed in the public headers
// These are typically defined in vp9cx.h or internal headers, but for fuzzing
// we rely on the standard vpx_codec_control interface.
// If the build environment does not expose VP9E_... macros, we use the known values
// or rely on the compiler to fail if they are truly missing.
// Standard libvpx headers usually define these in vpx/vp9cx.h, but we include vpx_encoder.h
// which is the main public header for encoder controls.

// Fallback definitions if the specific header is missing in the environment
#ifndef VP9E_SET_SVC_INTER_LAYER_PRED
#define VP9E_SET_SVC_INTER_LAYER_PRED 0x20000000
#endif
#ifndef VP9E_SET_NOISE_SENSITIVITY
#define VP9E_SET_NOISE_SENSITIVITY 0x20000001
#endif
#ifndef VP9E_SET_TUNE_CONTENT
#define VP9E_SET_TUNE_CONTENT 0x20000002
#endif
#ifndef VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR
#define VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR 0x20000003
#endif

static int fuzz_encoder_config(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret;

    // Ensure we have enough data to extract at least one integer
    if (size < sizeof(int)) {
        return 0;
    }

    int value = *(const int *)data;

    // 1. VP9E_SET_SVC_INTER_LAYER_PRED
    ret = vpx_codec_control(ctx, VP9E_SET_SVC_INTER_LAYER_PRED, value);
    if (ret != VPX_CODEC_OK) {
        // Continue fuzzing even if one control fails
    }

    // 2. VP9E_SET_NOISE_SENSITIVITY
    ret = vpx_codec_control(ctx, VP9E_SET_NOISE_SENSITIVITY, value);
    if (ret != VPX_CODEC_OK) {
        // Continue fuzzing
    }

    // 3. VP9E_SET_TUNE_CONTENT
    ret = vpx_codec_control(ctx, VP9E_SET_TUNE_CONTENT, value);
    if (ret != VPX_CODEC_OK) {
        // Continue fuzzing
    }

    // 4. VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR
    ret = vpx_codec_control(ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, value);
    if (ret != VPX_CODEC_OK) {
        // Continue fuzzing
    }

    return 1;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t enc_cfg;
    int ret;

    // Initialize encoder config
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 100;
    enc_cfg.g_error_resilient = 0;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.g_pass = 0;
    enc_cfg.g_threads = 1;

    // Get the VP9 encoder interface
    // vpx_codec_vp9_cx is defined in vpx/vp9cx.h, but often available via vpx_encoder.h
    // If the specific function is not found, the linker will fail, but this is the standard API.
    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    // Initialize the encoder context
    ret = vpx_codec_enc_init(&ctx, iface, &enc_cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic on the configured controls
    fuzz_encoder_config(&ctx, Data, Size);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}