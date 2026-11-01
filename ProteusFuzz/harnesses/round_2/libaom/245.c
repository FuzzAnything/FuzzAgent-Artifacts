// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX at aomdx.h:592:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_SB_SIZE at aomdx.h:580:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER at aomdx.h:519:1 in aomdx.h
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER at aomcx.h:1902:1 in aomcx.h
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64 at aomcx.h:1905:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_GET_LUMA_CDEF_STRENGTH at aomcx.h:2336:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 32;
    cfg.h = 32;

    err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    if (data && size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    return ctx;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    aom_image_t img;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    if (data && size > 0) {
        memset(&img, 0, sizeof(img));
        img.w = 32;
        img.h = 32;
        img.d_w = 32;
        img.d_h = 32;
        img.planes[0] = NULL;
        img.planes[1] = NULL;
        img.planes[2] = NULL;
        
        if (aom_img_alloc(&img, AOM_IMG_FMT_I420, img.w, img.h, 1) == AOM_CODEC_OK) {
            size_t bytes_to_copy = (size_t)((img.w * img.h * 3) / 2);
            if (size < bytes_to_copy) bytes_to_copy = size;
            if (img.planes[0]) {
                memcpy(img.planes[0], data, bytes_to_copy);
                aom_codec_encode(ctx, &img, 0, 1, 0);
            }
            aom_img_free(&img);
        }
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);

    int64_t val64;
    int val32;
    aom_codec_err_t ret;

    // Test Decoder Control Functions
    if (dec_ctx) {
        ret = aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX(dec_ctx, AOMD_GET_BASE_Q_IDX, &val32);
        (void)ret; (void)val32;

        ret = aom_codec_control_typechecked_AOMD_GET_SB_SIZE(dec_ctx, AOMD_GET_SB_SIZE, &val32);
        (void)ret; (void)val32;

        ret = aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER(dec_ctx, AOMD_GET_LAST_QUANTIZER, &val32);
        (void)ret; (void)val32;
    }

    // Test Encoder Control Functions
    if (enc_ctx) {
        ret = aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER(enc_ctx, AOME_GET_LAST_QUANTIZER, &val32);
        (void)ret; (void)val32;

        ret = aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64(enc_ctx, AOME_GET_LAST_QUANTIZER_64, &val64);
        (void)ret; (void)val64;

        ret = aom_codec_control_typechecked_AV1E_GET_LUMA_CDEF_STRENGTH(enc_ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &val32);
        (void)ret; (void)val32;
    }

    cleanup_codec(dec_ctx);
    cleanup_codec(enc_ctx);

    return 0;
}