// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_codec(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    aom_image_t img;

    if (size < 2) {
        return NULL;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    codec = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!codec) {
        return NULL;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    // Ensure dimensions are within safe bounds to prevent overflow
    uint32_t width = (data[0] % 128) + 16;
    uint32_t height = (data[1] % 128) + 16;
    
    // Cap dimensions to avoid huge allocations
    if (width > 1024 || height > 1024) {
        width = 128;
        height = 128;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    err = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        aom_codec_destroy(codec);
        free(codec);
        return NULL;
    }

    // Correctly calculate size based on stride and height for each plane
    // Plane 0 (Y): stride * height
    // Plane 1 (U): stride * (height/2)
    // Plane 2 (V): stride * (height/2)
    size_t y_size = (size_t)img.stride[0] * img.d_h;
    size_t uv_size = (size_t)img.stride[1] * (img.d_h / 2);

    memset(img.planes[0], 128, y_size);
    memset(img.planes[1], 128, uv_size);
    memset(img.planes[2], 128, uv_size);

    err = aom_codec_encode(codec, &img, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(codec);
        free(codec);
        return NULL;
    }

    aom_img_free(&img);
    return codec;
}

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_283(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t *codec = init_codec(Data, Size);
    if (!codec) {
        return 0;
    }

    int val1 = Data[0];
    int val2 = Data[1];
    int val3 = Data[2];
    int val4 = Data[3];
    int val5 = Data[4];
    int val6 = Data[5];

    aom_codec_control(codec, AV1E_SET_ENABLE_QM, val1);
    aom_codec_control(codec, AV1E_SET_FORCE_VIDEO_MODE, val2);
    aom_codec_control(codec, AV1E_SET_COEFF_COST_UPD_FREQ, val3);
    aom_codec_control(codec, AV1E_SET_DELTAQ_MODE, val4);
    aom_codec_control(codec, AOME_SET_ARNR_MAXFRAMES, val5);
    aom_codec_control(codec, AOME_SET_CQ_LEVEL, val6);

    aom_image_t img;
    // Use fixed small size for the second image to ensure safety
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, 128, 128, 1) != 0) {
        cleanup_codec(codec);
        return 0;
    }
    
    size_t y_size = (size_t)img.stride[0] * img.d_h;
    size_t uv_size = (size_t)img.stride[1] * (img.d_h / 2);

    memset(img.planes[0], 0x80, y_size);
    memset(img.planes[1], 0x80, uv_size);
    memset(img.planes[2], 0x80, uv_size);

    aom_codec_encode(codec, &img, 0, 1, 0);

    aom_codec_cx_pkt_t pkt;
    while (aom_codec_get_cx_data(codec, &pkt) == AOM_CODEC_OK) {
        // Process packet (do nothing)
    }

    aom_img_free(&img);
    cleanup_codec(codec);

    return 0;
}