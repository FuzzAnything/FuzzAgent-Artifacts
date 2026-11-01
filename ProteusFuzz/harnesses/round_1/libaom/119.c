// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/*
 * Fuzz driver for libaom codec control functions.
 * Targets:
 * 1. aom_codec_control_typechecked_AV1E_SET_QM_MAX
 * 2. aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID
 * 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
 * 4. aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT
 * 5. aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS
 * 6. aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
 *
 * Note: These functions are internal type-checked wrappers. We invoke them
 * via the standard aom_codec_control API with the corresponding control IDs.
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    unsigned int width = 0, height = 0;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Parse dimensions from input if available, otherwise use defaults */
    if (size >= 4) {
        width = (data[0] & 0xFF) | ((data[1] & 0xFF) << 8);
        height = (data[2] & 0xFF) | ((data[3] & 0xFF) << 8);
    }
    if (width == 0) width = 64;
    if (height == 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Initialize config manually with valid fields for the target libaom version */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    /* Use input data to tweak some config fields if possible */
    if (size >= 2) {
        cfg.threads = (data[0] & 0x0F) + 1;
    }

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
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

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    /* Use first byte to decide encoder vs decoder path */
    int use_encoder = (Data[0] & 0x01);
    int mode = (Data[0] >> 1) & 0x07; /* Select which control to test */

    aom_codec_ctx_t *ctx = NULL;
    int32_t val = 0;

    /* Safely read the 32-bit value from the buffer starting at offset 1 */
    if (Size >= 5) {
        val = (int32_t)Data[1] | ((int32_t)Data[2] << 8) | ((int32_t)Data[3] << 16) | ((int32_t)Data[4] << 24);
    } else {
        val = 0;
    }

    if (use_encoder) {
        ctx = init_encoder(Data, Size);
        if (!ctx) return 0;

        switch (mode) {
            case 0: /* AV1E_SET_QM_MAX */
                aom_codec_control(ctx, AV1E_SET_QM_MAX, val);
                break;
            case 1: /* AOME_SET_SPATIAL_LAYER_ID */
                aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, val);
                break;
            case 2: /* AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE */
                aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
                break;
            case 3: /* AOME_SET_NUMBER_SPATIAL_LAYERS */
                aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val);
                break;
            case 4: /* AV1E_SET_SVC_FRAME_DROP_MODE */
                aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);
                break;
            case 5: /* Test multiple controls */
                aom_codec_control(ctx, AV1E_SET_QM_MAX, val);
                aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, val & 0x0F);
                aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val & 1);
                break;
            case 6: /* Another combination */
                aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, (val & 0x0F) + 1);
                aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val & 0x03);
                break;
            default:
                break;
        }

        cleanup_encoder(ctx);
    } else {
        ctx = init_decoder(Data, Size);
        if (!ctx) return 0;

        switch (mode) {
            case 0: /* AV1D_SET_OPERATING_POINT */
                aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, val);
                break;
            case 1: /* AV1D_SET_OPERATING_POINT with different values */
                aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, val & 0xFFFF);
                break;
            case 2: /* AV1D_SET_OPERATING_POINT with extreme values */
                aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 0);
                aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 0xFFFFFFFF);
                break;
            case 3: /* AV1D_SET_OPERATING_POINT with small range */
                aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, val & 0x0F);
                break;
            default:
                break;
        }

        cleanup_decoder(ctx);
    }

    return 0;
}