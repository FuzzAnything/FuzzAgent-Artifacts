// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    vpx_image_t *img = NULL;
    vpx_codec_cx_pkt_t *pkt;
    vpx_codec_iter_t iter = NULL;

    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure encoder settings within valid ranges
    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    if (size > 5) cfg.g_lag_in_frames = data[5] % 10;

    err = vpx_codec_enc_init(&ctx, vpx_codec_vp8_cx(), &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Fuzz VP8E_SET_CPUUSED
    if (size > 6) {
        int cpu_used = data[6] % 9; // 0-8
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
    }

    // Fuzz VP8E_SET_TUNING
    if (size > 7) {
        int tuning = data[7] % 2; // 0 or 1
        vpx_codec_control(&ctx, VP8E_SET_TUNING, tuning);
    }

    // Create a dummy frame using vpx_img_alloc
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill planes with fuzzed data or zeros
    memset(img->planes[0], 0, cfg.g_w * cfg.g_h);
    memset(img->planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
    memset(img->planes[2], 128, (cfg.g_w * cfg.g_h) / 4);

    vpx_codec_encode(&ctx, img, 0, 1, 0, 0);

    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded frame
        }
    }

    vpx_img_free(img);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    vpx_image_t *img = NULL;
    vpx_codec_cx_pkt_t *pkt;
    vpx_codec_iter_t iter = NULL;

    if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 100;

    err = vpx_codec_enc_init(&ctx, vpx_codec_vp9_cx(), &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Fuzz VP9E_SET_TUNE_CONTENT
    if (size > 5) {
        int content_type = data[5] % 4; // 0-3
        vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, content_type);
    }

    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    memset(img->planes[0], 0, cfg.g_w * cfg.g_h);
    memset(img->planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
    memset(img->planes[2], 128, (cfg.g_w * cfg.g_h) / 4);

    vpx_codec_encode(&ctx, img, 0, 1, 0, 0);

    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded frame
        }
    }

    vpx_img_free(img);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide which codec to fuzz
    if (Data[0] % 2 == 0) {
        fuzz_vp8_encoder(Data, Size);
    } else {
        fuzz_vp9_encoder(Data, Size);
    }

    return 0;
}