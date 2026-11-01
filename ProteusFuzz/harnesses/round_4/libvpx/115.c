// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
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

// Helper to safely initialize a codec context for fuzzing
static vpx_codec_ctx_t *init_codec_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    vpx_codec_enc_cfg_t cfg;

    // Try to get VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    ctx->iface = iface;
    ctx->name = "vp8";
    ctx->err = VPX_CODEC_OK;
    ctx->err_detail = NULL;
    ctx->init_flags = 0;
    ctx->priv = NULL;

    // Initialize encoder config
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set some basic config values from input data if available
    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    ctx->config.enc = &cfg;

    // Initialize the codec
    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_codec_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        if (ctx->priv) {
            // Note: In real usage, we might need to free private data, 
            // but for fuzzing we just free the context structure
            // The actual priv memory is managed internally
        }
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_err_t res;
    int ctrl_id;
    int value;

    if (Size == 0) return 0;

    // Initialize codec context
    ctx = init_codec_context(Data, Size);
    if (!ctx) return 0;

    // Extract control ID and value from input data
    ctrl_id = (Data[0] % 1000) + 100; // Range to cover various control IDs
    if (Size > 1) {
        value = (int)(Data[1] * 256 + (Size > 2 ? Data[2] : 0));
    } else {
        value = 0;
    }

    // Test vpx_codec_control_ with various control IDs
    // This covers VP8E_SET_TUNING, VP9E_SET_TUNE_CONTENT, and others
    res = vpx_codec_control_(ctx, ctrl_id, value);
    // We don't assert here as we expect various return values

    // Try specific known control IDs if data allows
    if (Size > 3) {
        int specific_ctrl = Data[3] % 5;
        switch (specific_ctrl) {
            case 0:
                // Try VP8E_SET_TUNING (if supported)
                res = vpx_codec_control_(ctx, VP8E_SET_TUNING, value);
                break;
            case 1:
                // Try VP9E_SET_TUNE_CONTENT (if supported)
                res = vpx_codec_control_(ctx, VP9E_SET_TUNE_CONTENT, value);
                break;
            case 2:
                // Try VP8E_GET_LAST_QUANTIZER
                res = vpx_codec_control_(ctx, VP8E_GET_LAST_QUANTIZER, &value);
                break;
            case 3:
                // Try VPXD_GET_LAST_QUANTIZER
                res = vpx_codec_control_(ctx, VPXD_GET_LAST_QUANTIZER, &value);
                break;
            case 4:
                // Try VP8E_GET_LAST_QUANTIZER_64
                res = vpx_codec_control_(ctx, VP8E_GET_LAST_QUANTIZER_64, &value);
                break;
        }
    }

    // Cleanup
    cleanup_codec_context(ctx);

    return 0;
}