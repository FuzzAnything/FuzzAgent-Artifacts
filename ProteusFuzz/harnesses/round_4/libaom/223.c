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
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface using the standard API
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure basic settings
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] * 256 + Data[3]) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to drive control functions
    int val1 = Data[4] % 2;
    int val2 = Data[5] % 4;
    int val3 = Data[6] % 2;
    int val4 = Data[7] % 2;
    int val5 = Data[8] % 4;
    int val6 = Data[9] % 2;

    // Invoke target control functions with diverse inputs
    // 1. AOME_SET_ENABLEAUTOBWDREF
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val1);

    // 2. AV1E_SET_DELTAQ_MODE
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, val2);

    // 3. AV1E_SET_FORCE_VIDEO_MODE
    aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val3);

    // 4. AOME_SET_ENABLEAUTOALTREF
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, val4);

    // 5. AV1E_SET_S_FRAME_MODE
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val5);

    // 6. AV1E_SET_ENABLE_TPL_MODEL
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val6);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}