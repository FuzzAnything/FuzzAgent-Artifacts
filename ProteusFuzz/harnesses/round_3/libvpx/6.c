// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    
    if (!codec_iface) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Extract values from input data to diversify fuzzing
    int val1 = (int)Data[0];
    int val2 = (int)Data[1];
    int val3 = (int)Data[2];
    int val4 = (int)Data[3];
    int val5 = (int)Data[4];

    // Invoke VP9E_SET_FRAME_PERIODIC_BOOST
    vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val1);

    // Invoke VP9E_SET_NOISE_SENSITIVITY
    vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, val2);

    // Invoke VP8E_SET_STATIC_THRESHOLD
    vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, val3);

    // Invoke VP9E_SET_TUNE_CONTENT
    vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, val4);

    // Invoke VP9E_SET_TILE_COLUMNS
    vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, val5);

    vpx_codec_destroy(&ctx);
    return 0;
}