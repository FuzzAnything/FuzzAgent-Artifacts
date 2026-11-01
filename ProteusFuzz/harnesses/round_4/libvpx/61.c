// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int fuzz_encoder_control(vpx_codec_ctx_t *ctx, int ctrl_id, ...) {
    va_list args;
    va_start(args, ctrl_id);
    vpx_codec_err_t res = vpx_codec_control_(ctx, ctrl_id, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    if (!encoder_iface) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings fuzzily
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_error_resilient = (Data[5] % 3);
    cfg.g_lag_in_frames = (Data[6] % 20);
    cfg.g_pass = (Data[7] % 3);
    cfg.rc_end_usage = (Data[8] % 3);
    cfg.g_threads = (Data[9] % 4) + 1;

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder control commands
    int ctrl_id = Data[10] % 10;
    int value = (Data[11] % 10) - 5;

    // Simulate various control commands
    switch (ctrl_id) {
        case 0: // VP8E_SET_NOISE_SENSITIVITY
            fuzz_encoder_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, value);
            break;
        case 1: // VP8E_SET_ARNR_TYPE
            fuzz_encoder_control(&ctx, VP8E_SET_ARNR_TYPE, value);
            break;
        case 2: // VP8E_SET_ARNR_MAXFRAMES
            fuzz_encoder_control(&ctx, VP8E_SET_ARNR_MAXFRAMES, value);
            break;
        case 3: // VP8E_SET_CPUUSED
            fuzz_encoder_control(&ctx, VP8E_SET_CPUUSED, value);
            break;
        case 4: // VP8E_SET_ARNR_STRENGTH
            fuzz_encoder_control(&ctx, VP8E_SET_ARNR_STRENGTH, value);
            break;
        default:
            // Try random control IDs
            fuzz_encoder_control(&ctx, ctrl_id, value);
            break;
    }

    // Cleanup encoder
    vpx_codec_destroy(&ctx);

    return 0;
}