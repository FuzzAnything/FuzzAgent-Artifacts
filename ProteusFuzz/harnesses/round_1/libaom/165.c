// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * 2. AOME_SET_ARNR_STRENGTH
 * 3. AV1E_SET_DELTAQ_STRENGTH
 * 4. AV1E_SET_AQ_MODE
 * 5. AOME_SET_ARNR_MAXFRAMES
 * 6. AV1E_SET_MODE_COST_UPD_FREQ
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static void safe_control(aom_codec_ctx_t *c, int ctrl, int val) {
    aom_codec_err_t res = aom_codec_control(c, ctrl, val);
    (void)res; 
}

int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Parse input to determine dimensions and test parameters
    int width = (Data[0] % 15) + 16;
    int height = (Data[1] % 15) + 16;
    int param_int = (int)(Data[2] << 8 | Data[3]);
    int param_int2 = (int)(Data[4] << 8 | Data[5]);
    int param_int3 = (int)(Data[6] << 8 | Data[7]);

    // Ensure dimensions are not too large to avoid OOM in fuzzer
    if (width > 1024 || height > 1024) {
        width = 64;
        height = 64;
    }

    if (!init_encoder(&ctx, &cfg, width, height)) {
        return 0;
    }

    // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH (int)
    safe_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, param_int);

    // 2. AOME_SET_ARNR_STRENGTH (int)
    safe_control(&ctx, AOME_SET_ARNR_STRENGTH, param_int2);

    // 3. AV1E_SET_DELTAQ_STRENGTH (int)
    safe_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, param_int3);

    // 4. AV1E_SET_AQ_MODE (int)
    safe_control(&ctx, AV1E_SET_AQ_MODE, param_int);

    // 5. AOME_SET_ARNR_MAXFRAMES (int)
    safe_control(&ctx, AOME_SET_ARNR_MAXFRAMES, param_int2);

    // 6. AV1E_SET_MODE_COST_UPD_FREQ (int)
    safe_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, param_int3);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}