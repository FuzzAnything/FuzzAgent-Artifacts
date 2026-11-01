// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t *img;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ctx->iface = iface;
    ctx->name = "av1";
    ctx->err = AOM_CODEC_OK;
    ctx->err_detail = NULL;
    ctx->init_flags = 0;
    ctx->config.enc = NULL;
    ctx->priv = NULL;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        aom_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    if (data && size > 0) {
        size_t bytes_per_plane = 64 * 64;
        size_t total_needed = bytes_per_plane * 3;
        if (size > total_needed) size = total_needed;
        memcpy(img->planes[0], data, size > bytes_per_plane ? bytes_per_plane : size);
        if (size > bytes_per_plane) {
            memcpy(img->planes[1], data + bytes_per_plane, size - bytes_per_plane > bytes_per_plane ? bytes_per_plane : size - bytes_per_plane);
        }
        if (size > 2 * bytes_per_plane) {
            memcpy(img->planes[2], data + 2 * bytes_per_plane, size - 2 * bytes_per_plane > bytes_per_plane ? bytes_per_plane : size - 2 * bytes_per_plane);
        }
    }

    res = aom_codec_encode(ctx, img, 0, 1, 0);
    aom_img_free(img);

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int fuzz_control_function(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t res;
    int val = value;
    
    switch (control_id) {
        case AV1E_SET_ENABLE_QM:
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
            break;
        case AV1E_SET_NOISE_SENSITIVITY:
            res = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
            break;
        case AV1E_SET_AQ_MODE:
            res = aom_codec_control(ctx, AV1E_SET_AQ_MODE, val);
            break;
        case AV1E_SET_QM_MAX:
            res = aom_codec_control(ctx, AV1E_SET_QM_MAX, val);
            break;
        case AV1E_SET_DELTAQ_STRENGTH:
            res = aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val);
            break;
        case AV1E_ENABLE_RATE_GUIDE_DELTAQ:
            res = aom_codec_control(ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val);
            break;
        default:
            return 0;
    }
    return (res == AOM_CODEC_OK);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    int control_id = 0;
    int value = 0;

    if (Size >= 2) {
        control_id = (int)Data[0] % 6;
        value = (int)Data[1];
    } else if (Size == 1) {
        control_id = (int)Data[0] % 6;
        value = 0;
    }

    fuzz_control_function(ctx, control_id, value);

    if (Size >= 3) {
        int val2 = (int)Data[2];
        fuzz_control_function(ctx, (control_id + 1) % 6, val2);
    }

    cleanup_encoder(ctx);
    return 0;
}