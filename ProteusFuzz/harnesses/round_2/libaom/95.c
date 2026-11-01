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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 4) return 0;

    // Extract a 32-bit value from the fuzz input for control parameters
    uint32_t val = 0;
    memcpy(&val, data, sizeof(val));
    
    // Mask to ensure reasonable range for specific controls if needed
    // AOME_SET_CQ_LEVEL
    aom_codec_control(ctx, AOME_SET_CQ_LEVEL, (val % 255) + 1);
    
    // AV1E_SET_QM_U
    aom_codec_control(ctx, AV1E_SET_QM_U, (val % 255) + 1);
    
    // AV1E_SET_CDF_UPDATE_MODE
    aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, (val % 3)); // 0, 1, or 2
    
    // AV1E_SET_AQ_MODE
    aom_codec_control(ctx, AV1E_SET_AQ_MODE, (val % 4)); // 0-3
    
    // AV1E_SET_MIN_GF_INTERVAL
    aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, (val % 100) + 1);
    
    // AV1E_SET_QM_Y
    aom_codec_control(ctx, AV1E_SET_QM_Y, (val % 255) + 1);

    return 1;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Configure basic settings from fuzz data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 256) + 1;
    cfg.rc_target_bitrate = (Data[3] * 256 + Data[4]) * 1000;
    cfg.g_threads = (Data[5] % 4) + 1;
    
    // Use valid members for AV1 specific settings if available in this version
    // cfg.g_row_mt and cfg.g_lossless are not standard in all versions, 
    // so we skip them or use AV1 specific control if needed.
    // For lossless, we can try to set it via control or ignore if not in struct.
    // cfg.g_lossless is typically handled via aom_codec_enc_cfg_t in newer versions 
    // or via control. Let's skip direct struct access if not present.

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Call fuzzable control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}