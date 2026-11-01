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

/* Helper function to safely initialize an encoder context */
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int width, height;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* Get the AV1 encoder interface using the correct function name */
    iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    /* Extract dimensions from fuzz input if possible, otherwise use defaults */
    if (size >= 4) {
        width = data[0] + 16;
        height = data[1] + 16;
        if (width < 16 || height < 16 || width > 4096 || height > 4096) {
            width = 64;
            height = 64;
        }
    } else {
        width = 64;
        height = 64;
    }

    /* Set default configuration */
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    /* Initialize the encoder context */
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper function to cleanup encoder context */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/* Helper function to generate a dummy frame */
static aom_image_t *create_dummy_frame(int w, int h) {
    aom_image_t *img = (aom_image_t *)calloc(1, sizeof(aom_image_t));
    if (!img) return NULL;

    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->fmt = AOM_IMG_FMT_I420;

    int plane_size = (w + 1) * (h + 1);
    img->planes[0] = (uint8_t *)calloc(plane_size, 1);
    img->planes[1] = (uint8_t *)calloc(plane_size / 4, 1);
    img->planes[2] = (uint8_t *)calloc(plane_size / 4, 1);
    img->stride[0] = w;
    img->stride[1] = (w + 1) / 2;
    img->stride[2] = (w + 1) / 2;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    return img;
}

/* Helper function to cleanup dummy frame */
static void cleanup_dummy_frame(aom_image_t *img) {
    if (img) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_image_t *frame = NULL;
    int val_int;
    aom_codec_err_t res;

    /* Ensure minimum size for basic operations */
    if (Size < 2) {
        return 0;
    }

    /* Initialize encoder context */
    ctx = init_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    /* Create a dummy frame based on input dimensions */
    int w = (Data[0] % 100) + 16;
    int h = (Data[1] % 100) + 16;
    frame = create_dummy_frame(w, h);
    if (!frame) {
        cleanup_encoder_context(ctx);
        return 0;
    }

    /* Fuzz AV1E_SET_DENOISE_NOISE_LEVEL */
    if (Size >= 3) {
        val_int = Data[2];
        res = aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val_int);
        (void)res;
    }

    /* Fuzz AV1E_SET_NOISE_SENSITIVITY */
    if (Size >= 4) {
        val_int = Data[3];
        res = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val_int);
        (void)res;
    }

    /* Fuzz AV1E_SET_LOOPFILTER_CONTROL */
    if (Size >= 5) {
        val_int = Data[4];
        res = aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, val_int);
        (void)res;
    }

    /* Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING */
    if (Size >= 6) {
        val_int = Data[5];
        res = aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val_int);
        (void)res;
    }

    /* Fuzz AV1_SET_SKIP_LOOP_FILTER (Decoder side, but try on encoder if supported) */
    if (Size >= 7) {
        val_int = Data[6];
        res = aom_codec_control(ctx, AV1_SET_SKIP_LOOP_FILTER, val_int);
        (void)res;
    }

    /* Fuzz AOME_GET_LOOPFILTER_LEVEL (Get operation) */
    if (Size >= 8) {
        val_int = Data[7];
        res = aom_codec_control(ctx, AOME_GET_LOOPFILTER_LEVEL, &val_int);
        (void)res;
    }

    /* Cleanup */
    cleanup_dummy_frame(frame);
    cleanup_encoder_context(ctx);

    return 0;
}