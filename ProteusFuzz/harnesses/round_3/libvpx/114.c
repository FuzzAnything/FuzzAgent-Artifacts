// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9_SET_BYTE_ALIGNMENT__value at vp8dx.h:206:1 in vp8dx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *codec_interface = vpx_codec_vp9_cx();

    if (!codec_interface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(codec_interface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 160;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, codec_interface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Create a minimal frame
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    if (vpx_img_alloc(&img, fmt, 160, 128, 1) != NULL) {
        return 0;
    }

    // Fill image with fuzz data if available
    // Fixed: Changed img.plane to img.planes
    if (size > 0) {
        size_t plane_size = img.planes[0][0] * img.stride[0];
        size_t copy_size = (size < plane_size) ? size : plane_size;
        memcpy(img.planes[0], data, copy_size);
    }

    // Attempt to set various VP9 encoder controls using vpx_codec_control
    
    // VP9E_SET_NOISE_SENSITIVITY
    int noise_sens = (size > 0) ? data[0] % 100 : 0;
    vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sens);

    // VP9E_SET_POSTENCODE_DROP
    int post_encode_drop = (size > 1) ? data[1] % 2 : 0;
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, post_encode_drop);

    // VP9_SET_BYTE_ALIGNMENT
    int byte_align = (size > 2) ? data[2] % 2 : 0;
    vpx_codec_control(&ctx, VP9_SET_BYTE_ALIGNMENT, byte_align);

    // VP9E_SET_SVC_INTER_LAYER_PRED
    int svc_inter_layer = (size > 3) ? data[3] % 2 : 0;
    vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, svc_inter_layer);

    // VP9E_SET_FRAME_PARALLEL_DECODING
    int frame_parallel = (size > 4) ? data[4] % 2 : 0;
    vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel);

    // Encode a frame
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Fuzz the encoder
    fuzz_vp9_encoder(Data, Size);
    
    return 0;
}