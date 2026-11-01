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

#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

static int fuzzer_run_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    int control_index = Data[0] % 6;
    int value = Data[1];

    int ret = AOM_CODEC_OK;

    switch (control_index) {
        case 0:
            ret = fuzzer_run_encoder_control(&ctx, AV1E_SET_INTER_DCT_ONLY, value);
            break;
        case 1:
            ret = fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, value);
            break;
        case 2:
            ret = fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value);
            break;
        case 3:
            ret = fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);
            break;
        case 4:
            ret = fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value);
            break;
        case 5:
            ret = fuzzer_run_encoder_control(&ctx, AV1E_SET_TUNE_CONTENT, value);
            break;
        default:
            break;
    }

    aom_codec_destroy(&ctx);

    return 0;
}