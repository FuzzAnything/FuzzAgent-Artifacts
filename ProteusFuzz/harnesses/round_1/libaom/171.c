// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (ctx == NULL || cfg == NULL) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzzing data to set configuration parameters safely
    if (size >= 8) {
        uint32_t width = *(uint32_t *)data;
        uint32_t height = *(uint32_t *)(data + 4);
        // Ensure dimensions are valid and within reasonable bounds
        if (width > 0 && width <= 4096) cfg->g_w = width;
        if (height > 0 && height <= 4096) cfg->g_h = height;
        
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_threads = 1;
        cfg->rc_target_bitrate = 1000;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // Fuzz various control commands
    int val1, val2, val3, val4, val5, val6;
    
    // Ensure we have enough data for 6 int values (24 bytes)
    if (Size >= 32) {
        val1 = *(int32_t *)(Data + 8);
        val2 = *(int32_t *)(Data + 12);
        val3 = *(int32_t *)(Data + 16);
        val4 = *(int32_t *)(Data + 20);
        val5 = *(int32_t *)(Data + 24);
        val6 = *(int32_t *)(Data + 28);
    } else {
        val1 = val2 = val3 = val4 = val5 = val6 = 0;
    }

    // AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    aom_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val1);

    // AV1E_SET_MIN_GF_INTERVAL
    aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val2);

    // AV1E_SET_MAX_GF_INTERVAL
    aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val3);

    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val4);

    // AOME_SET_CQ_LEVEL
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, val5);

    // AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6);

    cleanup_encoder(&ctx);

    return 0;
}