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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int *value) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int control_id = 0;
    int value = 0;

    if (Size > 0) {
        control_id = Data[0] % 6;
        if (Size > 1) {
            value = (int)Data[1];
        }
    }

    int result = 0;
    switch (control_id) {
        case 0:
            result = fuzz_encoder_control(&ctx, AV1E_SET_DELTALF_MODE, &value);
            break;
        case 1:
            result = fuzz_encoder_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, &value);
            break;
        case 2:
            result = fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &value);
            break;
        case 3:
            result = fuzz_encoder_control(&ctx, AOME_SET_CQ_LEVEL, &value);
            break;
        case 4:
            result = fuzz_encoder_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, &value);
            break;
        case 5:
            result = fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, &value);
            break;
        default:
            break;
    }

    (void)result;

    aom_codec_destroy(&ctx);
    return 0;
}