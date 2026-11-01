// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int get_int(const uint8_t *d, size_t s) {
    if (s < sizeof(int)) return 0;
    int val;
    memcpy(&val, d, sizeof(int));
    return val;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int idx = 0;
    int val;

    // 1. aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG
    if (idx + sizeof(int) <= Size) {
        val = get_int(Data + idx, Size - idx);
        idx += sizeof(int);
        aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS
    if (idx + sizeof(int) <= Size) {
        val = get_int(Data + idx, Size - idx);
        idx += sizeof(int);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
    }

    // 3. aom_codec_control_typechecked_AV1_SET_TILE_MODE
    // AV1_SET_TILE_MODE expects a pointer to aom_tile_info
    if (idx + sizeof(aom_tile_info) <= Size) {
        aom_tile_info tile_info;
        memset(&tile_info, 0, sizeof(tile_info));
        memcpy(&tile_info, Data + idx, sizeof(tile_info));
        idx += sizeof(aom_tile_info);
        aom_codec_control(&ctx, AV1_SET_TILE_MODE, &tile_info);
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING
    if (idx + sizeof(int) <= Size) {
        val = get_int(Data + idx, Size - idx);
        idx += sizeof(int);
        aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_TILE_ROWS
    if (idx + sizeof(int) <= Size) {
        val = get_int(Data + idx, Size - idx);
        idx += sizeof(int);
        aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS
    if (idx + sizeof(int) <= Size) {
        val = get_int(Data + idx, Size - idx);
        idx += sizeof(int);
        aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, val);
    }

    cleanup_codec(&ctx);
    return 0;
}