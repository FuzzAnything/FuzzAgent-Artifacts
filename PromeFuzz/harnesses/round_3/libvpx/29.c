// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) {
        *size = 0;
        return 0;
    }
    uint32_t val;
    memcpy(&val, *data, 4);
    *data += 4;
    *size -= 4;
    return val;
}

static uint8_t read_byte(const uint8_t **data, size_t *size) {
    if (*size < 1) {
        *size = 0;
        return 0;
    }
    uint8_t val = **data;
    *data += 1;
    *size -= 1;
    return val;
}

static void try_control(vpx_codec_ctx_t *codec, int ctrl_id, uint32_t arg) {
    switch (ctrl_id) {
        case VP8E_SET_CPUUSED:
            vpx_codec_control(codec, VP8E_SET_CPUUSED, (int)arg);
            break;
        case VP8E_SET_ENABLEAUTOALTREF:
            vpx_codec_control(codec, VP8E_SET_ENABLEAUTOALTREF, (unsigned int)arg);
            break;
        case VP8E_SET_NOISE_SENSITIVITY:
            vpx_codec_control(codec, VP8E_SET_NOISE_SENSITIVITY, (unsigned int)arg);
            break;
        case VP8E_SET_SHARPNESS:
            vpx_codec_control(codec, VP8E_SET_SHARPNESS, (unsigned int)arg);
            break;
        case VP8E_SET_STATIC_THRESHOLD:
            vpx_codec_control(codec, VP8E_SET_STATIC_THRESHOLD, (unsigned int)arg);
            break;
        case VP8E_SET_TOKEN_PARTITIONS:
            vpx_codec_control(codec, VP8E_SET_TOKEN_PARTITIONS, (int)arg);
            break;
        case VP9E_SET_LOSSLESS:
            vpx_codec_control(codec, VP9E_SET_LOSSLESS, (unsigned int)arg);
            break;
        case VP9E_SET_TILE_COLUMNS:
            vpx_codec_control(codec, VP9E_SET_TILE_COLUMNS, (int)arg);
            break;
        case VP9E_SET_TILE_ROWS:
            vpx_codec_control(codec, VP9E_SET_TILE_ROWS, (int)arg);
            break;
        default:
            break;
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 13) return 0;

    const uint8_t *data = Data;
    size_t size = Size;

    uint32_t width = read_uint32(&data, &size) % 4096 + 1;
    uint32_t height = read_uint32(&data, &size) % 4096 + 1;
    uint32_t bitrate = read_uint32(&data, &size) % 1000000;

    uint8_t control_flags = read_byte(&data, &size);
    uint32_t control_args[8] = {0};
    for (int i = 0; i < 8; i++) {
        if (control_flags & (1 << i)) {
            control_args[i] = read_uint32(&data, &size);
        }
    }

    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = bitrate;

    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }

    int ctrl_ids[] = {
        VP8E_SET_CPUUSED,
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_NOISE_SENSITIVITY,
        VP8E_SET_SHARPNESS,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_TOKEN_PARTITIONS,
        VP9E_SET_LOSSLESS,
        VP9E_SET_TILE_COLUMNS
    };

    for (int i = 0; i < 8; i++) {
        if (control_flags & (1 << i)) {
            try_control(&codec, ctrl_ids[i], control_args[i]);
        }
    }

    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);

    vpx_codec_enc_cfg_t new_cfg = cfg;
    new_cfg.rc_target_bitrate = bitrate / 2 + 1;
    vpx_codec_enc_config_set(&codec, &new_cfg);

    size_t frame_data_size = size;
    const uint8_t *frame_data = data;
    size_t image_size = width * height * 3 / 2;

    uint8_t *img_buf = (uint8_t*)calloc(1, image_size);
    if (!img_buf) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    size_t first_frame_data_size = frame_data_size / 2;
    if (first_frame_data_size > image_size) first_frame_data_size = image_size;
    memcpy(img_buf, frame_data, first_frame_data_size);

    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 0);
    if (!img) {
        free(img_buf);
        vpx_codec_destroy(&codec);
        return 0;
    }

    memcpy(img->img_data, img_buf, image_size);

    vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);

    global_headers = vpx_codec_get_global_headers(&codec);

    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
    }

    size_t second_frame_data_size = frame_data_size - first_frame_data_size;
    if (second_frame_data_size > image_size) second_frame_data_size = image_size;
    memset(img_buf, 0, image_size);
    memcpy(img_buf, frame_data + first_frame_data_size, second_frame_data_size);
    memcpy(img->img_data, img_buf, image_size);

    vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);

    global_headers = vpx_codec_get_global_headers(&codec);

    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
    }

    vpx_img_free(img);
    free(img_buf);
    vpx_codec_destroy(&codec);

    return 0;
}