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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    // Try to set the control with the given value
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    
    // If the control failed, try with 0 and 1 to explore edge cases
    if (err != AOM_CODEC_OK) {
        aom_codec_control(ctx, control_id, 0);
        aom_codec_control(ctx, control_id, 1);
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_322(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface using the correct function name
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16; // Width
    cfg.g_h = (Data[1] % 256) + 16; // Height
    cfg.g_timebase.num = (Data[2] % 60) + 1;
    cfg.g_timebase.den = (Data[3] % 60) + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    // Initialize the encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Extract values from input data for control parameters
    int qm_max = (Size > 4) ? (int)(Data[4] % 256) : 0;
    int noise_sensitivity = (Size > 5) ? (int)(Data[5] % 256) : 0;
    int enable_rate_guide_deltaq = (Size > 6) ? (int)(Data[6] % 2) : 0;
    int s_frame_mode = (Size > 7) ? (int)(Data[7] % 4) : 0;
    int chroma_subsampling_y = (Size > 8) ? (int)(Data[8] % 3) : 0;
    int enable_auto_altref = (Size > 9) ? (int)(Data[9] % 2) : 0;

    // Fuzz the target API functions
    fuzz_encoder_control(&ctx, AV1E_SET_QM_MAX, qm_max);
    fuzz_encoder_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    fuzz_encoder_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, enable_rate_guide_deltaq);
    fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);
    fuzz_encoder_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, chroma_subsampling_y);
    fuzz_encoder_control(&ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_altref);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}