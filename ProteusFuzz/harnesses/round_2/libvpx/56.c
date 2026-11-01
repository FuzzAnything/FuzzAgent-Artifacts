// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_GET_LAST_QUANTIZER__value at vp8cx.h:996:1 in vp8cx.h
// VP8E_GET_LAST_QUANTIZER_64__value at vp8cx.h:998:1 in vp8cx.h
// VPXD_GET_LAST_QUANTIZER__value at vp8dx.h:214:1 in vp8dx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx encoding control functions.
 * Targets: VP9E_SET_TILE_ROWS, VP9E_SET_TILE_COLUMNS,
 *          VP8E_GET_LAST_QUANTIZER, VPXD_GET_LAST_QUANTIZER (via vpx_codec_control)
 *
 * Strategy:
 * 1. Initialize a VP9 encoder context.
 * 2. Use fuzzed input to determine configuration parameters (tile rows, cols).
 * 3. Invoke vpx_codec_control with various control IDs to trigger target functions.
 * 4. Handle errors and cleanup properly.
 */

static int init_encoder(vpx_codec_ctx_t *ctx, int width, int height, int fps_num, int fps_den) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (!iface) {
        return -1;
    }

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    /* Apply fuzzed dimensions, clamp to valid ranges */
    cfg.g_w = width > 0 ? width : 16;
    cfg.g_h = height > 0 ? height : 16;
    cfg.g_timebase.num = fps_num > 0 ? fps_num : 1;
    cfg.g_timebase.den = fps_den > 0 ? fps_den : 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    int tile_rows, tile_cols;
    int width, height, fps_num, fps_den;
    int ctrl_id;
    int value;

    if (Size < 4) return 0;

    /* Parse input data */
    width = (int)Data[0];
    height = (int)Data[1];
    fps_num = (int)Data[2];
    fps_den = (int)Data[3];
    
    /* Ensure non-zero values for initialization */
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (fps_num <= 0) fps_num = 1;
    if (fps_den <= 0) fps_den = 30;

    /* Initialize encoder */
    if (init_encoder(&ctx, width, height, fps_num, fps_den) != 0) {
        return 0;
    }

    /* Extract parameters for tile configuration if enough data */
    if (Size >= 6) {
        tile_rows = (int)Data[4];
        tile_cols = (int)Data[5];
    } else {
        tile_rows = 0;
        tile_cols = 0;
    }

    /* Try setting tile rows */
    if (tile_rows >= 0 && tile_rows <= 8) {
        res = vpx_codec_control(&ctx, VP9E_SET_TILE_ROWS, tile_rows);
        if (res != VPX_CODEC_OK && res != VPX_CODEC_INCAPABLE) {
            /* Ignore errors, continue fuzzing */
        }
    }

    /* Try setting tile columns */
    if (tile_cols >= 0 && tile_cols <= 8) {
        res = vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);
        if (res != VPX_CODEC_OK && res != VPX_CODEC_INCAPABLE) {
            /* Ignore errors, continue fuzzing */
        }
    }

    /* Try getting last quantizer for VP8 (might fail on VP9 context, which is expected) */
    int *last_quantizer = NULL;
    res = vpx_codec_control(&ctx, VP8E_GET_LAST_QUANTIZER, &last_quantizer);
    /* Ignore result, just exercising the path */

    /* Try getting last quantizer 64-bit version if available (might be unsupported) */
    int64_t *last_quantizer_64 = NULL;
    res = vpx_codec_control(&ctx, VP8E_GET_LAST_QUANTIZER_64, &last_quantizer_64);
    /* Ignore result */

    /* Try VPXD_GET_LAST_QUANTIZER (decoder side, might fail on encoder context) */
    int *decoder_quantizer = NULL;
    res = vpx_codec_control(&ctx, VPXD_GET_LAST_QUANTIZER, &decoder_quantizer);
    /* Ignore result */

    /* Randomly pick a control ID from the range to explore more states */
    if (Size >= 7) {
        ctrl_id = (int)Data[6];
        value = (Size >= 8) ? (int)Data[7] : 0;
        
        /* Attempt a generic control call with fuzzed ID and value using the internal function
         * because the macro vpx_codec_control requires a compile-time constant for the ID.
         */
        res = vpx_codec_control_(&ctx, ctrl_id, value);
        /* Ignore result */
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}