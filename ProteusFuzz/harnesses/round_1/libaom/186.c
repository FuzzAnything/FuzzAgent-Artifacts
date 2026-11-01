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

int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use aom_codec_av1_cx() to get the AV1 encoder interface
    // Note: aom_codec_av1_cx takes no arguments
    const aom_codec_iface_t *iface = aom_codec_av1_cx(); 
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure width, height, and other parameters safely
    uint32_t width = (Data[0] % 256) + 16;
    uint32_t height = (Data[1] % 256) + 16;
    uint32_t fps_num = (Data[2] % 60) + 1;
    uint32_t fps_den = (Data[3] % 60) + 1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Helper to extract values from fuzzed data
    uint32_t val1 = (Size > 4) ? (Data[4] % 256) : 0;
    uint32_t val2 = (Size > 5) ? (Data[5] % 256) : 0;
    uint32_t val3 = (Size > 6) ? (Data[6] % 256) : 0;
    uint32_t val4 = (Size > 7) ? (Data[7] % 256) : 0;

    // 1. aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES
    aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, val1);

    // 2. aom_codec_control_typechecked_AV1E_SET_ALLOW_WARPED_MOTION
    aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val2);

    // 3. aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val3);

    // 4. aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS
    aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val4);

    // 5. aom_codec_control_typechecked_AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val1);

    // 6. aom_codec_control_typechecked_AOME_SET_CQ_LEVEL
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, val2);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}