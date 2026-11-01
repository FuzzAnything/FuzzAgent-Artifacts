// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec = vpx_codec_vp8_cx();
    if (!codec) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;

    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where possible, ensuring valid dimensions
    int w = (size > 0) ? data[0] : 16;
    int h = (size > 1) ? data[1] : 16;
    
    // Ensure dimensions are non-zero and reasonable to prevent allocation issues
    if (w < 1) w = 16;
    if (h < 1) h = 16;
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;

    cfg.g_w = w;
    cfg.g_h = h;
    
    if (size > 2) cfg.g_timebase.num = data[2] ? data[2] : 1;
    if (size > 3) cfg.g_timebase.den = data[3] ? data[3] : 1;
    if (size > 4) cfg.g_lag_in_frames = data[4];
    if (size > 5) cfg.rc_target_bitrate = data[5] * 100;
    if (size > 6) cfg.g_error_resilient = data[6] & 1;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy image
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) == NULL) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Initialize image planes with zeros using correct sizes based on stride and height
    // Plane 0 (Y): stride * height
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    // Plane 1 (U) and Plane 2 (V): stride * (height + 1) / 2
    // Using the actual height of the image (img.h) ensures we don't overflow
    int chroma_h = (img.h + 1) / 2;
    memset(img.planes[1], 0, img.stride[1] * chroma_h);
    memset(img.planes[2], 0, img.stride[2] * chroma_h);

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (size > 7) {
        int val = data[7];
        vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, val);
    }

    // Fuzz VP8E_SET_STATIC_THRESHOLD
    if (size > 8) {
        int val = data[8];
        vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, val);
    }

    // Fuzz VP8E_SET_GF_CBR_BOOST_PCT
    if (size > 9) {
        int val = data[9];
        vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, val);
    }

    // Fuzz VP8E_SET_CPUUSED
    if (size > 10) {
        int val = data[10] % 9; // Valid range 0-8
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val);
    }

    // Encode a frame
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;

    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where possible, ensuring valid dimensions
    int w = (size > 0) ? data[0] : 16;
    int h = (size > 1) ? data[1] : 16;
    
    // Ensure dimensions are non-zero and reasonable to prevent allocation issues
    if (w < 1) w = 16;
    if (h < 1) h = 16;
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;

    cfg.g_w = w;
    cfg.g_h = h;
    
    if (size > 2) cfg.g_timebase.num = data[2] ? data[2] : 1;
    if (size > 3) cfg.g_timebase.den = data[3] ? data[3] : 1;
    if (size > 4) cfg.g_lag_in_frames = data[4];
    if (size > 5) cfg.rc_target_bitrate = data[5] * 100;
    if (size > 6) cfg.g_error_resilient = data[6] & 1;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy image using vpx_img_alloc to handle memory correctly
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) == NULL) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Initialize image planes with zeros using correct sizes based on stride and height
    // Plane 0 (Y): stride * height
    memset(img.planes[0], 0, img.stride[0] * img.h);
    // Plane 1 (U) and Plane 2 (V): stride * (height + 1) / 2
    int chroma_h = (img.h + 1) / 2;
    memset(img.planes[1], 0, img.stride[1] * chroma_h);
    memset(img.planes[2], 0, img.stride[2] * chroma_h);

    // Fuzz VP9E_SET_GF_CBR_BOOST_PCT
    if (size > 7) {
        int val = data[7];
        vpx_codec_control(&ctx, VP9E_SET_GF_CBR_BOOST_PCT, val);
    }

    // Fuzz VP9E_SET_POSTENCODE_DROP
    if (size > 8) {
        int val = data[8] & 1; // Boolean value
        vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, val);
    }

    // Encode a frame
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Alternate between VP8 and VP9 based on the first byte
    if (Data[0] & 1) {
        fuzz_vp8_encoder(Data, Size);
    } else {
        fuzz_vp9_encoder(Data, Size);
    }

    return 0;
}