// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    // Correctly obtain the interface pointer first
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }

    // Pass the interface pointer, not the string name
    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 0;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

static void init_decoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, aom_codec_av1_dx(), &cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    int op = Data[0] % 6;
    int val = Data[1];

    // Prepare SVC params for specific test
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = 1;
    svc_params.number_temporal_layers = 1;

    switch (op) {
        case 0: // AV1E_SET_ENABLE_INTRABC
            init_encoder_context(&ctx, &cfg);
            if (ctx.iface != NULL) {
                aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
            }
            aom_codec_destroy(&ctx);
            break;

        case 1: // AV1E_SET_SKIP_POSTPROC_FILTERING
            init_encoder_context(&ctx, &cfg);
            if (ctx.iface != NULL) {
                aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
            }
            aom_codec_destroy(&ctx);
            break;

        case 2: // AV1E_SET_LOOPFILTER_CONTROL
            init_encoder_context(&ctx, &cfg);
            if (ctx.iface != NULL) {
                aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, val);
            }
            aom_codec_destroy(&ctx);
            break;

        case 3: // AV1_SET_SKIP_LOOP_FILTER (Decoder)
            init_decoder_context(&ctx);
            if (ctx.iface != NULL) {
                aom_codec_control(&ctx, AV1_SET_SKIP_LOOP_FILTER, val);
            }
            aom_codec_destroy(&ctx);
            break;

        case 4: // AOME_GET_LOOPFILTER_LEVEL
            init_encoder_context(&ctx, &cfg);
            if (ctx.iface != NULL) {
                int level;
                aom_codec_control(&ctx, AOME_GET_LOOPFILTER_LEVEL, &level);
            }
            aom_codec_destroy(&ctx);
            break;

        case 5: // AV1E_SET_SVC_PARAMS
            init_encoder_context(&ctx, &cfg);
            if (ctx.iface != NULL) {
                // Fuzz some fields of svc_params safely
                if (Size > 2) {
                    svc_params.number_spatial_layers = (Data[2] % 3) + 1;
                    svc_params.number_temporal_layers = (Data[2] % 3) + 1;
                }
                aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
            }
            aom_codec_destroy(&ctx);
            break;

        default:
            break;
    }

    return 0;
}