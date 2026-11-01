// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx codec control functions.
 * Focuses on vpx_codec_control_ and related control IDs.
 * Since several target functions are internal or invalid, we focus on
 * valid public APIs that exercise similar paths: VP8E_SET_TUNING,
 * VP8E_GET_LAST_QUANTIZER, VP9E_SET_TUNE_CONTENT, etc.
 */

static int try_encode_init(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;

    /* Choose encoder based on data content to diversify states */
    if (size > 0 && data[0] % 2 == 0) {
        iface = vpx_codec_vp8_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }

    if (!iface) return 0;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    /* Use fuzz data to tweak config slightly */
    if (size > 0) cfg.g_w = (data[0] % 32) + 16;
    if (size > 1) cfg.g_h = (data[1] % 32) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 255) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 255) + 1;
    cfg.g_threads = (size > 4 ? data[4] % 4 : 1) + 1;
    cfg.rc_target_bitrate = (size > 5 ? (data[5] * 100) : 1000);

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    return 1;
}

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    int ctrl_id;
    int val;

    if (Size == 0) return 0;

    /* Initialize context */
    memset(&ctx, 0, sizeof(ctx));

    /* Try to initialize encoder */
    if (!try_encode_init(&ctx, Data, Size)) {
        return 0;
    }

    /* Derive control ID and value from input */
    ctrl_id = Data[0] % 1000;
    val = (Size > 1 ? Data[1] : 0);

    /* Map some common control IDs to valid ones for testing */
    switch (ctrl_id) {
        case 0:
            ctrl_id = VP8E_SET_TUNING;
            val = val % 10; /* Valid tuning values 0-9 */
            break;
        case 1:
            ctrl_id = VP8E_GET_LAST_QUANTIZER;
            break;
        case 2:
            ctrl_id = VP8E_GET_LAST_QUANTIZER_64;
            break;
        case 3:
            ctrl_id = VP9E_SET_TUNE_CONTENT;
            val = val % 4; /* Valid content types */
            break;
        case 4:
            ctrl_id = VPXD_GET_LAST_QUANTIZER;
            break;
        default:
            /* Try some other valid control IDs randomly */
            if (val % 2 == 0) {
                ctrl_id = VP8E_SET_TUNING;
                val = val % 10;
            } else {
                ctrl_id = VP9E_SET_TUNE_CONTENT;
                val = val % 4;
            }
            break;
    }

    /* Invoke vpx_codec_control_ directly as per task requirement */
    res = vpx_codec_control_(&ctx, ctrl_id, val);

    /* Handle return values */
    if (res != VPX_CODEC_OK && res != VPX_CODEC_INCAPABLE) {
        /* Invalid parameter or error - acceptable in fuzzing */
    }

    /* Cleanup */
    cleanup_codec(&ctx);

    return 0;
}