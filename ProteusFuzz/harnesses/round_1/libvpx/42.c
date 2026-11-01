// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ARNR_TYPE__value at vp8cx.h:1004:1 in vp8cx.h
// VP8E_SET_ARNR_MAXFRAMES__value at vp8cx.h:1000:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder = NULL;
    vpx_codec_err_t res;

    // Initialize context with zeroed memory
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Attempt to get the VP8 encoder interface
    encoder = vpx_codec_vp8_cx();
    if (!encoder) {
        return 0;
    }

    // Initialize configuration
    res = vpx_codec_enc_config_default(encoder, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_lag_in_frames = Data[4] % 20;
    cfg.rc_target_bitrate = (Data[5] % 1000) + 100;

    // Initialize the encoder
    res = vpx_codec_enc_init(&ctx, encoder, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Extract arguments from input
    int arg_val = Data[6] % 100;
    unsigned int flags = Data[7];

    // Invoke specific control commands based on flags to ensure valid macro expansion
    // We must use literal constants in the macro invocation because the macro concatenates the ID.
    
    if (flags & 0x01) {
        // VP8E_SET_CPUUSED: expects int (0-8)
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, arg_val % 9);
    }
    
    if (flags & 0x02) {
        // VP8E_SET_NOISE_SENSITIVITY: expects int
        vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, arg_val);
    }
    
    if (flags & 0x04) {
        // VP8E_SET_ARNR_TYPE: expects int
        vpx_codec_control(&ctx, VP8E_SET_ARNR_TYPE, arg_val % 4);
    }
    
    if (flags & 0x08) {
        // VP8E_SET_ARNR_MAXFRAMES: expects int
        vpx_codec_control(&ctx, VP8E_SET_ARNR_MAXFRAMES, arg_val % 16);
    }
    
    if (flags & 0x10) {
        // VP8E_SET_ARNR_STRENGTH: expects int
        vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arg_val % 10);
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}