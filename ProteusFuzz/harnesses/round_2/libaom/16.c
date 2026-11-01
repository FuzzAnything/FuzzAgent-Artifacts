// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t ret;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_dx();

    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        
        ret = aom_codec_decode(ctx, data, size, NULL);
        if (ret == AOM_CODEC_OK) {
            while ((pkt = aom_codec_get_frame(ctx, &iter)) != NULL) {
                // Frame processed
            }
        }
    }

    // Try various control IDs if context is valid
    int ctrl_ids[] = {
        AV1_SET_DECODE_TILE_ROW,
        AV1_SET_DECODE_TILE_COL,
        AV1_INVERT_TILE_DECODE_ORDER,
        AV1_SET_TILE_MODE
    };
    int num_ctrls = sizeof(ctrl_ids) / sizeof(ctrl_ids[0]);
    int i;

    for (i = 0; i < num_ctrls; i++) {
        int val = (size > 0) ? (data[0] % 16) : 0;
        ret = aom_codec_control(ctx, ctrl_ids[i], val);
        (void)ret;
    }

    cleanup_codec(ctx);
    return 0;
}

static int fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Try various control IDs
    // Note: AV1E_SET_TILE_COLS is not a standard macro in libaom, using AV1E_SET_TILE_ROWS and others
    int valid_ctrl_ids[] = {
        AV1E_SET_TILE_ROWS,
        AOME_SET_CPUUSED,
        AOME_SET_CQ_LEVEL
    };
    int num_ctrls = sizeof(valid_ctrl_ids) / sizeof(valid_ctrl_ids[0]);
    int i;

    for (i = 0; i < num_ctrls; i++) {
        int val = (size > 0) ? (data[i % size] % 256) : 0;
        ret = aom_codec_control(ctx, valid_ctrl_ids[i], val);
        (void)ret;
    }

    cleanup_codec(ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz Decoder
    {
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));
        fuzz_decoder(&ctx, Data, Size);
    }

    // Fuzz Encoder
    {
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));
        fuzz_encoder(&ctx, Data, Size);
    }

    return 0;
}