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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int *value) {
    if (!ctx) return 0;
    
    aom_codec_err_t res;
    res = aom_codec_control(ctx, control_id, value);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices up to 9
    if (Size < 10) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;

    aom_codec_err_t init_res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (init_res != AOM_CODEC_OK) {
        return 0;
    }

    int val;
    // Extract values from input for different controls safely
    val = (int)(Data[4] & 0xFF);
    fuzz_codec_control(&ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, &val);

    val = (int)(Data[5] & 0xFF);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, &val);

    val = (int)(Data[6] & 0xFF);
    fuzz_codec_control(&ctx, AV1E_SET_QM_MIN, &val);

    val = (int)(Data[7] & 0xFF);
    fuzz_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, &val);

    val = (int)(Data[8] & 0xFF);
    fuzz_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, &val);

    val = (int)(Data[9] & 0xFF);
    fuzz_codec_control(&ctx, AV1E_SET_MTU, &val);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}