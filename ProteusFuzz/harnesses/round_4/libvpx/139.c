// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8_SET_POSTPROC__value at vp8.h:124:1 in vp8.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vp8.h"
#include "vp8cx.h"
#include "vp8dx.h"

static int get_int(const uint8_t *d, size_t *idx, size_t size, int max_val) {
    if (*idx + 4 > size) return 0;
    int val = d[*idx] | (d[*idx + 1] << 8) | (d[*idx + 2] << 16) | (d[*idx + 3] << 24);
    *idx += 4;
    if (val < 0) val = 0;
    if (val > max_val) val = max_val;
    return val;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        vpx_codec_destroy(ctx);
    }
}

static int test_vp8_encoder_controls(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    size_t idx = 0;

    // VP8E_SET_NOISE_SENSITIVITY: typically range 0-255 or similar
    int noise_sens = get_int(data, &idx, size, 255);
    vpx_codec_control(ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sens);

    // VP8E_SET_MAX_INTRA_BITRATE_PCT: typically 0-100
    int max_intra_pct = get_int(data, &idx, size, 100);
    vpx_codec_control(ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);

    // VP8E_SET_ARNR_STRENGTH: typically 0-3
    int arnr_strength = get_int(data, &idx, size, 3);
    vpx_codec_control(ctx, VP8E_SET_ARNR_STRENGTH, arnr_strength);

    // VP8E_SET_SCREEN_CONTENT_MODE: typically 0 or 1
    int screen_content = get_int(data, &idx, size, 1);
    vpx_codec_control(ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_content);

    // VP8_SET_POSTPROC: requires vp8_postproc_cfg_t
    if (idx + 12 <= size) {
        vp8_postproc_cfg_t cfg;
        cfg.post_proc_flag = get_int(data, &idx, size, 255);
        cfg.deblocking_level = get_int(data, &idx, size, 16);
        cfg.noise_level = get_int(data, &idx, size, 16);
        vpx_codec_control(ctx, VP8_SET_POSTPROC, &cfg);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Configure basic encoder settings
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Run the control tests
    test_vp8_encoder_controls(&ctx, Data, Size);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}