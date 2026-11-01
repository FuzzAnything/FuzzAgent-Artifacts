// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static aom_codec_ctx_t *init_codec(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_image_t *img;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_iter_t iter = NULL;
    int width, height;
    int i;

    if (size < 4) {
        return NULL;
    }

    width = (data[0] << 8) | data[1];
    height = (data[2] << 8) | data[3];

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        return NULL;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    codec = malloc(sizeof(aom_codec_ctx_t));
    if (!codec) {
        return NULL;
    }
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) {
        aom_codec_destroy(codec);
        free(codec);
        return NULL;
    }

    for (i = 0; i < 3 && size > 0; i++) {
        size_t plane_size = (width * height) >> (i == 0 ? 0 : 2);
        size_t copy_size = size < plane_size ? size : plane_size;
        memcpy(img->planes[i], data, copy_size);
        data += copy_size;
        size -= copy_size;
    }

    ret = aom_codec_encode(codec, img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(codec);
        free(codec);
        return NULL;
    }

    while ((pkt = aom_codec_get_cx_data(codec, &iter))) {
        // Process packet if needed
    }

    aom_img_free(img);
    return codec;
}

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_203(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *codec;
    aom_codec_err_t ret;
    int value;

    if (Size < 6) {
        return 0;
    }

    codec = init_codec(Data, Size);
    if (!codec) {
        return 0;
    }

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    value = Data[0] & 1;
    ret = aom_codec_control(codec, AV1E_SET_INTRA_DCT_ONLY, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_TX64
    value = (Data[1] & 1);
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_TX64, value);
    (void)ret;

    // Fuzz AV1E_SET_COLOR_RANGE
    value = Data[2] & 1;
    ret = aom_codec_control(codec, AV1E_SET_COLOR_RANGE, value);
    (void)ret;

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    value = Data[3] % 9; // Assuming valid range 0-8
    ret = aom_codec_control(codec, AV1E_SET_MATRIX_COEFFICIENTS, value);
    (void)ret;

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    value = (Data[4] % 10); // Assuming valid range 0-9
    ret = aom_codec_control(codec, AV1E_SET_COLOR_PRIMARIES, value);
    (void)ret;

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    value = (Data[5] % 10); // Assuming valid range 0-9
    ret = aom_codec_control(codec, AV1E_SET_TRANSFER_CHARACTERISTICS, value);
    (void)ret;

    cleanup_codec(codec);
    return 0;
}