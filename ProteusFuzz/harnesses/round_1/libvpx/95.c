// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_GET_LAST_QUANTIZER__value at vp8cx.h:996:1 in vp8cx.h
// VP8E_GET_LAST_QUANTIZER_64__value at vp8cx.h:998:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t g_ctx;

static void cleanup_codec(void) {
    if (g_ctx.iface != NULL) {
        vpx_codec_destroy(&g_ctx);
    }
    memset(&g_ctx, 0, sizeof(g_ctx));
}

static int init_codec(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    memset(&g_ctx, 0, sizeof(g_ctx));

    // Attempt to initialize with VP8 encoder
    iface = vpx_codec_vp8_cx();
    if (iface == NULL) {
        // Fallback to VP9 if VP8 is not available
        iface = vpx_codec_vp9_cx();
        if (iface == NULL) {
            return 0;
        }
    }

    // vpx_codec_enc_config_default requires: iface, cfg, usage
    // Usage: VPX_CX_DEFAULT (0), VPX_CX_PASS (1), VPX_CX_REALTIME (2)
    // We use 0 (VPX_CX_DEFAULT)
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure parameters slightly to explore states
    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = (data[1] % 512) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
        cfg.g_lag_in_frames = (data[5] % 25);
        // rc_end_usage: VPX_CBR (0), VPX_VBR (1), VPX_Q (2)
        cfg.rc_end_usage = (int)(data[6] % 3);
        cfg.g_error_resilient = (data[7] % 3);
    }

    res = vpx_codec_enc_init(&g_ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Clean up any previous state
    cleanup_codec();

    // Initialize the codec
    if (!init_codec(Data, Size)) {
        return 0;
    }

    // Extract control ID and parameters from input
    int ctrl_id = Data[0];
    int param1 = (int)Data[1];
    int param2 = (int)Data[2];
    int param3 = (int)Data[3];
    int param4 = (int)Data[4];
    int param5 = (int)Data[5];
    int param6 = (int)Data[6];
    int param7 = (int)Data[7];

    // Map some common control IDs to explore different states
    // We only call specific, safe control IDs that are known to be supported by the encoder context.
    // We avoid calling vpx_codec_control_ with random IDs and varargs to prevent invalid pointer dereferences.
    
    switch (ctrl_id) {
        case VP8E_SET_TUNING:
            // VP8E_SET_TUNING takes an int (tuning index)
            vpx_codec_control(&g_ctx, VP8E_SET_TUNING, param1);
            break;
        case VP8E_GET_LAST_QUANTIZER:
            // VP8E_GET_LAST_QUANTIZER takes a pointer to int
            {
                int quant;
                vpx_codec_control(&g_ctx, VP8E_GET_LAST_QUANTIZER, &quant);
            }
            break;
        case VP8E_GET_LAST_QUANTIZER_64:
            // VP8E_GET_LAST_QUANTIZER_64 takes a pointer to int64_t
            {
                int64_t quant64;
                vpx_codec_control(&g_ctx, VP8E_GET_LAST_QUANTIZER_64, &quant64);
            }
            break;
        case VP9E_SET_TUNE_CONTENT:
            // VP9E_SET_TUNE_CONTENT takes an int (content type)
            vpx_codec_control(&g_ctx, VP9E_SET_TUNE_CONTENT, param1);
            break;
        // Note: VPXD_GET_LAST_QUANTIZER is for decoders, skipping to avoid context mismatch.
        default:
            // For unknown IDs, we do not call the function with random varargs.
            // Instead, we can try a known safe call or simply return to avoid crashes.
            // Attempting to call with a known safe ID like VP8E_SET_TUNING with a random param
            // to exercise the control path without invalid pointers.
            vpx_codec_control(&g_ctx, VP8E_SET_TUNING, param1);
            break;
    }

    // Cleanup
    cleanup_codec();

    return 0;
}