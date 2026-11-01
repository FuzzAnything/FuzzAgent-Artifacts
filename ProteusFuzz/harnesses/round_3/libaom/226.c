// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void setup_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return;

    // Initialize default configuration
    aom_codec_enc_config_default(NULL, cfg, 0);

    // Fuzz width and height from input data if size permits
    if (size >= 8) {
        uint32_t w = *(uint32_t *)data;
        uint32_t h = *(uint32_t *)(data + 4);
        if (w > 0 && w < 4096) cfg->g_w = w;
        if (h > 0 && h < 4096) cfg->g_h = h;
    } else {
        cfg->g_w = 32;
        cfg->g_h = 32;
    }

    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;
    // Fixed field names based on compiler error
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    // Initialize encoder context
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    setup_encoder_context(&ctx, &cfg, Data, Size);

    if (ctx.iface == NULL) {
        return 0;
    }

    // Extract control type and value from fuzz input
    uint32_t control_type = (Data[0] % 6) + 1; // 1 to 6 corresponding to the 6 APIs
    int32_t value = (int32_t)(Data[1] & 0xFF); // Simple boolean-like value

    int ret = 0;

    // Map control_type to actual AV1E control constants
    switch (control_type) {
        case 1:
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, value);
            break;
        case 2:
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, value);
            break;
        case 3:
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);
            break;
        case 4:
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);
            break;
        case 5:
            ret = aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, value);
            break;
        case 6:
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, value);
            break;
        default:
            break;
    }

    // Ignore return value for fuzzing stability, but could be used for coverage
    (void)ret;

    cleanup_encoder_context(&ctx);

    return 0;
}