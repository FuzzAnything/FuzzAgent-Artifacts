// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Try to set the control with the fuzzed value
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    
    // If the specific control failed, try with 0 or 1 as fallbacks to explore states
    if (ret != AOM_CODEC_OK) {
        if (value != 0) {
            aom_codec_control(ctx, control_id, 0);
        }
        if (value != 1) {
            aom_codec_control(ctx, control_id, 1);
        }
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int value;

    // Initialize codec context
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Parse fuzzed data for configuration safely
    cfg.g_w = (Data[0] % 1024) + 16; // Width between 16 and 1039
    cfg.g_h = (Data[1] % 1024) + 16; // Height between 16 and 1039
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) * 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Extract value from input data
    value = (Data[6] << 8) | Data[7];

    // Fuzz various control functions using correct enum names
    // AV1E_SET_ENABLE_DIAGONAL_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
    
    // AV1E_SET_NOISE_SENSITIVITY
    fuzz_encoder_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, value);
    
    // AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, value);
    
    // AV1E_SET_S_FRAME_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, value);
    
    // AV1E_SET_ENABLE_PAETH_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, value);
    
    // AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);

    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}