// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int try_control(aom_codec_ctx_t *ctx, int ctrl_id, void *arg) {
    aom_codec_err_t res = aom_codec_control(ctx, ctrl_id, arg);
    (void)res;
    return 0;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    cfg.g_w = Data[0] ? Data[0] : 64;
    cfg.g_h = Data[1] ? Data[1] : 64;
    cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    cfg.g_timebase.den = Data[3] ? Data[3] : 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 128, cfg.g_w * cfg.g_h / 4);
    memset(img.planes[2], 128, cfg.g_w * cfg.g_h / 4);

    aom_codec_encode(&ctx, &img, 0, 1, 0);

    unsigned int val = 0;
    if (Size > 4) {
        val = Data[4];
    }

    try_control(&ctx, AV1E_SET_INTER_DCT_ONLY, &val);
    try_control(&ctx, AV1_SET_TILE_MODE, &val);
    try_control(&ctx, AV1E_SET_ENABLE_QM, &val);
    try_control(&ctx, AV1E_SET_TILE_COLUMNS, &val);
    try_control(&ctx, AV1E_SET_QM_V, &val);
    try_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, &val);

    for (size_t i = 5; i < Size; i++) {
        val = Data[i];
        try_control(&ctx, AV1E_SET_INTER_DCT_ONLY, &val);
        try_control(&ctx, AV1E_SET_TILE_COLUMNS, &val);
        try_control(&ctx, AV1E_SET_ENABLE_QM, &val);
        try_control(&ctx, AV1E_SET_QM_V, &val);
        try_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, &val);
    }

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}