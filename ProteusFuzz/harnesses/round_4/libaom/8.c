// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t **ctx, int width, int height, int fps_num, int fps_den) {
    if (width <= 0 || height <= 0 || fps_num <= 0 || fps_den <= 0) {
        return 0;
    }

    aom_codec_ctx_t *c = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!c) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(c);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(c);
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_den;
    cfg.g_timebase.den = fps_num;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CBR;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(c, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(c);
        return 0;
    }

    *ctx = c;
    return 1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int fps_num = Data[2];
    int fps_den = Data[3];

    // Ensure reasonable bounds to avoid immediate crashes in initialization
    if (width > 4096 || height > 4096 || fps_num > 1000 || fps_den > 1000) {
        width = 128; height = 128; fps_num = 30; fps_den = 1;
    }
    if (width == 0) width = 128;
    if (height == 0) height = 128;
    if (fps_num == 0) fps_num = 30;
    if (fps_den == 0) fps_den = 1;

    aom_codec_ctx_t *ctx = NULL;
    if (!init_encoder(&ctx, width, height, fps_num, fps_den)) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_RESTORATION (0/1)
    if (Size > 4) {
        int val = Data[4] & 1;
        aom_codec_control(ctx, AV1E_SET_ENABLE_RESTORATION, val);
    }

    // Fuzz AV1E_SET_SUPERBLOCK_SIZE (0, 1, 2)
    if (Size > 5) {
        int val = Data[5] % 3;
        aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, val);
    }

    // Fuzz AV1E_SET_QM_U (0-255)
    if (Size > 6) {
        int val = Data[6];
        aom_codec_control(ctx, AV1E_SET_QM_U, val);
    }

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING (0/1)
    if (Size > 7) {
        int val = Data[7] & 1;
        aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
    }

    // Fuzz AV1E_SET_TILE_ROWS (0-15)
    if (Size > 8) {
        int val = Data[8] % 16;
        aom_codec_control(ctx, AV1E_SET_TILE_ROWS, val);
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC (0/1)
    if (Size > 9) {
        int val = Data[9] & 1;
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    cleanup_codec(ctx);
    return 0;
}