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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int try_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, ctrl_id, value);
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices used below
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Safely use fuzz data with bounds checking
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    if (cfg.g_w < 16 || cfg.g_h < 16) {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Ensure we have at least 4 bytes before accessing Data[2] and Data[3]
    if (Size >= 4) {
        cfg.rc_target_bitrate = ((int)Data[2] * 256 + (int)Data[3]) * 1000;
        if (cfg.rc_target_bitrate == 0) cfg.rc_target_bitrate = 1000;
    } else {
        cfg.rc_target_bitrate = 1000;
    }

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz data for control parameters
    int val1 = (int)Data[4];
    int val2 = (int)Data[5];
    int val3 = (int)Data[6];
    int val4 = (int)Data[7];
    int val5 = (int)Data[8];
    int val6 = (int)Data[9];

    // Attempt to call the control functions via aom_codec_control
#ifdef AOME_SET_ENABLEAUTOBWDREF
    try_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val1);
#endif

#ifdef AV1E_SET_COEFF_COST_UPD_FREQ
    try_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val2);
#endif

#ifdef AV1E_SET_ENABLE_REF_FRAME_MVS
    try_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val3);
#endif

#ifdef AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    try_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val4);
#endif

#ifdef AV1E_SET_RTC_EXTERNAL_RC
    try_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val5);
#endif

#ifdef AV1E_SET_FRAME_PERIODIC_BOOST
    try_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val6);
#endif

    aom_codec_destroy(&ctx);

    return 0;
}