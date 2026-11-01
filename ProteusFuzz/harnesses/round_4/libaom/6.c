// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (!ctx) return 0;
    
    // Try setting the control with the provided value
    aom_codec_control(ctx, control_id, value);
    
    // Also try with 0 and 1 to explore different states
    aom_codec_control(ctx, control_id, 0);
    aom_codec_control(ctx, control_id, 1);
    
    return 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16; // Minimum 16
    cfg.g_h = (Size > 1 ? Data[1] % 256 : 1) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 2 ? Data[2] % 256 : 1) + 1;
    cfg.rc_target_bitrate = (Size > 3 ? Data[3] % 1000 : 1) + 100;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input data for control parameters
    int32_t val1 = (Size > 4) ? Data[4] : 0;
    int32_t val2 = (Size > 5) ? Data[5] : 0;
    int32_t val3 = (Size > 6) ? Data[6] : 0;
    int32_t val4 = (Size > 7) ? Data[7] : 0;
    int32_t val5 = (Size > 8) ? Data[8] : 0;
    int32_t val6 = (Size > 9) ? Data[9] : 0;

    // Fuzz the target control functions using correct enum names from aomcx.h
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_QM, val1);
    fuzz_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val2);
    fuzz_codec_control(&ctx, AV1E_SET_AQ_MODE, val3);
    fuzz_codec_control(&ctx, AV1E_SET_QM_MAX, val4);
    fuzz_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val5);
    fuzz_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}