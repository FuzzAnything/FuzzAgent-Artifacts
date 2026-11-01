// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_GET_ACTIVEMAP__value at vp8cx.h:1056:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
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
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_integer.h>

static int fuzz_vp8e_set_cpuused(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = 0;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;

    if (size > 0) {
        int val = data[0] % 9; // 0 to 8
        ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val);
    }

    vpx_codec_destroy(&ctx);
    return ret;
}

static int fuzz_vp9e_get_activemap(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_active_map_t amap;
    int ret = 0;

    if (vpx_codec_enc_config_default(&vpx_codec_vp9_cx, &cfg, 0) != 0) return 0;
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp9_cx, &cfg, 0) != 0) return 0;

    memset(&amap, 0, sizeof(amap));
    if (size >= 4) {
        amap.rows = (data[0] % 100) + 1;
        amap.cols = (data[1] % 100) + 1;
        amap.active_map = malloc(amap.rows * amap.cols);
        if (amap.active_map) {
            if (size > 2) {
                memcpy(amap.active_map, data + 2, size - 2);
            }
            ret = vpx_codec_control(&ctx, VP9E_GET_ACTIVEMAP, &amap);
            free(amap.active_map);
        }
    }

    vpx_codec_destroy(&ctx);
    return ret;
}

static int fuzz_vp8e_set_max_intra_bitrate_pct(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = 0;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;

    if (size > 0) {
        int val = data[0] % 101; // 0 to 100
        ret = vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, val);
    }

    vpx_codec_destroy(&ctx);
    return ret;
}

static int fuzz_vp8e_set_temporal_layer_id(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = 0;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;

    if (size > 0) {
        int val = data[0] % 4; // Assuming 0-3 for temporal layers
        ret = vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, val);
    }

    vpx_codec_destroy(&ctx);
    return ret;
}

static int fuzz_vp8e_set_activemap(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_active_map_t amap;
    int ret = 0;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;

    memset(&amap, 0, sizeof(amap));
    if (size >= 4) {
        amap.rows = (data[0] % 100) + 1;
        amap.cols = (data[1] % 100) + 1;
        amap.active_map = malloc(amap.rows * amap.cols);
        if (amap.active_map) {
            if (size > 2) {
                memcpy(amap.active_map, data + 2, size - 2);
            }
            ret = vpx_codec_control(&ctx, VP8E_SET_ACTIVEMAP, &amap);
            free(amap.active_map);
        }
    }

    vpx_codec_destroy(&ctx);
    return ret;
}

static int fuzz_vp8e_set_frame_flags(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = 0;

    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp8_cx, &cfg, 0) != 0) return 0;

    if (size > 0) {
        int flags = (int)data[0];
        ret = vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, flags);
    }

    vpx_codec_destroy(&ctx);
    return ret;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_vp8e_set_cpuused(Data, Size);
    fuzz_vp9e_get_activemap(Data, Size);
    fuzz_vp8e_set_max_intra_bitrate_pct(Data, Size);
    fuzz_vp8e_set_temporal_layer_id(Data, Size);
    fuzz_vp8e_set_activemap(Data, Size);
    fuzz_vp8e_set_frame_flags(Data, Size);

    return 0;
}