// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static int init_decoder(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    uint8_t mode = Data[0] % 2;
    int param = (int)(Data[0] % 100);

    if (mode == 0) {
        // Encoder path
        aom_codec_ctx_t ctx;
        aom_codec_enc_cfg_t cfg;
        memset(&ctx, 0, sizeof(ctx));
        memset(&cfg, 0, sizeof(cfg));

        if (init_encoder(&ctx, &cfg) != 0) return 0;

        // Try various encoder controls
        if (Data[0] % 2 == 0) {
            aom_codec_control(&ctx, AOME_SET_SHARPNESS, param);
        }
        if (Data[0] % 3 == 0) {
            aom_codec_control(&ctx, AV1E_SET_QM_U, param);
        }
        if (Data[0] % 4 == 0) {
            aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, param);
        }
        if (Data[0] % 5 == 0) {
            aom_codec_control(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, param);
        }
        if (Data[0] % 6 == 0) {
            aom_codec_control(&ctx, AV1E_SET_ROW_MT, param);
        }

        cleanup_encoder(&ctx);
    } else {
        // Decoder path
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));

        if (init_decoder(&ctx) != 0) return 0;

        // Try decoder controls
        if (Data[0] % 2 == 0) {
            aom_codec_control(&ctx, AV1D_SET_ROW_MT, param);
        }

        cleanup_decoder(&ctx);
    }

    return 0;
}