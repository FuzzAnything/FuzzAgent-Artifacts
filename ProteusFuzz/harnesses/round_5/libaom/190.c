// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

// Helper to safely initialize encoder context
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
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    aom_codec_err_t ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

// Helper to safely initialize decoder context
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
    // Initialize struct to zero to avoid undefined behavior with unused fields
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    // Removed invalid fields: allow_highbitdepth, allow_multiref, allow_screencontenttools
    // These fields do not exist in the provided aom_codec_dec_cfg_t definition.

    aom_codec_err_t ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
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

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_190(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide which function to test
    uint8_t choice = Data[0] % 6;

    switch (choice) {
        case 0: {
            // AV1E_SET_ROW_MT
            aom_codec_ctx_t *ctx = init_encoder(Data, Size);
            if (ctx) {
                int row_mt = (Data[1] % 4) + 1; // 1 to 4
                aom_codec_control(ctx, AV1E_SET_ROW_MT, row_mt);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 1: {
            // AV1D_SET_ROW_MT
            aom_codec_ctx_t *ctx = init_decoder(Data, Size);
            if (ctx) {
                int row_mt = (Data[1] % 4) + 1; // 1 to 4
                aom_codec_control(ctx, AV1D_SET_ROW_MT, row_mt);
                cleanup_decoder(ctx);
            }
            break;
        }
        case 2: {
            // AV1E_SET_ALLOW_WARPED_MOTION
            aom_codec_ctx_t *ctx = init_encoder(Data, Size);
            if (ctx) {
                int allow = (Data[1] % 2); // 0 or 1
                aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, allow);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 3: {
            // AV1E_SET_ENABLE_OBMC
            aom_codec_ctx_t *ctx = init_encoder(Data, Size);
            if (ctx) {
                int enable = (Data[1] % 2); // 0 or 1
                aom_codec_control(ctx, AV1E_SET_ENABLE_OBMC, enable);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 4: {
            // AV1E_SET_ENABLE_INTERINTRA_WEDGE
            aom_codec_ctx_t *ctx = init_encoder(Data, Size);
            if (ctx) {
                int enable = (Data[1] % 2); // 0 or 1
                aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable);
                cleanup_encoder(ctx);
            }
            break;
        }
        case 5: {
            // AOME_SET_SHARPNESS
            aom_codec_ctx_t *ctx = init_encoder(Data, Size);
            if (ctx) {
                int sharpness = Data[1] % 8; // 0 to 7
                aom_codec_control(ctx, AOME_SET_SHARPNESS, sharpness);
                cleanup_encoder(ctx);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}