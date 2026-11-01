// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (!ctx) return 0;
    // Attempt to set the control value
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_enc_cfg_t cfg;
    // aom_codec_av1_cx() returns a pointer, so we pass it directly without &
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure basic parameters
    cfg.g_w = (Data[0] % 256) + 16; // Width
    cfg.g_h = (Data[1] % 256) + 16; // Height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;

    // aom_codec_av1_cx() returns a pointer directly
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz data for controls
    int32_t val1 = (int32_t)(Data[2] % 10); // For pyramid height
    int32_t val2 = (int32_t)(Data[3] % 3);  // For error resilient mode (0, 1, 2)
    int32_t val3 = (int32_t)(Data[4] % 2);  // For auto tiles (0, 1)
    int32_t val4 = (int32_t)(Data[5] % 100); // For max gf interval
    int32_t val5 = (int32_t)(Data[6] % 8);  // For tile rows
    int32_t val6 = (int32_t)(Data[7] % 10); // For min pyramid height

    // Invoke the target control functions using correct macro names
    // 1. AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    fuzz_encoder_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val1);

    // 2. AV1E_SET_ERROR_RESILIENT_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val2);

    // 3. AV1E_SET_AUTO_TILES
    fuzz_encoder_control(&ctx, AV1E_SET_AUTO_TILES, val3);

    // 4. AV1E_SET_MAX_GF_INTERVAL
    fuzz_encoder_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val4);

    // 5. AV1E_SET_TILE_ROWS
    fuzz_encoder_control(&ctx, AV1E_SET_TILE_ROWS, val5);

    // 6. AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    fuzz_encoder_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}