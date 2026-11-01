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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;
    if (size < 2) return 0;

    int val = (data[0] & 0xFF);
    int val2 = (data[1] & 0xFF);

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    
    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
    
    // AV1E_SET_ENABLE_GLOBAL_MOTION
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    
    // AV1E_SET_TRANSFER_CHARACTERISTICS
    aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
    
    // AOME_SET_MAX_INTRA_BITRATE_PCT
    aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val);
    
    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);

    return 0;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use data to set some configuration parameters safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 1000) + 100;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}