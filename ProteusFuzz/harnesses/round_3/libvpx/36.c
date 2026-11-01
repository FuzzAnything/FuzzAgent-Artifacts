// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9D_SET_ROW_MT__value at vp8dx.h:216:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_interface = vpx_codec_vp9_cx();
    if (!codec_interface) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(codec_interface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data if possible
    if (size >= sizeof(int)) {
        int val = *(const int*)data;
        if (val > 0 && val <= 4096) cfg.g_w = val;
        if (size >= sizeof(int) * 2) {
            val = *(const int*)(data + sizeof(int));
            if (val > 0 && val <= 4096) cfg.g_h = val;
        }
    }

    if (vpx_codec_enc_init(&codec, codec_interface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Attempt to set various encoder controls using the provided macros
    int control_value = 0;
    if (size >= sizeof(int)) {
        control_value = *(const int*)data;
    }

    // VP9E_SET_ROW_MT__value
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, control_value);

    // VP9E_SET_MAX_GF_INTERVAL__value
    vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, control_value);

    // VP9E_SET_POSTENCODE_DROP__value
    vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, control_value);

    // VP9E_SET_LOSSLESS__value
    vpx_codec_control(&codec, VP9E_SET_LOSSLESS, control_value);

    // VP9E_SET_FRAME_PERIODIC_BOOST__value
    vpx_codec_control(&codec, VP9E_SET_FRAME_PERIODIC_BOOST, control_value);

    // Encode a dummy frame to exercise the state
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, cfg.g_w, cfg.g_h, 1);

    // Fill image with fuzzed data
    if (size > 0) {
        size_t img_size = (size_t)img.d_h * img.stride[0];
        size_t copy_size = (size < img_size) ? size : img_size;
        memcpy(img.planes[0], data, copy_size);
    }

    vpx_codec_encode(&codec, &img, 0, 1, 0, 0);

    vpx_codec_destroy(&codec);
    vpx_img_free(&img);
    return 0;
}

static int fuzz_decoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_interface = vpx_codec_vp9_dx();
    if (!codec_interface) return 0;

    vpx_codec_ctx_t codec;
    
    if (vpx_codec_dec_init(&codec, codec_interface, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Attempt to set decoder controls
    int control_value = 0;
    if (size >= sizeof(int)) {
        control_value = *(const int*)data;
    }

    // VP9D_SET_ROW_MT__value
    vpx_codec_control(&codec, VP9D_SET_ROW_MT, control_value);

    // Decode a dummy frame
    if (size > 0) {
        vpx_codec_decode(&codec, data, size, NULL, 0);
    }

    vpx_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_encoder(Data, Size);
    fuzz_decoder(Data, Size);

    return 0;
}