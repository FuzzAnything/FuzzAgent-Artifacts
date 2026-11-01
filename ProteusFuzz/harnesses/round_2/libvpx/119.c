// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static int fuzz_vp8e_set_noise_sensitivity(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) < 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;

    ret = vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0);
    if (ret < 0) {
        return 0;
    }

    if (size > 0) {
        int val = (int)data[0] % 10;
        ret = vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, val);
        (void)ret;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp8e_set_temporal_layer_id(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) < 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;

    ret = vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0);
    if (ret < 0) {
        return 0;
    }

    if (size > 0) {
        int val = (int)data[0] % 4;
        ret = vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, val);
        (void)ret;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp8e_set_activemap(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_active_map_t active_map;
    unsigned char *map_data = NULL;
    int ret;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) < 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;

    ret = vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0);
    if (ret < 0) {
        return 0;
    }

    if (size >= 4) {
        unsigned int rows = (data[0] % 10) + 1;
        unsigned int cols = (data[1] % 10) + 1;
        size_t map_size = rows * cols;

        if (size >= 4 + map_size) {
            map_data = malloc(map_size);
            if (map_data) {
                memcpy(map_data, data + 2, map_size);
                active_map.active_map = map_data;
                active_map.rows = rows;
                active_map.cols = cols;
                ret = vpx_codec_control(&ctx, VP8E_SET_ACTIVEMAP, &active_map);
                (void)ret;
                free(map_data);
            }
        }
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp8e_set_cpuused(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) < 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;

    ret = vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0);
    if (ret < 0) {
        return 0;
    }

    if (size > 0) {
        int val = (int)data[0] % 9;
        ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val);
        (void)ret;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp8e_set_frame_flags(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) < 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;

    ret = vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0);
    if (ret < 0) {
        return 0;
    }

    if (size > 0) {
        int val = (int)data[0];
        ret = vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, val);
        (void)ret;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    switch (Data[0] % 6) {
        case 0:
            fuzz_vp8e_set_noise_sensitivity(Data, Size);
            break;
        case 1:
            fuzz_vp8e_set_temporal_layer_id(Data, Size);
            break;
        case 2:
            fuzz_vp8e_set_activemap(Data, Size);
            break;
        case 3:
            fuzz_vp8e_set_cpuused(Data, Size);
            break;
        case 4:
            fuzz_vp8e_set_frame_flags(Data, Size);
            break;
        case 5:
            fuzz_vp8e_set_noise_sensitivity(Data, Size);
            fuzz_vp8e_set_temporal_layer_id(Data, Size);
            break;
    }

    return 0;
}