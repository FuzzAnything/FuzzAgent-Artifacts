// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int setup_encoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    if (!ctx || !iface) return 0;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    memset(&cfg, 0, sizeof(aom_codec_enc_cfg_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set minimal valid config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 30;
    cfg.g_timebase.den = 1;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_usage = AOM_CQ; // Use CQ mode

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_232(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    if (!setup_encoder(&ctx, iface)) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Extract data from input to drive the control parameters
    int32_t param_int = 0;
    int32_t param_int2 = 0;
    
    if (Size >= 4) {
        param_int = *(const int32_t *)(Data);
    }
    if (Size >= 8) {
        param_int2 = *(const int32_t *)(Data + 4);
    }

    // Convert to unsigned if needed
    unsigned int u_param = (unsigned int)param_int;
    unsigned int u_param2 = (unsigned int)param_int2;

    // Set CQ level after init using the correct macro name AOME_SET_CQ_LEVEL
    // Limit value to valid range [0, 255] for safety
    int cq_val = (param_int % 256);
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_val);

    // 1. AV1E_SET_ENABLE_SUPERRES (bool/int)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, u_param);

    // 2. AV1E_SET_ENABLE_FLIP_IDTX (bool/int)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, u_param);

    // 3. AV1E_SET_NOISE_SENSITIVITY (int)
    aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, param_int);

    // 4. AV1E_SET_ENABLE_DIRECTIONAL_INTRA (bool/int)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, u_param);

    // 5. AV1E_SET_RATE_DISTRIBUTION_INFO
    // This control expects a pointer to a struct. We pass a pointer to input data if size is sufficient.
    if (Size >= 16) {
        aom_codec_control(&ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, (void *)(Data + 4));
    }

    // 6. AV1E_SET_TARGET_SEQ_LEVEL_IDX (int)
    aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, param_int2);

    cleanup_encoder(&ctx);
    return 0;
}