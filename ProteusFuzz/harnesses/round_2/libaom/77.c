// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

/* Helper to safely initialize encoder context */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzz data to randomize config slightly, with strict bounds checking */
    if (size >= 5) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

/* Helper to safely initialize decoder context */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (size >= 2) {
        cfg.threads = (data[0] % 8) + 1;
        cfg.allow_lowbitdepth = (data[1] % 2);
    }

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Test Encoder Control Functions */
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        /* AV1E_SET_ENABLE_INTRABC */
        int enable_intrabc = Data[0] % 2;
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

        /* AV1E_SET_LOOPFILTER_CONTROL */
        struct {
            int level;
            int sharpness;
            int delta_enabled;
            int delta_update;
            int delta_ref[8];
            int delta_mode[8];
        } lf_ctrl;
        memset(&lf_ctrl, 0, sizeof(lf_ctrl));
        if (Size > 5) {
            lf_ctrl.level = Data[1] % 64;
            lf_ctrl.sharpness = Data[2] % 8;
            lf_ctrl.delta_enabled = (Data[3] % 2) != 0;
            lf_ctrl.delta_update = (Data[4] % 2) != 0;
            lf_ctrl.delta_ref[0] = (int8_t)(Data[5] - 64);
        }
        aom_codec_control(enc_ctx, AV1E_SET_LOOPFILTER_CONTROL, &lf_ctrl);

        /* AV1E_SET_SVC_PARAMS */
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        if (Size > 10) {
            svc_params.number_spatial_layers = (Data[6] % 3) + 1;
            svc_params.number_temporal_layers = (Data[7] % 3) + 1;
            for (int i = 0; i < AOM_MAX_LAYERS && (6 + i + 4) < Size; i++) {
                svc_params.max_quantizers[i] = (Data[8 + i] % 64) + 16;
                svc_params.min_quantizers[i] = (Data[9 + i] % 64) + 16;
            }
            if (Size > 10) {
                svc_params.layer_target_bitrate[0] = (Data[10] % 1000) + 100;
            }
        }
        aom_codec_control(enc_ctx, AV1E_SET_SVC_PARAMS, &svc_params);

        /* AV1E_SET_SKIP_POSTPROC_FILTERING */
        int skip_postproc = (Size > 11) ? (Data[11] % 2) : 0;
        aom_codec_control(enc_ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);

        /* Cleanup Encoder */
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }

    /* Test Decoder Control Functions */
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        /* AV1_SET_SKIP_LOOP_FILTER */
        int skip_loop_filter = (Size > 12) ? (Data[12] % 2) : 0;
        aom_codec_control(dec_ctx, AV1_SET_SKIP_LOOP_FILTER, skip_loop_filter);

        /* AOME_GET_LOOPFILTER_LEVEL */
        int loopfilter_level = 0;
        aom_codec_control(dec_ctx, AOME_GET_LOOPFILTER_LEVEL, &loopfilter_level);

        /* Cleanup Decoder */
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    return 0;
}