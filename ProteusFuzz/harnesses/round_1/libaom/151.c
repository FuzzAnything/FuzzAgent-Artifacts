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

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (iface == NULL) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic configuration from fuzz input to vary states
    // Ensure Size is sufficient before accessing Data array
    cfg.g_w = (Data[0] % 1024) + 16; // Width
    cfg.g_h = (Data[1] % 1024) + 16; // Height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = (Data[2] % 10000) + 100;
    cfg.g_pass = AOM_RC_ONE_PASS;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz input to control parameters
    int val1 = (int)(Data[1] & 0xFF);
    int val2 = (int)(Data[2] & 0xFF);
    int val3 = (int)(Data[3] & 0xFF);
    int val4 = (int)(Data[4] & 0xFF);
    int val5 = (int)(Data[5] & 0xFF);
    int val6 = (int)(Data[6] & 0xFF);

    // Invoke the controls with fuzzed values
    // AV1E_SET_QM_U
    aom_codec_control(&ctx, AV1E_SET_QM_U, val1);
    
    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val2);
    
    // AV1E_SET_QM_MIN
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val3);
    
    // AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val4);
    
    // AV1E_SET_MV_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val5);
    
    // AV1E_SET_MODE_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}