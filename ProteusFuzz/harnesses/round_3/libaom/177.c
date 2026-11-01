// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure width/height if available, otherwise defaults
    if (size >= 4) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        if (w > 0 && w < 8192 && h > 0 && h < 8192) {
            cfg->g_w = w;
            cfg->g_h = h;
        }
    }
    // Ensure alignment
    cfg->g_w = (cfg->g_w + 15) & ~15;
    cfg->g_h = (cfg->g_h + 15) & ~15;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_177(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int initialized = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (Size < 4) {
        // Minimal data to attempt initialization with defaults
        initialized = init_encoder(&ctx, &cfg, Data, Size);
    } else {
        initialized = init_encoder(&ctx, &cfg, Data, Size);
    }

    if (!initialized) {
        return 0;
    }

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    if (Size > 0) {
        int interval = Data[0] * 10; // Scale up slightly
        err = aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, interval);
        (void)err; // Ignore error for fuzzing state exploration
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    if (Size > 1) {
        int enable = Data[1] % 2;
        err = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, enable);
        (void)err;
    }

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    if (Size > 2) {
        int enable = Data[2] % 2;
        err = aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable);
        (void)err;
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size > 3) {
        int enable = Data[3] % 2;
        err = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable);
        (void)err;
    }

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    if (Size > 4) {
        int pos = Data[4] % 3; // 0, 1, or 2
        err = aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, pos);
        (void)err;
    }

    // Fuzz AV1E_GET_BASELINE_GF_INTERVAL
    int gf_interval = 0;
    err = aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval);
    (void)err;
    (void)gf_interval;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}