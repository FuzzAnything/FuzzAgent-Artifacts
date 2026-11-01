// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

/*
 * Helper function to initialize an encoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t *img = NULL;
    int ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzz input to vary configuration slightly */
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
        cfg.g_timebase.num = (size > 2 ? data[2] % 100 : 1) + 1;
        cfg.g_timebase.den = (size > 3 ? data[3] % 100 : 1) + 1;
        cfg.g_threads = (size > 4 ? data[4] % 4 : 1) + 1;
        cfg.rc_target_bitrate = (size > 5 ? data[5] % 1000 : 100) + 100;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Allocate a dummy image to ensure encoder is ready */
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        aom_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    /* Fixed: aom_codec_encode takes 5 arguments */
    ret = aom_codec_encode(ctx, img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    aom_img_free(img);
    return ctx;
}

/*
 * Helper function to initialize a decoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    int ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* Initialize config manually, using valid fields only */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_212(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *enc_ctx = NULL;
    aom_codec_ctx_t *dec_ctx = NULL;
    int ret;

    /* Ensure we have enough data for basic operations */
    if (Size < 2) {
        return 0;
    }

    /* Initialize encoder and decoder based on input parity */
    if (Data[0] % 2 == 0) {
        enc_ctx = init_encoder(Data, Size);
    } else {
        dec_ctx = init_decoder(Data, Size);
    }

    /*
     * Fuzz aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID
     * Requires encoder context. Sets spatial layer ID.
     */
    if (enc_ctx) {
        int layer_id = (Data[1] % 8);
        ret = aom_codec_control(enc_ctx, AOME_SET_SPATIAL_LAYER_ID, layer_id);
        (void)ret;
    }

    /*
     * Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
     * Requires encoder context. Enables low complexity decode mode.
     */
    if (enc_ctx && Size > 2) {
        int enable = (Data[2] % 2);
        ret = aom_codec_control(enc_ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, enable);
        (void)ret;
    }

    /*
     * Fuzz aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
     * Requires encoder context. Sets SVC frame drop mode.
     */
    if (enc_ctx && Size > 3) {
        int mode = (Data[3] % 4);
        ret = aom_codec_control(enc_ctx, AV1E_SET_SVC_FRAME_DROP_MODE, mode);
        (void)ret;
    }

    /*
     * Fuzz aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT
     * Requires decoder context. Sets operating point for scalable video.
     */
    if (dec_ctx && Size > 4) {
        int op = (Data[4] % 128);
        ret = aom_codec_control(dec_ctx, AV1D_SET_OPERATING_POINT, op);
        (void)ret;
    }

    /*
     * Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE
     * Requires encoder context. Sets max partition size.
     */
    if (enc_ctx && Size > 5) {
        int size_val = (Data[5] % 64) + 4;
        ret = aom_codec_control(enc_ctx, AV1E_SET_MAX_PARTITION_SIZE, size_val);
        (void)ret;
    }

    /*
     * Fuzz aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS
     * Requires encoder context. Sets number of spatial layers.
     */
    if (enc_ctx && Size > 6) {
        int layers = (Data[6] % 4) + 1;
        ret = aom_codec_control(enc_ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, layers);
        (void)ret;
    }

    /* Cleanup */
    if (enc_ctx) {
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }

    if (dec_ctx) {
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    return 0;
}