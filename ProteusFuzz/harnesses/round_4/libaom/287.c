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
#include <limits.h>

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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_287(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices used below
    if (Size < 8) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to configure basic settings safely
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_threads = (Data[2] % 8) + 1;
    cfg.rc_target_bitrate = (Data[3] * 256 + Data[4]) % 1000000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_SVC_REF_FRAME_COMP_PRED
    aom_svc_ref_frame_comp_pred_t comp_pred;
    comp_pred.use_comp_pred[0] = (Data[0] % 2);
    comp_pred.use_comp_pred[1] = (Data[1] % 2);
    comp_pred.use_comp_pred[2] = (Data[2] % 2);
    aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &comp_pred);

    // Fuzz AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
    int enable_mv_unit_test = (Data[3] % 2);
    aom_codec_control(&ctx, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, &enable_mv_unit_test);

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Create a dummy file as per instructions
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, "./dummy_file");
    }

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    int drop_mode = Data[4] % 3; // Assuming 3 modes exist
    aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &drop_mode);

    // Fuzz AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    int enable_low_complexity = (Data[5] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, &enable_low_complexity);

    // Fuzz AV1E_SET_FP_MT_UNIT_TEST
    int enable_fp_mt = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_FP_MT_UNIT_TEST, &enable_fp_mt);

    cleanup_codec(&ctx);
    return 0;
}