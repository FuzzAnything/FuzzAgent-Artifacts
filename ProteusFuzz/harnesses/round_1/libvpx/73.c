// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) return 0;

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&codec, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzzing various control settings
    if (size > 0 && data[0] % 2 == 0) {
        vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, (int)(data[0] & 0xFF));
    }
    if (size > 1 && data[1] % 3 == 0) {
        vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, (int)(data[1] & 0xFF));
    }
    if (size > 2 && data[2] % 4 == 0) {
        vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, (int)(data[2] & 0xFF));
    }
    if (size > 3 && data[3] % 5 == 0) {
        vpx_codec_control(&codec, VP8E_SET_SCREEN_CONTENT_MODE, (int)(data[3] & 0xFF));
    }

    // Encode a dummy frame
    vpx_image_t frame;
    vpx_img_alloc(&frame, VPX_IMG_FMT_I420, 16, 16, 1);
    
    // Fill with fuzzed data if available
    if (size > 0) {
        size_t copy_size = size < 16*16 ? size : 16*16;
        memcpy(frame.planes[0], data, copy_size);
    }

    vpx_codec_encode(&codec, &frame, 0, 1, 0, 0);

    vpx_codec_destroy(&codec);
    vpx_img_free(&frame);
    return 0;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_vp8_encoder(Data, Size);

    return 0;
}