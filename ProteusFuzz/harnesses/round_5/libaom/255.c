// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int *value) {
    if (ctx == NULL) return 0;
    
    // Ensure value pointer is valid before dereferencing
    if (value == NULL) return 0;
    
    // Try setting integer values
    int ret = aom_codec_control(ctx, control_id, *value);
    if (ret != AOM_CODEC_OK) return 0;
    
    // Try getting values (for GET controls)
    ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    
    return 1;
}

int LLVMFuzzerTestOneInput_255(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use fuzzing data to configure encoder
    cfg.g_w = (Data[0] % 256) + 16; // Width: 16-271
    cfg.g_h = (Data[1] % 256) + 16; // Height: 16-271
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Size > 2) ? (Data[2] * 1000) : 1000;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Prepare test values from input data
    int test_values[6];
    for (int i = 0; i < 6; i++) {
        test_values[i] = (Size > i + 2) ? Data[i + 2] : (i % 256);
    }
    
    // Fuzz the target control functions
    // 1. AV1E_SET_MIN_PARTITION_SIZE
    fuzz_encoder_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, &test_values[0]);
    
    // 2. AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, &test_values[1]);
    
    // 3. AV1E_GET_SEQ_LEVEL_IDX - Only try to GET, not SET
    // For GET operations, we need to ensure the encoder is in a valid state
    int dummy_val = 0;
    aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &dummy_val);
    
    // 4. AV1E_SET_DENOISE_NOISE_LEVEL
    fuzz_encoder_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, &test_values[3]);
    
    // 5. AV1E_SET_ENABLE_SUPERRES
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_SUPERRES, &test_values[4]);
    
    // 6. AV1E_SET_ENABLE_DNL_DENOISING
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, &test_values[5]);
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}