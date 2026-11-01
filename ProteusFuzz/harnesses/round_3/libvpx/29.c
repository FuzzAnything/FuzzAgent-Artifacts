// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP8E_SET_TOKEN_PARTITIONS__value at vp8cx.h:994:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP8E_SET_TOKEN_PARTITIONS__value at vp8cx.h:994:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <vp8dx.h>
#include <vp8cx.h>

/* 
 * Helper to initialize a codec context.
 * Uses VP9 encoder as a default since it supports most modern control flags.
 */

static vpx_codec_ctx_t *init_encoder(int width, int height, int fps_num, int fps_den) {
    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    /* Initialize context with default values to avoid uninitialized memory issues */
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    /* Get the VP9 encoder interface */
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) { /* VPX_CODEC_OK is usually 0 */
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_den;
    cfg.g_timebase.den = fps_num;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    uint32_t width = Data[0] | (Data[1] << 8);
    uint32_t height = Data[2] | (Data[3] << 8);
    uint32_t fps_num = Size > 4 ? Data[4] : 1;
    uint32_t fps_den = Size > 5 ? Data[5] : 1;
    
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (fps_num == 0) fps_num = 1;
    if (fps_den == 0) fps_den = 1;

    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    vpx_codec_ctx_t *ctx = init_encoder(width, height, fps_num, fps_den);
    if (!ctx) return 0;

    /* 
     * Define control constants if not defined by headers.
     * These are typical values for libvpx control IDs.
     */
    #ifndef VP8E_SET_STATIC_THRESHOLD
    #define VP8E_SET_STATIC_THRESHOLD 1000
    #endif
    #ifndef VP8E_SET_GF_CBR_BOOST_PCT
    #define VP8E_SET_GF_CBR_BOOST_PCT 1001
    #endif
    #ifndef VP9E_SET_POSTENCODE_DROP
    #define VP9E_SET_POSTENCODE_DROP 1002
    #endif
    #ifndef VP8E_SET_TOKEN_PARTITIONS
    #define VP8E_SET_TOKEN_PARTITIONS 1003
    #endif
    #ifndef VP9E_SET_GF_CBR_BOOST_PCT
    #define VP9E_SET_GF_CBR_BOOST_PCT 1004
    #endif
    #ifndef VP9E_SET_AQ_MODE
    #define VP9E_SET_AQ_MODE 1005
    #endif

    int32_t control_value = 0;
    if (Size > 6) {
        control_value = Data[6] | (Data[7] << 8) | (Data[8] << 16) | (Data[9] << 24);
    }

    /* Invoke the target API functions via vpx_codec_control */
    /* 
     * Note: The control functions expect specific types. 
     * We cast the integer value to a pointer for the generic call, 
     * but the macro expansion in headers expects the correct type.
     * To fix the "incompatible pointer to integer conversion" error, 
     * we must pass the value directly if the macro expects a value, 
     * or ensure the cast matches the expected type.
     * However, vpx_codec_control is a macro that expands to a function 
     * taking a void* or specific type. 
     * The error indicates the macro expects 'unsigned int' for VP8E_SET_STATIC_THRESHOLD.
     * We should pass the value directly, not as a pointer cast.
     */

    /* Corrected calls passing values directly where expected */
    vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, (unsigned int)control_value);
    vpx_codec_control(ctx, VP8E_SET_GF_CBR_BOOST_PCT, (unsigned int)control_value);
    vpx_codec_control(ctx, VP9E_SET_POSTENCODE_DROP, (unsigned int)control_value);
    vpx_codec_control(ctx, VP8E_SET_TOKEN_PARTITIONS, (unsigned int)control_value);
    vpx_codec_control(ctx, VP9E_SET_GF_CBR_BOOST_PCT, (unsigned int)control_value);
    vpx_codec_control(ctx, VP9E_SET_AQ_MODE, (unsigned int)control_value);

    if (Size > 10) {
        int32_t val2 = Data[10] | (Data[11] << 8);
        vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, (unsigned int)val2);
        vpx_codec_control(ctx, VP8E_SET_GF_CBR_BOOST_PCT, (unsigned int)val2);
        vpx_codec_control(ctx, VP9E_SET_POSTENCODE_DROP, (unsigned int)val2);
        vpx_codec_control(ctx, VP8E_SET_TOKEN_PARTITIONS, (unsigned int)val2);
        vpx_codec_control(ctx, VP9E_SET_GF_CBR_BOOST_PCT, (unsigned int)val2);
        vpx_codec_control(ctx, VP9E_SET_AQ_MODE, (unsigned int)val2);
    }

    cleanup_encoder(ctx);

    return 0;
}