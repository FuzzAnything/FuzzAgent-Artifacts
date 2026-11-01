// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vp8dx.h"
#include "vp8cx.h"
#include "vpx_encoder.h"

// Define the control constants if they are not exposed in the included headers
#ifndef VP9E_SET_ALT_REF_AQ
#define VP9E_SET_ALT_REF_AQ 0x4000
#endif
#ifndef VP9E_SET_COLOR_RANGE
#define VP9E_SET_COLOR_RANGE 0x4001
#endif
#ifndef VP9E_SET_MAX_GF_INTERVAL
#define VP9E_SET_MAX_GF_INTERVAL 0x4002
#endif
#ifndef VP9E_SET_POSTENCODE_DROP
#define VP9E_SET_POSTENCODE_DROP 0x4003
#endif
#ifndef VP9E_SET_QUANTIZER_ONE_PASS
#define VP9E_SET_QUANTIZER_ONE_PASS 0x4004
#endif
#ifndef VP9E_SET_TARGET_LEVEL
#define VP9E_SET_TARGET_LEVEL 0x4005
#endif

static int fuzz_vp9_encoder_controls(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *encoder = vpx_codec_vp9_cx();
    if (!encoder) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 1000;

    if (vpx_codec_enc_init(&ctx, encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Generate dummy frame data if size is sufficient
    uint8_t *frame_data = NULL;
    size_t frame_size = 0;
    
    // Ensure we have enough data for a minimal frame or create one
    if (size > 100) {
        frame_data = (uint8_t *)malloc(size);
        if (!frame_data) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        memcpy(frame_data, data, size);
        frame_size = size;
    } else {
        // Create minimal valid frame (128x128 I420)
        // Y: 128*128, U: 64*64, V: 64*64
        size_t min_size = 128 * 128 + 64 * 64 + 64 * 64;
        frame_data = (uint8_t *)malloc(min_size);
        if (!frame_data) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        memset(frame_data, 0, min_size);
        frame_size = min_size;
    }

    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    // Wrap the image data
    if (vpx_img_wrap(&img, fmt, 128, 128, 1, frame_data) != 0) {
        free(frame_data);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz various control settings based on input data
    if (size > 0) {
        int value = (int)data[0];
        vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, value);
    }
    if (size > 1) {
        int value = (int)data[1];
        vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, value);
    }
    if (size > 2) {
        int value = (int)data[2];
        vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, value);
    }
    if (size > 3) {
        int value = (int)data[3];
        vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, value);
    }
    if (size > 4) {
        int value = (int)data[4];
        vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, value);
    }
    if (size > 5) {
        int value = (int)data[5];
        vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, value);
    }

    // Attempt to encode a frame to exercise the encoder state
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume packets
    }

    vpx_img_free(&img);
    free(frame_data);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a dummy file if needed (per instructions)
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    fuzz_vp9_encoder_controls(Data, Size);
    return 0;
}