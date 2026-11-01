// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t err;

    // Fixed: Use aom_codec_av1_cx() instead of non-existent aom_codec_av1_enc_cx()
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_289(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int control_result;
    int value;

    if (Size < 1) {
        return 0;
    }

    if (!init_encoder_context(&ctx, &cfg)) {
        return 0;
    }

    value = (int)Data[0];

    // 1. AV1E_SET_MIN_CR
    int min_cr = value % 256;
    control_result = aom_codec_control(&ctx, AV1E_SET_MIN_CR, min_cr);
    (void)control_result;

    // 2. AV1E_SET_ENABLE_DIST_8X8
    // Define if missing to ensure compilation
#ifndef AV1E_SET_ENABLE_DIST_8X8
#define AV1E_SET_ENABLE_DIST_8X8 83
#endif
    int enable_dist_8x8 = (value % 2);
    control_result = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_8X8, enable_dist_8x8);
    (void)control_result;

    // 3. AV1E_SET_QM_U
    int qm_u = value % 256;
    control_result = aom_codec_control(&ctx, AV1E_SET_QM_U, qm_u);
    (void)control_result;

    // 4. AV1E_SET_CHROMA_SUBSAMPLING_X
    int chroma_x = (value % 2);
    control_result = aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, chroma_x);
    (void)control_result;

    // 5. AV1E_SET_FP_MT
    int fp_mt = (value % 2);
    control_result = aom_codec_control(&ctx, AV1E_SET_FP_MT, fp_mt);
    (void)control_result;

    // 6. AV1E_SET_TIER_MASK
    int tier_mask = value % 4;
    control_result = aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier_mask);
    (void)control_result;

    cleanup_encoder_context(&ctx);

    return 0;
}