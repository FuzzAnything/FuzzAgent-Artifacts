// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // Ensure we have enough data to read a value
    if (size < sizeof(int)) return 0;
    
    int value;
    memcpy(&value, data, sizeof(int));
    
    // Map the random value to a reasonable range (0 or 1 for booleans, or specific enum ranges)
    // For boolean flags, we typically use 0 or 1. For others, we might need to clamp.
    
    // 1. AV1E_SET_ENABLE_MASKED_COMP
    int val1 = (value & 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_MASKED_COMP, val1);
    
    // 2. AV1E_SET_ENABLE_SMOOTH_INTRA
    int val2 = (value & 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val2);
    
    // 3. AV1E_SET_ENABLE_DUAL_FILTER
    int val3 = (value & 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val3);
    
    // 4. AV1E_SET_ENABLE_INTERINTRA_WEDGE
    int val4 = (value & 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val4);
    
    // 5. AV1E_SET_TUNE_CONTENT
    // This typically takes an enum aom_tune_content_t. 
    // Valid values are usually AOM_CONTENT_DEFAULT, AOM_CONTENT_CG, AOM_CONTENT_GAME, etc.
    // We will try a few valid options based on bits.
    int tune_val = value % 4; // Try 0, 1, 2, 3
    if (tune_val > 3) tune_val = 0;
    aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, tune_val);
    
    // 6. AV1E_SET_ENABLE_FILTER_INTRA
    int val6 = (value & 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val6);

    return 0;
}

int LLVMFuzzerTestOneInput_273(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Set some basic config to ensure encoder initializes
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_usage = AOM_USAGE_REALTIME;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Perform the fuzzing on the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}