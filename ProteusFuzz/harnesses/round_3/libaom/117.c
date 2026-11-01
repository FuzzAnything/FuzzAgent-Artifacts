// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int val;

    // Initialize configuration
    if (aom_codec_enc_config_default("aom-av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to set some configuration parameters
    cfg.g_w = (Size > 0) ? Data[0] : 128;
    cfg.g_h = (Size > 1) ? Data[1] : 128;
    cfg.g_timebase.num = (Size > 2) ? Data[2] : 1;
    cfg.g_timebase.den = (Size > 3) ? Data[3] : 30;
    cfg.g_threads = (Size > 4) ? Data[4] % 16 : 1;
    cfg.g_pass = 0;
    cfg.g_lag_in_frames = 0;

    // Initialize encoder context
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract a value from the input to use as parameter
    val = (Size > 5) ? (int)Data[5] : 0;

    // Fuzz AV1E_SET_DELTAQ_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, val);
    (void)ret;

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
    (void)ret;

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    ret = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val);
    (void)ret;

    // Fuzz AOME_SET_CQ_LEVEL
    ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, val);
    (void)ret;

    // Fuzz AOME_SET_ENABLEAUTOBWDREF
    ret = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}