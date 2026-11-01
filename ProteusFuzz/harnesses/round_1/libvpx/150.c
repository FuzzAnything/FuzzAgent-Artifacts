// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP9E_SET_MAX_INTER_BITRATE_PCT__value at vp8cx.h:1014:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static int create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    unsigned char dummy_data[1024];
    memset(dummy_data, 0, sizeof(dummy_data));
    size_t written = fwrite(dummy_data, 1, sizeof(dummy_data), f);
    fclose(f);
    return (written == sizeof(dummy_data)) ? 0 : -1;
}

static int cleanup_dummy_file(const char *filename) {
    return remove(filename);
}

static vpx_codec_ctx_t *init_encoder(vpx_codec_iface_t *codec_iface, int width, int height) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_err_t err = vpx_codec_enc_init(ctx, codec_iface, NULL, 0);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    err = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_pass = 0;
    cfg.g_threads = 1;

    err = vpx_codec_enc_config_set(ctx, &cfg);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int value = (Data[2] << 8) | Data[3];

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }

    if (create_dummy_file("./dummy_file") != 0) {
        return 0;
    }

    vpx_codec_iface_t *codec_vp8 = vpx_codec_vp8_cx();
    vpx_codec_iface_t *codec_vp9 = vpx_codec_vp9_cx();

    if (!codec_vp8 || !codec_vp9) {
        cleanup_dummy_file("./dummy_file");
        return 0;
    }

    vpx_codec_ctx_t *ctx_vp8 = init_encoder(codec_vp8, width, height);
    vpx_codec_ctx_t *ctx_vp9 = init_encoder(codec_vp9, width, height);

    if (!ctx_vp8 || !ctx_vp9) {
        cleanup_encoder(ctx_vp8);
        cleanup_encoder(ctx_vp9);
        cleanup_dummy_file("./dummy_file");
        return 0;
    }

    // Fuzz VP8E_SET_NOISE_SENSITIVITY
    vpx_codec_control(ctx_vp8, VP8E_SET_NOISE_SENSITIVITY, value);

    // Fuzz VP8E_SET_ARNR_STRENGTH
    vpx_codec_control(ctx_vp8, VP8E_SET_ARNR_STRENGTH, value);

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    vpx_codec_control(ctx_vp8, VP8E_SET_MAX_INTRA_BITRATE_PCT, value);

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    vpx_codec_control(ctx_vp8, VP8E_SET_SCREEN_CONTENT_MODE, value);

    // Fuzz VP9E_SET_MAX_INTER_BITRATE_PCT
    vpx_codec_control(ctx_vp9, VP9E_SET_MAX_INTER_BITRATE_PCT, value);

    // Fuzz VP9E_SET_NOISE_SENSITIVITY
    vpx_codec_control(ctx_vp9, VP9E_SET_NOISE_SENSITIVITY, value);

    cleanup_encoder(ctx_vp8);
    cleanup_encoder(ctx_vp9);
    cleanup_dummy_file("./dummy_file");

    return 0;
}