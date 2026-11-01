// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_RTC_EXTERNAL_RATECTRL__value at vp8cx.h:1105:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static int fuzz_vp8_encoder_controls(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;
    vpx_codec_err_t err;
    vpx_codec_iface_t *iface;
    vpx_codec_iter_t iter = NULL;
    vpx_codec_cx_pkt_t *pkt;
    int cleanup_img = 0;

    // Get the VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input
    if (Size > 0) cfg.g_timebase.num = Data[0] ? Data[0] : 1;
    if (Size > 1) cfg.g_timebase.den = Data[1] ? Data[1] : 1000;
    if (Size > 2) cfg.g_w = (Data[2] % 1024) + 16;
    if (Size > 3) cfg.g_h = (Data[3] % 1024) + 16;
    if (Size > 4) cfg.g_threads = (Data[4] % 8) + 1;
    if (Size > 5) cfg.rc_target_bitrate = (Data[5] % 1000) + 100;
    if (Size > 6) cfg.g_error_resilient = Data[6] % 2;

    // Initialize encoder
    err = vpx_codec_enc_init(&encoder, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize image
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        vpx_codec_destroy(&encoder);
        return 0;
    }
    cleanup_img = 1;

    // Fill image with fuzz data
    memset(img.planes[0], Data[Size > 7 ? 7 : 0], cfg.g_w * cfg.g_h);
    memset(img.planes[1], Data[Size > 8 ? 8 : 0], (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], Data[Size > 9 ? 9 : 0], (cfg.g_w * cfg.g_h) / 4);

    // Prepare controls based on available data
    int ctrl_val = 0;
    if (Size > 10) ctrl_val = (int)Data[10];
    if (Size > 11) ctrl_val = (ctrl_val << 8) | Data[11];
    if (Size > 12) ctrl_val = (ctrl_val << 8) | Data[12];
    if (Size > 13) ctrl_val = (ctrl_val << 8) | Data[13];

    // Apply VP8E_SET_CPUUSED
    if (Size > 14) {
        int cpuused = Data[14] % 9; // 0-8
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, cpuused);
    }

    // Apply VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (Size > 15) {
        int max_intra = Data[15] % 101; // 0-100
        vpx_codec_control(&encoder, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra);
    }

    // Apply VP8E_SET_FRAME_FLAGS
    if (Size > 16) {
        int flags = Data[16] % 4; // Limited flags for safety
        vpx_codec_control(&encoder, VP8E_SET_FRAME_FLAGS, flags);
    }

    // Apply VP8E_SET_TEMPORAL_LAYER_ID
    if (Size > 17) {
        int layer_id = Data[17] % 4; // 0-3
        vpx_codec_control(&encoder, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);
    }

    // Apply VP8E_SET_RTC_EXTERNAL_RATECTRL (if supported)
    if (Size > 18) {
        int rate_ctrl = Data[18] % 2;
        vpx_codec_control(&encoder, VP8E_SET_RTC_EXTERNAL_RATECTRL, rate_ctrl);
    }

    // Encode frame
    err = vpx_codec_encode(&encoder, &img, 0, 1, 0, 0);
    if (err != VPX_CODEC_OK) {
        // Ensure cleanup even on error
        goto cleanup;
    }

    // Get and free encoded packets to ensure no leaks
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Data is managed by libvpx, but we must iterate to completion
        }
    }

cleanup:
    if (cleanup_img) {
        vpx_img_free(&img);
    }
    vpx_codec_destroy(&encoder);

    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    fuzz_vp8_encoder_controls(Data, Size);
    return 0;
}