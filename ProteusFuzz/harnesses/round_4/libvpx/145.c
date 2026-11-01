// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx vpx_codec_control_ and related control IDs.
 * Focuses on initializing an encoder context and invoking
 * vpx_codec_control_ with various control IDs derived from the input data.
 */

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

static vpx_codec_err_t init_encoder_context(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return VPX_CODEC_ERROR;
    }

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return res;
    }

    /* Use fuzzing data to set some configuration parameters safely */
    /* Ensure we have enough data before accessing indices */
    if (size >= 5) {
        cfg.g_w = 16 + (data[0] % 1024);
        cfg.g_h = 16 + (data[1] % 1024);
        cfg.g_timebase.num = 1 + (data[2] % 10);
        cfg.g_timebase.den = 1 + (data[3] % 10);
        cfg.g_threads = 1 + (data[4] % 4);
    }

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    return res;
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx = {0};
    vpx_codec_err_t res;
    int ctrl_id;

    if (Size < 1) {
        return 0;
    }

    /* Initialize encoder context */
    res = init_encoder_context(&ctx, Data, Size);
    if (res != VPX_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    /* Extract control ID from input data safely */
    if (Size >= 5) {
        ctrl_id = ((Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4]) & 0x7FFFFFFF;
    } else {
        ctrl_id = Data[0] % 1000;
    }

    /*
     * Try to invoke vpx_codec_control_ with various control IDs.
     * We pass minimal or no arguments where possible.
     */

    /* Attempt 1: No arguments (for controls that don't require them) */
    res = vpx_codec_control_(&ctx, ctrl_id);

    /* Attempt 2: With a single integer argument */
    if (Size > 5) {
        int arg1 = (int)(Data[5] % 256);
        res = vpx_codec_control_(&ctx, ctrl_id, arg1);
    }

    /* Attempt 3: With two integer arguments */
    if (Size > 6) {
        int arg1 = (int)(Data[5] % 256);
        int arg2 = (int)(Data[6] % 256);
        res = vpx_codec_control_(&ctx, ctrl_id, arg1, arg2);
    }

    /* Attempt 4: With a pointer argument (dummy pointer) */
    if (Size > 7) {
        int arg1 = (int)(Data[7] % 256);
        int dummy = 0;
        res = vpx_codec_control_(&ctx, ctrl_id, arg1, &dummy);
    }

    /* Cleanup */
    cleanup_codec(&ctx);

    return 0;
}