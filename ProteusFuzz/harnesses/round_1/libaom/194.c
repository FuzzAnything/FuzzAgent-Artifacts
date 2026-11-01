// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

/* Fuzz driver for libaom control functions */

static aom_codec_ctx_t *init_encoder(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t *init_decoder(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 1;

    err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    uint8_t type = Data[0] % 6;
    uint8_t value = Data[1] % 256;
    int int_val = (int)(Data[1] % 100);

    switch (type) {
        case 0: {
            /* AV1E_SET_ENABLE_SUPERRES */
            aom_codec_ctx_t *ctx = init_encoder();
            if (ctx) {
                aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, value);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 1: {
            /* AV1E_SET_TIMING_INFO_TYPE */
            aom_codec_ctx_t *ctx = init_encoder();
            if (ctx) {
                aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, value);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 2: {
            /* AV1E_SET_COEFF_COST_UPD_FREQ */
            aom_codec_ctx_t *ctx = init_encoder();
            if (ctx) {
                aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, int_val);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 3: {
            /* AV1E_SET_S_FRAME_MODE */
            aom_codec_ctx_t *ctx = init_encoder();
            if (ctx) {
                aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, value);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 4: {
            /* AV1_SET_INSPECTION_CALLBACK */
            aom_codec_ctx_t *ctx = init_decoder();
            if (ctx) {
                aom_inspect_init inspect;
                inspect.inspect_cb = NULL;
                inspect.inspect_ctx = NULL;
                aom_codec_control(ctx, AV1_SET_INSPECTION_CALLBACK, &inspect);
                cleanup_decoder(ctx);
            }
            break;
        }
        case 5: {
            /* AV1E_SET_TARGET_SEQ_LEVEL_IDX */
            aom_codec_ctx_t *ctx = init_encoder();
            if (ctx) {
                aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, int_val);
                cleanup_encoder(ctx);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}