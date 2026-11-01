// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 0;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    int value;
    int ret;

    if (Size >= 1) {
        value = Data[0];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value);
        (void)ret;
    }

    if (Size >= 2) {
        value = Data[1];
        ret = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, value);
        (void)ret;
    }

    if (Size >= 3) {
        value = Data[2];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, value);
        (void)ret;
    }

    if (Size >= 4) {
        value = Data[3];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);
        (void)ret;
    }

    if (Size >= 5) {
        value = Data[4];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, value);
        (void)ret;
    }

    if (Size >= 6) {
        value = Data[5];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
        (void)ret;
    }

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    memset(img.planes[0], 128, 16 * 16);
    memset(img.planes[1], 128, 8 * 8);
    memset(img.planes[2], 128, 8 * 8);

    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);

    cleanup_codec(ctx);

    return 0;
}