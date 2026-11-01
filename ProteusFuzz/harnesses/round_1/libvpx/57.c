// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8_SET_POSTPROC__value at vp8.h:124:1 in vp8.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

/*
 * Fuzz driver for libvpx vpx_codec_control_ function.
 * 
 * This driver initializes a VP8 encoder context and dispatches various 
 * control commands using vpx_codec_control based on the fuzzing input.
 */

static int try_encode_init(vpx_codec_ctx_t *ctx, const vpx_codec_enc_cfg_t *cfg) {
    vpx_codec_iface_t *iface;
    vpx_codec_err_t err;

    // Use the correct function name for VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return -1;
    }

    err = vpx_codec_enc_init(ctx, iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg_enc;
    vpx_codec_err_t err;
    int ctrl_id = 0;
    int value_int = 0;
    int value_int2 = 0;
    vp8_postproc_cfg_t postproc_cfg;
    
    if (Size < 2) {
        return 0;
    }

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg_enc, 0, sizeof(cfg_enc));
    memset(&postproc_cfg, 0, sizeof(postproc_cfg));

    // Parse control ID from input
    ctrl_id = (int)(Data[0] & 0xFF);

    // Parse additional values for control arguments
    if (Size > 1) value_int = (int)(Data[1] & 0xFF);
    if (Size > 2) value_int2 = (int)(Data[2] & 0xFF);

    // Setup minimal encoder config
    cfg_enc.g_w = 16;
    cfg_enc.g_h = 16;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;
    cfg_enc.g_lag_in_frames = 0;
    cfg_enc.rc_target_bitrate = 100;
    cfg_enc.g_error_resilient = 1;

    if (try_encode_init(&ctx, &cfg_enc) != 0) {
        return 0;
    }

    // Map raw ctrl_id to known constants to ensure coverage of specific paths
    if (ctrl_id == 0) {
        // VP8E_SET_NOISE_SENSITIVITY
        err = vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, value_int);
    } else if (ctrl_id == 1) {
        // VP8E_SET_SCREEN_CONTENT_MODE
        err = vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, value_int);
    } else if (ctrl_id == 2) {
        // VP8_SET_POSTPROC
        postproc_cfg.post_proc_flag = value_int;
        postproc_cfg.deblocking_level = value_int2 & 0x0F; 
        postproc_cfg.noise_level = (value_int2 >> 4) & 0x0F; 
        err = vpx_codec_control(&ctx, VP8_SET_POSTPROC, &postproc_cfg);
    } else if (ctrl_id == 3) {
        // VP9E_SET_NOISE_SENSITIVITY (might return INCAPABLE on VP8 context)
        err = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, value_int);
    } else if (ctrl_id == 4) {
        // VP9E_SET_FRAME_PARALLEL_DECODING
        err = vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, value_int);
    } else {
        // Use raw value to test generic error handling for unknown IDs
        // We cast to int to ensure the macro receives a literal-like token if possible, 
        // but since it's a variable, we rely on the library's internal handling or error.
        // Note: The macro vpx_codec_control expects a literal ID to generate the function name.
        // Passing a variable directly to the macro vpx_codec_control(ctx, var_id, ...) 
        // will fail to compile because the preprocessor cannot concatenate 'var_id'.
        // To fuzz unknown IDs, we must call the underlying function vpx_codec_control_ directly
        // or restrict fuzzing to known IDs.
        // Given the macro limitation, we will skip unknown IDs to ensure compilation,
        // or we can use the raw function if we want to test invalid IDs.
        // Let's use the raw function for unknown IDs to test error paths.
        err = vpx_codec_control_(&ctx, ctrl_id, value_int);
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}