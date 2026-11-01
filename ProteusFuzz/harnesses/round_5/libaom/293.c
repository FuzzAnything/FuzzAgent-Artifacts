// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    if (size < 2) return 0;

    // Use data to determine which control to invoke and what value to pass
    int cmd = data[0] % 6; // 0 to 5 for the 6 target functions
    int32_t val = 0;
    if (size >= 4) {
        val = *(int32_t *)(data + 1);
    } else if (size == 2) {
        val = data[1];
    }

    switch (cmd) {
        case 0: // AV1E_SET_ENABLE_INTRABC
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
            break;
        case 1: // AV1E_SET_QM_U
            ret = aom_codec_control(ctx, AV1E_SET_QM_U, val);
            break;
        case 2: // AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
            ret = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val);
            break;
        case 3: // AV1E_SET_GF_CBR_BOOST_PCT
            ret = aom_codec_control(ctx, AV1E_SET_GF_CBR_BOOST_PCT, val);
            break;
        case 4: // AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
            ret = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val);
            break;
        case 5: // AOME_SET_TUNING
            ret = aom_codec_control(ctx, AOME_SET_TUNING, val);
            break;
    }
    (void)ret; // Suppress unused warning
    return 0;
}

int LLVMFuzzerTestOneInput_293(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    // Initialize encoder config
    if (aom_codec_enc_config_default("av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config to make initialization more likely to succeed
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;

    // Initialize encoder context
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    iface = ctx.iface;
    if (!iface) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Perform fuzzing on control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}