// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    uint8_t mode = Data[0] % 2; // 0 for encoder, 1 for decoder
    uint8_t cmd_index = Data[1] % 6; // Select one of the 6 commands

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;
    int ret = 0;

    if (mode == 0) {
        // Encoder setup
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        struct aom_codec_enc_cfg cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;

        res = aom_codec_enc_init(ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }
    } else {
        // Decoder setup
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        struct aom_codec_dec_cfg cfg;
        cfg.threads = 1;
        cfg.allow_lowbitdepth = 1;

        res = aom_codec_dec_init(ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }
    }

    // Prepare arguments based on command index
    int int_arg = 0;
    if (Size > 2) {
        int_arg = (int)(Data[2] & 0xFF);
    }

    switch (cmd_index) {
        case 0: // AOME_SET_SPATIAL_LAYER_ID
            if (mode == 0) {
                ret = aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, int_arg);
            }
            break;
        case 1: // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
            if (mode == 0) {
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, int_arg);
            }
            break;
        case 2: // AOME_SET_NUMBER_SPATIAL_LAYERS
            if (mode == 0) {
                ret = aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, int_arg);
            }
            break;
        case 3: // AV1E_SET_SVC_FRAME_DROP_MODE
            if (mode == 0) {
                ret = aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, int_arg);
            }
            break;
        case 4: // AV1E_SET_ENABLE_FILTER_INTRA
            if (mode == 0) {
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, int_arg);
            }
            break;
        case 5: // AV1D_SET_OPERATING_POINT
            if (mode == 1) {
                ret = aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, int_arg);
            }
            break;
        default:
            break;
    }

    cleanup_codec(ctx);
    return 0;
}