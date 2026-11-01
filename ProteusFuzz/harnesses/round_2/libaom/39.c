// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all the fields we access
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzzed data to configure encoder settings safely
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AOME_SET_CQ_LEVEL
    int cq_level = (Data[4] % 64);
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    int enable_tpl = (Data[5] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, enable_tpl);

    // Fuzz AOME_SET_SHARPNESS
    int sharpness = (Data[6] % 8);
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, sharpness);

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    int frame_boost = (Data[7] % 10);
    aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, frame_boost);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    int coeff_freq = (Data[8] % 100);
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_freq);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    int mode_freq = (Data[9] % 100);
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_freq);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}