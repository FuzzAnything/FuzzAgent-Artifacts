// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_SHARPNESS__value at vp8cx.h:990:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static int run_vp8_fuzz(vpx_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    if (vpx_codec_enc_init(codec, iface, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(codec);
        return 0;
    }

    // Fuzz VP8E_SET_ARNR_STRENGTH
    if (size >= 1) {
        int arnr_strength = data[0] % 10; // Assume valid range 0-9
        if (vpx_codec_control(codec, VP8E_SET_ARNR_STRENGTH, arnr_strength) != 0) {
            // Ignore errors for fuzzing
        }
    }

    // Fuzz VP8E_SET_SHARPNESS
    if (size >= 2) {
        int sharpness = (int8_t)data[1]; // Assume signed 8-bit
        if (vpx_codec_control(codec, VP8E_SET_SHARPNESS, sharpness) != 0) {
            // Ignore errors
        }
    }

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    if (size >= 3) {
        int screen_content_mode = data[2] % 2; // 0 or 1
        if (vpx_codec_control(codec, VP8E_SET_SCREEN_CONTENT_MODE, screen_content_mode) != 0) {
            // Ignore errors
        }
    }

    vpx_codec_destroy(codec);
    return 1;
}

static int run_vp9_fuzz(vpx_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (vpx_codec_enc_init(codec, iface, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(codec);
        return 0;
    }

    // Fuzz VP9E_SET_DISABLE_LOOPFILTER
    if (size >= 1) {
        int disable_loopfilter = data[0] % 2; // 0 or 1
        if (vpx_codec_control(codec, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter) != 0) {
            // Ignore errors
        }
    }

    // Fuzz VP9E_SET_FRAME_PARALLEL_DECODING
    if (size >= 2) {
        int frame_parallel_decoding = data[1] % 2; // 0 or 1
        if (vpx_codec_control(codec, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding) != 0) {
            // Ignore errors
        }
    }

    // Fuzz VP9E_SET_NOISE_SENSITIVITY
    if (size >= 3) {
        int noise_sensitivity = data[2] % 10; // Assume range 0-9
        if (vpx_codec_control(codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity) != 0) {
            // Ignore errors
        }
    }

    vpx_codec_destroy(codec);
    return 1;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Decide whether to test VP8 or VP9 based on input
    int test_vp8 = (Data[0] % 2) == 0;

    if (test_vp8) {
        vpx_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));
        run_vp8_fuzz(&codec, Data, Size);
    } else {
        vpx_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));
        run_vp9_fuzz(&codec, Data, Size);
    }

    return 0;
}