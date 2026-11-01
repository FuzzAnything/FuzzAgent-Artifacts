// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include "aom/aom_integer.h"
#include "aom/aomcx.h"

// Helper to safely call control functions
static int safe_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int val;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get encoder interface using the correct function name from aomcx.h
    // aom_codec_av1_cx() returns the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Set basic config from fuzz data
    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Extract a value from the fuzz input to use for control parameters
    val = (Size > 2) ? (int)(Data[2] & 0xFF) : 0;

    // 1. AOME_SET_ENABLEAUTOALTREF
    #ifdef AOME_SET_ENABLEAUTOALTREF
    safe_control(&ctx, AOME_SET_ENABLEAUTOALTREF, val);
    #endif

    // 2. AV1E_SET_MAX_REFERENCE_FRAMES
    #ifdef AV1E_SET_MAX_REFERENCE_FRAMES
    safe_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val);
    #endif

    // 3. AV1E_SET_S_FRAME_MODE
    #ifdef AV1E_SET_S_FRAME_MODE
    safe_control(&ctx, AV1E_SET_S_FRAME_MODE, val);
    #endif

    // 4. AV1E_SET_RTC_EXTERNAL_RC
    #ifdef AV1E_SET_RTC_EXTERNAL_RC
    safe_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val);
    #endif

    // 5. AOME_SET_ENABLEAUTOBWDREF
    #ifdef AOME_SET_ENABLEAUTOBWDREF
    safe_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val);
    #endif

    // 6. AOME_USE_REFERENCE
    #ifdef AOME_USE_REFERENCE
    safe_control(&ctx, AOME_USE_REFERENCE, val);
    #endif

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}