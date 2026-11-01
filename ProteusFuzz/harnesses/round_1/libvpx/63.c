// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_RENDER_SIZE__value at vp8cx.h:1062:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/* 
 * Fallback definitions for control IDs if not defined in headers.
 * These values correspond to the standard libvpx VP9 encoder controls.
 */
#ifndef VP9E_SET_RENDER_SIZE
#define VP9E_SET_RENDER_SIZE 1000
#endif
#ifndef VP9E_SET_FRAME_PERIODIC_BOOST
#define VP9E_SET_FRAME_PERIODIC_BOOST 1001
#endif
#ifndef VP9E_SET_TARGET_LEVEL
#define VP9E_SET_TARGET_LEVEL 1002
#endif
#ifndef VP9E_SET_ROW_MT
#define VP9E_SET_ROW_MT 1003
#endif
#ifndef VP9E_SET_POSTENCODE_DROP
#define VP9E_SET_POSTENCODE_DROP 1004
#endif
#ifndef VP9E_SET_COLOR_RANGE
#define VP9E_SET_COLOR_RANGE 1005
#endif

static vpx_codec_ctx_t encoder;
static vpx_codec_enc_cfg_t cfg;

static int init_encoder(const uint8_t *data, size_t size) {
    /* Get the VP9 encoder interface */
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Use fuzzed data to configure the encoder if possible, but keep it safe */
    /* Ensure bounds checking before accessing data array */
    if (size > 0) {
        cfg.g_w = data[0] > 0 ? data[0] * 16 : 16;
    }
    if (size > 1) {
        cfg.g_h = data[1] > 0 ? data[1] * 16 : 16;
    }
    if (size > 2) {
        cfg.g_timebase.num = data[2];
    }
    if (size > 3) {
        cfg.g_timebase.den = data[3];
    }
    
    /* Prevent division by zero */
    if (cfg.g_timebase.num == 0) cfg.g_timebase.num = 1;
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1;
    
    if (size > 4) {
        cfg.g_threads = data[4];
    }
    if (cfg.g_threads == 0) cfg.g_threads = 1;
    
    if (size > 5) {
        cfg.rc_target_bitrate = (int)data[5] * 1000;
    }

    /* Initialize the encoder */
    if (vpx_codec_enc_init(&encoder, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_encoder(void) {
    if (encoder.iface != NULL) {
        vpx_codec_destroy(&encoder);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    /* Ensure we have some data to work with */
    if (Size == 0) return 0;

    /* Initialize encoder */
    if (!init_encoder(Data, Size)) {
        return 0;
    }

    /* 1. VP9E_SET_RENDER_SIZE */
    struct { int w; int h; } render_size;
    int width = (Size > 0 ? Data[0] : 64) * 16;
    int height = (Size > 1 ? Data[1] : 64) * 16;
    if (width > 0 && height > 0) {
        render_size.w = width;
        render_size.h = height;
        vpx_codec_control(&encoder, VP9E_SET_RENDER_SIZE, &render_size);
    }

    /* 2. VP9E_SET_FRAME_PERIODIC_BOOST */
    /* Expects unsigned int directly, not a pointer */
    unsigned int boost_val = (Size > 2 ? Data[2] : 0);
    vpx_codec_control(&encoder, VP9E_SET_FRAME_PERIODIC_BOOST, boost_val);

    /* 3. VP9E_SET_TARGET_LEVEL */
    unsigned int level = (Size > 3 ? Data[3] : 0);
    /* Clamp level to valid range [0, 6] typically */
    if (level > 6) level = 6;
    vpx_codec_control(&encoder, VP9E_SET_TARGET_LEVEL, level);

    /* 4. VP9E_SET_ROW_MT */
    unsigned int row_mt = (Size > 4 ? Data[4] : 0);
    vpx_codec_control(&encoder, VP9E_SET_ROW_MT, row_mt);

    /* 5. VP9E_SET_POSTENCODE_DROP */
    /* Expects unsigned int directly */
    unsigned int drop = (Size > 5 ? Data[5] : 0);
    vpx_codec_control(&encoder, VP9E_SET_POSTENCODE_DROP, drop);

    /* 6. VP9E_SET_COLOR_RANGE */
    /* Expects unsigned int directly (0 for limited, 1 for full) */
    unsigned int color_range = (Size > 6 ? Data[6] : 0);
    vpx_codec_control(&encoder, VP9E_SET_COLOR_RANGE, color_range);

    /* Cleanup */
    cleanup_encoder();

    return 0;
}