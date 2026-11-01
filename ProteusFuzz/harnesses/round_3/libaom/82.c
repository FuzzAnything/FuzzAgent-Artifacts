// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 9) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface.
    // aom_codec_av1_cx() is defined in aom/aomcx.h
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to configure basic settings
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 100) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int enable_qm = Data[3] % 2;
    int qm_y = Data[4] % 100;
    int qm_v = Data[5] % 100;
    int aq_mode = Data[6] % 4;
    int qm_max = Data[7] % 100;
    int deltaq_strength = Data[8] % 10;

    // Use standard AV1 Encoder Control IDs defined in aom/aom_encoder.h
    // These correspond to the typechecked functions mentioned in the task.
    // AV1E_SET_ENABLE_QM
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm) != AOM_CODEC_OK) {}
    
    // AV1E_SET_QM_Y
    if (aom_codec_control(&ctx, AV1E_SET_QM_Y, qm_y) != AOM_CODEC_OK) {}
    
    // AV1E_SET_QM_V
    if (aom_codec_control(&ctx, AV1E_SET_QM_V, qm_v) != AOM_CODEC_OK) {}
    
    // AV1E_SET_AQ_MODE
    if (aom_codec_control(&ctx, AV1E_SET_AQ_MODE, aq_mode) != AOM_CODEC_OK) {}
    
    // AV1E_SET_QM_MAX
    if (aom_codec_control(&ctx, AV1E_SET_QM_MAX, qm_max) != AOM_CODEC_OK) {}
    
    // AV1E_SET_DELTAQ_STRENGTH
    if (aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength) != AOM_CODEC_OK) {}

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}