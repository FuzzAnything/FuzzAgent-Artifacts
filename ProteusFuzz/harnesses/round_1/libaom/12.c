// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void init_fuzz_data(const uint8_t *Data, size_t Size, int *vals) {
    int i;
    memset(vals, 0, sizeof(int) * 6);
    
    if (Size == 0) return;

    vals[0] = (Size > 0) ? (Data[0] % 3) : 0;
    vals[1] = (Size > 1) ? (Data[1] % 2) : 0;
    vals[2] = (Size > 2) ? (Data[2] % 3) : 0;
    vals[3] = (Size > 3) ? (int)Data[3] : 0;
    vals[4] = (Size > 4) ? (int)Data[4] % 101 : 0;
    vals[5] = (Size > 5) ? (Data[5] % 5) : 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int vals[6];
    
    memset(&ctx, 0, sizeof(ctx));
    
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;
    
    res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    init_fuzz_data(Data, Size, vals);
    
    aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, vals[0]);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, vals[1]);
    aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, vals[2]);
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, vals[3]);
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, vals[4]);
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, vals[5]);
    
    if (Size > 6) {
        int val_rand = (int)Data[6] % 10;
        aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val_rand);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val_rand);
    }
    
    aom_codec_destroy(&ctx);
    
    return 0;
}