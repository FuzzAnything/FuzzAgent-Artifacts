// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void try_control(aom_codec_ctx_t *ctx, int ctrl, int val) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, ctrl, val);
    (void)ret;
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (size < 4) return 0;

    uint32_t width = *(uint32_t *)data;
    uint32_t height = *(uint32_t *)(data + 4);
    
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    if ((iface = aom_codec_av1_cx()) == NULL) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = AOM_CBR;
    cfg->rc_target_bitrate = 100;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_201(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int initialized = 0;

    if (Size < 8) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }
    initialized = 1;

    int val1 = (Size > 0) ? Data[0] : 0;
    int val2 = (Size > 1) ? Data[1] : 0;
    int val3 = (Size > 2) ? Data[2] : 0;
    int val4 = (Size > 3) ? Data[3] : 0;
    int val5 = (Size > 4) ? Data[4] : 0;
    int val6 = (Size > 5) ? Data[5] : 0;

    try_control(&ctx, AV1E_SET_COLOR_PRIMARIES, val1 % 10);

    try_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val2 % 2);
    if (Size > 6) try_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val2 + 100);

    try_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val3 % 2);
    if (Size > 7) try_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val3 + 50);

    try_control(&ctx, AV1E_SET_ENABLE_SUPERRES, val4 % 2);
    if (Size > 8) try_control(&ctx, AV1E_SET_ENABLE_SUPERRES, val4 + 20);

    try_control(&ctx, AV1E_SET_TIMING_INFO_TYPE, val5 % 5);
    if (Size > 9) try_control(&ctx, AV1E_SET_TIMING_INFO_TYPE, val5 + 10);

    try_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val6 % 2);
    if (Size > 10) try_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val6 + 30);

    if (initialized) {
        cleanup_encoder(&ctx);
    }

    return 0;
}