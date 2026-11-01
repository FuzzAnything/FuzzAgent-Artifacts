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

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get the AV1 encoder interface using the correct function
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure some basic parameters safely
    cfg.g_w = (Data[0] % 128) + 16; // Width between 16 and 143
    cfg.g_h = (Data[1] % 128) + 16; // Height between 16 and 143
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AOME_SET_CQ_LEVEL
    int cq_level = (Data[2] % 64); // Valid range 0-63
    ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    int enable_tpl_model = (Data[3] % 2); // 0 or 1
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, enable_tpl_model);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
    int enable_filter_intra = (Data[4] % 2); // 0 or 1
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra);
    (void)ret;

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    int frame_periodic_boost = (Data[5] % 100); // 0-99
    ret = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, frame_periodic_boost);
    (void)ret;

    // Fuzz AV1E_SET_MAX_GF_INTERVAL
    int max_gf_interval = (Data[6] % 8) + 1; // 1-8
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, max_gf_interval);
    (void)ret;

    // Fuzz AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    int gf_min_pyramid_height = (Data[7] % 4); // 0-3
    ret = aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, gf_min_pyramid_height);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}