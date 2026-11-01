// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP8E_SET_SCALEMODE__value at vp8cx.h:982:1 in vp8cx.h
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

/*
 * Fuzz driver for libvpx encoder control APIs.
 * Targets:
 * - VP9E_SET_SVC
 * - VP9E_SET_TILE_COLUMNS
 * - VP9E_SET_TILE_ROWS
 * - VP9E_SET_DISABLE_LOOPFILTER
 * - VP8E_SET_SCALEMODE
 * via vpx_codec_control_
 */

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Initialize encoder configuration */
    vpx_codec_enc_cfg_t cfg;
    /* Use the VP9 encoder interface */
    if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Parse fuzz input to configure encoder settings */
    int svc_value = Data[0];
    int tile_cols = (Size > 1) ? Data[1] : 0;
    int tile_rows = (Size > 2) ? Data[2] : 0;
    int disable_loopfilter = (Size > 3) ? Data[3] : 0;
    int scale_mode_h = (Size > 4) ? Data[4] : 0;
    int scale_mode_v = (Size > 5) ? Data[5] : 0;

    /* Configure basic encoder parameters */
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_target_bitrate = 1000;

    /* Initialize encoder */
    if (vpx_codec_enc_init(&ctx, vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /*
     * Fuzz various control APIs
     * We use the actual macro definitions from libvpx headers.
     */

    /* Fuzz VP9E_SET_SVC */
    vpx_codec_control(&ctx, VP9E_SET_SVC, svc_value);

    /* Fuzz VP9E_SET_TILE_COLUMNS */
    if (tile_cols >= 0 && tile_cols <= 16) {
        vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);
    }

    /* Fuzz VP9E_SET_TILE_ROWS */
    if (tile_rows >= 0 && tile_rows <= 16) {
        vpx_codec_control(&ctx, VP9E_SET_TILE_ROWS, tile_rows);
    }

    /* Fuzz VP9E_SET_DISABLE_LOOPFILTER */
    if (disable_loopfilter == 0 || disable_loopfilter == 1) {
        vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter);
    }

    /* Fuzz VP8E_SET_SCALEMODE (requires vpx_scaling_mode_t) */
    if (scale_mode_h >= 0 && scale_mode_h <= 3 && scale_mode_v >= 0 && scale_mode_v <= 3) {
        vpx_scaling_mode_t sm;
        sm.h_scaling_mode = (VPX_SCALING_MODE)scale_mode_h;
        sm.v_scaling_mode = (VPX_SCALING_MODE)scale_mode_v;
        vpx_codec_control(&ctx, VP8E_SET_SCALEMODE, &sm);
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}