// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    // Use fuzz data to tweak config slightly if size is sufficient
    // Ensure we have enough data before accessing indices
    if (size >= 3) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = (data[1] % 512) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = (data[2] % 8) + 1;
    } else if (size >= 1) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

static aom_codec_ctx_t* init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_dec_cfg_t cfg;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    // Initialize config with default values manually
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    // Use fuzz data to tweak config slightly if size is sufficient
    if (size > 0) {
        cfg.threads = (data[0] % 8) + 1;
    }

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Randomly choose between encoder and decoder context
    int is_encoder = (Data[0] % 2);
    aom_codec_ctx_t *ctx = NULL;

    if (is_encoder) {
        ctx = init_encoder(Data, Size);
    } else {
        ctx = init_decoder(Data, Size);
    }

    if (!ctx) return 0;

    // Prepare control arguments based on available data
    int32_t val_int = 0;
    if (Size > 1) val_int = Data[1];
    if (Size > 2) val_int += Data[2];
    if (Size > 3) val_int += Data[3];

    // 1. aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID
    // Requires encoder. Argument is int32_t spatial_layer_id
    if (is_encoder) {
        aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, val_int);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    // Requires encoder. Argument is int32_t enable
    if (is_encoder) {
        aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val_int);
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA
    // Requires encoder. Argument is int32_t enable
    if (is_encoder) {
        aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val_int);
    }

    // 4. aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT
    // Requires decoder. Argument is int32_t operating_point
    if (!is_encoder) {
        aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, val_int);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
    // Requires encoder. Argument is int32_t mode
    if (is_encoder) {
        aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val_int);
    }

    // 6. aom_codec_control_typechecked_AV1D_SET_SKIP_FILM_GRAIN
    // Requires decoder. Argument is int32_t skip
    if (!is_encoder) {
        aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, val_int);
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}