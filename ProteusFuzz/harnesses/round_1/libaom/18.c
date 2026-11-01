// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_ENABLE_FLIP_IDTX
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_QM_V
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_QUANT_B_ADAPT
 * - AV1E_SET_ENABLE_ANGLE_DELTA
 */

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;
    cfg->g_lag_in_frames = 0;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    // Removed g_content_type as it may not exist in all versions
    // cfg->g_content_type = AOM_CONTENT_DEFAULT;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    int val1 = (Size > 0) ? (int)(Data[0] & 0xFF) : 0;
    int val2 = (Size > 1) ? (int)(Data[1] & 0xFF) : 0;
    int val3 = (Size > 2) ? (int)(Data[2] & 0xFF) : 0;
    int val4 = (Size > 3) ? (int)(Data[3] & 0xFF) : 0;
    int val5 = (Size > 4) ? (int)(Data[4] & 0xFF) : 0;
    int val6 = (Size > 5) ? (int)(Data[5] & 0xFF) : 0;

    // AV1E_SET_ENABLE_FLIP_IDTX
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val1);

    // AV1E_SET_INTER_DCT_ONLY
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val2);

    // AV1E_SET_QM_V
    aom_codec_control(&ctx, AV1E_SET_QM_V, val3);

    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val4);

    // AV1E_SET_QUANT_B_ADAPT
    aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, val5);

    // AV1E_SET_ENABLE_ANGLE_DELTA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val6);

    // Additional state exploration
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, 0);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, 1);
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, 0);
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, 0);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, 1);
    aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, 0);
    aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, 0);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, 1);

    cleanup_encoder(&ctx);

    return 0;
}