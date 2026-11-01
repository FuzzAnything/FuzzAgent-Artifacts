// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void *safe_malloc(size_t size) {
    if (size == 0 || size > 1024 * 1024 * 10) {
        return NULL;
    }
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    vpx_codec_iface_t *encoder = vpx_codec_vp9_cx();
    if (!encoder) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)safe_malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_encoder(ctx);
        return 0;
    }

    cfg.g_w = Data[0] % 256;
    cfg.g_h = (Size > 1 ? Data[1] : 0) % 256;
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_pass = 1;
    cfg.g_profile = 0;

    if (vpx_codec_enc_init(ctx, encoder, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_encoder(ctx);
        return 0;
    }

    // Fuzz VP9E_SET_MAX_GF_INTERVAL
    if (Size > 0) {
        int max_gf_interval = Data[0];
        vpx_codec_control(ctx, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
    }

    // Fuzz VP9E_SET_TILE_ROWS
    if (Size > 1) {
        int tile_rows = Data[1];
        vpx_codec_control(ctx, VP9E_SET_TILE_ROWS, tile_rows);
    }

    // Fuzz VP9E_SET_DELTA_Q_UV
    if (Size > 2) {
        int delta_q_uv = Data[2];
        vpx_codec_control(ctx, VP9E_SET_DELTA_Q_UV, delta_q_uv);
    }

    // Fuzz VP9E_SET_TARGET_LEVEL
    if (Size > 3) {
        int target_level = Data[3];
        vpx_codec_control(ctx, VP9E_SET_TARGET_LEVEL, target_level);
    }

    // Fuzz VP9E_SET_COLOR_RANGE
    if (Size > 4) {
        int color_range = Data[4];
        vpx_codec_control(ctx, VP9E_SET_COLOR_RANGE, color_range);
    }

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    if (Size > 5) {
        int quantizer_one_pass = Data[5];
        vpx_codec_control(ctx, VP9E_SET_QUANTIZER_ONE_PASS, quantizer_one_pass);
    }

    cleanup_encoder(ctx);
    return 0;
}