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
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[0] + Data[1]) * 1000 + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = (int)(Data[0]);
    int val2 = (int)(Data[1]);
    int val3 = (int)(Data[0] + Data[1]);
    int val4 = (int)(Data[0] * 2);
    int val5 = (int)(Data[1] * 2);
    int val6 = (int)(Data[0] + Data[1] + 1);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRABC, val1);

    // Fuzz AV1E_SET_TILE_ROWS
    fuzz_encoder_control(&ctx, AV1E_SET_TILE_ROWS, val2);

    // Fuzz AV1E_SET_MAX_GF_INTERVAL
    fuzz_encoder_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val3);

    // Fuzz AOME_SET_CPUUSED
    fuzz_encoder_control(&ctx, AOME_SET_CPUUSED, val4);

    // Fuzz AV1E_SET_BITRATE_ONE_PASS_CBR
    fuzz_encoder_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, val5);

    // Fuzz AOME_SET_MAX_INTER_BITRATE_PCT
    // Note: Using the standard AOM control ID if the specific typechecked macro is not available
    // or if the identifier name in the prompt was a typo for AOME_SET_MAX_INTER_BITRATE_PCT
    // which might be AOME_SET_MAX_INTER_BITRATE_PCT in standard headers.
    // If the specific macro is missing, we rely on the standard definition or skip if undefined.
    // Assuming standard AOM control ID exists:
    #ifdef AOME_SET_MAX_INTER_BITRATE_PCT
    fuzz_encoder_control(&ctx, AOME_SET_MAX_INTER_BITRATE_PCT, val6);
    #endif

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}