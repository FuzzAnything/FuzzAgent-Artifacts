// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_MIN_GF_INTERVAL__value at vp8cx.h:1052:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

static vpx_codec_ctx_t* create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    if (!iface) {
        return NULL;
    }

    ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_pass = 0;
    cfg.g_profile = 0;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    int val;
    if (Size > 0) {
        val = Data[0];
    } else {
        val = 0;
    }

    // Fuzz VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(ctx, VP9E_SET_MAX_GF_INTERVAL, val);
    
    // Fuzz VP9E_SET_LOSSLESS
    vpx_codec_control(ctx, VP9E_SET_LOSSLESS, val);
    
    // Fuzz VP9E_SET_FRAME_PERIODIC_BOOST
    vpx_codec_control(ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val);
    
    // Fuzz VP9E_SET_TARGET_LEVEL
    vpx_codec_control(ctx, VP9E_SET_TARGET_LEVEL, val);
    
    // Fuzz VP9E_SET_MIN_GF_INTERVAL
    vpx_codec_control(ctx, VP9E_SET_MIN_GF_INTERVAL, val);
    
    // Fuzz VP9E_SET_POSTENCODE_DROP
    vpx_codec_control(ctx, VP9E_SET_POSTENCODE_DROP, val);

    cleanup_encoder(ctx);
    return 0;
}