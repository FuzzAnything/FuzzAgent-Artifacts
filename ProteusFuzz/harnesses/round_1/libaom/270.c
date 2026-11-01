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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_270(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid out-of-bounds access
    if (Size < 4) {
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
    
    // Use input data to configure encoder settings fuzzily
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 1000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions with input data
    int val = Data[3];

    // AV1E_SET_TRANSFER_CHARACTERISTICS
    aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);

    // AV1E_SET_ENABLE_DIST_WTD_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);

    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);

    // AV1E_SET_QUANT_B_ADAPT
    aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, val);

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);

    // AV1E_SET_TUNE_CONTENT
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}