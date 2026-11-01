// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_err_t err;
    aom_codec_enc_cfg_t cfg;

    if (!iface) return NULL;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return NULL;

    // Use fuzz input to randomize some config parameters within safe bounds
    if (size >= 1) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (size > 1 ? data[1] % 1024 : 16) + 16;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    codec = malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;

    err = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *codec = init_encoder(Data, Size);
    if (!codec) return 0;

    // Try AV1E_SET_INTER_DCT_ONLY
    int inter_dct_only = (Data[0] % 2);
    aom_codec_control(codec, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    // Try AV1E_SET_ENABLE_INTERINTER_WEDGE
    int enable_interinter_wedge = (Size > 1 ? Data[1] % 2 : 0);
    aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_interinter_wedge);

    // Try AV1E_GET_ACTIVEMAP
    aom_active_map_t active_map;
    memset(&active_map, 0, sizeof(active_map));
    aom_codec_control(codec, AV1E_GET_ACTIVEMAP, &active_map);

    // Try AV1E_SET_ENABLE_AB_PARTITIONS
    int enable_ab_partitions = (Size > 2 ? Data[2] % 2 : 0);
    aom_codec_control(codec, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab_partitions);

    // Try AV1E_SET_TUNE_CONTENT
    // Assuming it takes an integer enum, we map input to a small range
    int tune_content = (Size > 3 ? Data[3] % 3 : 0); // 0, 1, or 2
    aom_codec_control(codec, AV1E_SET_TUNE_CONTENT, tune_content);

    // Try AOME_SET_ACTIVEMAP
    // We need to allocate a dummy active map
    unsigned int rows = (Size > 4 ? Data[4] % 64 : 1) + 1;
    unsigned int cols = (Size > 5 ? Data[5] % 64 : 1) + 1;
    active_map.rows = rows;
    active_map.cols = cols;
    active_map.active_map = malloc(rows * cols);
    if (active_map.active_map) {
        unsigned char val = (Size > 6 ? Data[6] % 2 : 0);
        memset(active_map.active_map, val, rows * cols);
        aom_codec_control(codec, AOME_SET_ACTIVEMAP, &active_map);
        free(active_map.active_map);
    }

    cleanup_encoder(codec);
    return 0;
}