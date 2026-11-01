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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 100;
    cfg.g_threads = (Data[4] % 8) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC
    // The control ID is defined as AV1E_SET_RTC_EXTERNAL_RC in aom_encoder.h
    int param1 = (int)(Data[5] % 256);
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, param1);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_FRAME_PERIODIC_BOOST
    int param2 = (int)(Data[6] % 256);
    aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, param2);

    // Fuzz aom_codec_control_typechecked_AOME_USE_REFERENCE
    int param3 = (int)(Data[7] % 256);
    aom_codec_control(&ctx, AOME_USE_REFERENCE, param3);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_ONESIDED_COMP
    int param4 = (int)(Data[8] % 256);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, param4);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS
    int param5 = (int)(Data[9] % 256);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, param5);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA
    int param6 = (int)(Data[10] % 256);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, param6);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}