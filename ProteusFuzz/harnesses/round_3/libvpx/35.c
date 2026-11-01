// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_COLOR_SPACE__value at vp8cx.h:1050:1 in vp8cx.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void free_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

static int setup_encoder(vpx_codec_ctx_t **ctx, vpx_codec_iface_t *codec, vpx_codec_enc_cfg_t *cfg) {
    if (!ctx || !codec || !cfg) return -1;

    *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!*ctx) return -1;

    memset(*ctx, 0, sizeof(vpx_codec_ctx_t));

    if (vpx_codec_enc_init(*ctx, codec, cfg, 0) != VPX_CODEC_OK) {
        free_encoder_context(*ctx);
        return -1;
    }

    return 0;
}

static int set_encoder_controls(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    int value;
    int ret = 0;

    // VP9E_SET_COLOR_SPACE
    if (size >= 1) {
        value = (int)(data[0] & 0xFF);
        ret = vpx_codec_control(ctx, VP9E_SET_COLOR_SPACE, value);
        if (ret != VPX_CODEC_OK) return ret;
    }

    // VP9E_SET_TILE_ROWS
    if (size >= 2) {
        value = (int)(data[1] & 0xFF);
        ret = vpx_codec_control(ctx, VP9E_SET_TILE_ROWS, value);
        if (ret != VPX_CODEC_OK) return ret;
    }

    // VP9E_SET_ALT_REF_AQ
    if (size >= 3) {
        value = (int)(data[2] & 0xFF);
        ret = vpx_codec_control(ctx, VP9E_SET_ALT_REF_AQ, value);
        if (ret != VPX_CODEC_OK) return ret;
    }

    // VP9E_SET_COLOR_RANGE
    if (size >= 4) {
        value = (int)(data[3] & 0xFF);
        ret = vpx_codec_control(ctx, VP9E_SET_COLOR_RANGE, value);
        if (ret != VPX_CODEC_OK) return ret;
    }

    // VP9E_SET_QUANTIZER_ONE_PASS
    if (size >= 5) {
        value = (int)(data[4] & 0xFF);
        ret = vpx_codec_control(ctx, VP9E_SET_QUANTIZER_ONE_PASS, value);
        if (ret != VPX_CODEC_OK) return ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Minimum size check to ensure we have enough data for basic config
    if (Size < 1) return 0;

    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Safely derive config values from input, checking bounds first
    cfg.g_w = (Size > 0 ? Data[0] : 16) % 512 + 16;
    cfg.g_h = (Size > 1 ? Data[1] : 16) % 512 + 16;
    
    // Ensure timebase denominator is not zero
    uint8_t num_val = (Size > 2 ? Data[2] : 1) % 100 + 1;
    uint8_t den_val = (Size > 3 ? Data[3] : 1) % 100 + 1;
    cfg.g_timebase.num = num_val;
    cfg.g_timebase.den = den_val;
    
    cfg.rc_target_bitrate = (Size > 4 ? Data[4] : 100) % 100 + 100;

    vpx_codec_ctx_t *ctx = NULL;
    if (setup_encoder(&ctx, codec, &cfg) != 0) {
        return 0;
    }

    // Apply encoder controls
    set_encoder_controls(ctx, Data, Size);

    // Cleanup
    free_encoder_context(ctx);

    return 0;
}