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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Ensure the codec is initialized before sending control commands
    if (ctx->priv == NULL) return 0;

    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    if (err != AOM_CODEC_OK) {
        // Non-fatal, just continue fuzzing
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_224(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (Size > 0) cfg.g_w = (Data[0] % 512) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 512) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    if (Size > 4) cfg.g_threads = (Data[4] % 8) + 1;
    if (Size > 5) cfg.rc_target_bitrate = (Data[5] % 10000) + 100;

    // Initialize encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control commands using correct identifiers
    int control_ids[] = {
        AV1E_SET_QUANT_B_ADAPT,
        AV1E_SET_ENABLE_INTRA_EDGE_FILTER,
        AV1E_SET_ALLOW_WARPED_MOTION,
        AV1E_SET_TUNE_CONTENT,
        AV1E_SET_ENABLE_DIST_WTD_COMP,
        AV1E_SET_ENABLE_RECT_TX
    };

    for (size_t i = 0; i < sizeof(control_ids) / sizeof(control_ids[0]); i++) {
        int value = (Size > i + 6) ? (Data[i + 6] % 2) : (Data[0] % 2);
        fuzz_encoder_control(&ctx, control_ids[i], value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}