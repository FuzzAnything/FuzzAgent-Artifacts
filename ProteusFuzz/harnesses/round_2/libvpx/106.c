// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"

static int try_fuzz_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    int ctrl_id;
    int val;

    if (size < 6) return 0;

    iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (data[0] % 256) * 16;
    cfg.g_h = (data[1] % 256) * 16;
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    ctrl_id = (int)(data[2] | (data[3] << 8));
    val = (int)(data[4] | (data[5] << 8));

    int known_ctrls[] = {
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_NOISE_SENSITIVITY,
        VP8E_SET_SHARPNESS,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_TOKEN_PARTITIONS,
        VP8E_SET_CPUUSED,
        VP9E_SET_TILE_COLUMNS,
        VP9E_SET_TILE_ROWS,
        0
    };

    res = vpx_codec_control_(&ctx, ctrl_id, val);
    (void)res;

    for (int i = 0; known_ctrls[i] != 0; i++) {
        int fuzz_val = (int)(data[(i % size) + 6]);
        res = vpx_codec_control_(&ctx, known_ctrls[i], fuzz_val);
        (void)res;
    }

    vpx_codec_destroy(&ctx);
    return 1;
}

static int try_fuzz_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    int ctrl_id;
    int val;

    if (size < 6) return 0;

    iface = vpx_codec_vp9_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    res = vpx_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    ctrl_id = (int)(data[0] | (data[1] << 8));
    val = (int)(data[2] | (data[3] << 8));

    /* Use correct constant names available in vp8dx.h */
    int known_ctrls[] = {
        VP9_SET_SKIP_LOOP_FILTER,
        VP9D_GET_FRAME_SIZE,
        VP9D_GET_DISPLAY_SIZE,
        0
    };

    res = vpx_codec_control_(&ctx, ctrl_id, val);
    (void)res;

    for (int i = 0; known_ctrls[i] != 0; i++) {
        int fuzz_val = (int)(data[(i % size) + 4]);
        res = vpx_codec_control_(&ctx, known_ctrls[i], fuzz_val);
        (void)res;
    }

    vpx_codec_destroy(&ctx);
    return 1;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    try_fuzz_encoder(Data, Size);
    try_fuzz_decoder(Data, Size);

    return 0;
}