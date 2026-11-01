// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target APIs:
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 * - AV1E_SET_FORCE_VIDEO_MODE
 * - AV1E_SET_TARGET_SEQ_LEVEL_IDX
 * - AV1E_SET_TIMING_INFO_TYPE
 * - AV1E_SET_ENABLE_SUPERRES
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_encoder(ctx);
        return NULL;
    }

    /* Use fuzz input to configure width/height if possible, otherwise defaults */
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        /* Ensure non-zero and reasonable bounds */
        if (w > 0 && w < 8192) cfg.g_w = w;
        if (h > 0 && h < 8192) cfg.g_h = h;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_encoder(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    /*
     * We interpret the input data to select which control function to call
     * and what arguments to pass.
     * Byte 0: Select function (0-5)
     * Remaining bytes: Arguments
     */

    int func_idx = Data[0] % 6;
    const uint8_t *args = Data + 1;
    size_t arg_len = Size - 1;

    int arg_val = 0;
    if (arg_len >= 4) {
        arg_val = *(const int32_t *)args;
    } else if (arg_len >= 1) {
        arg_val = (int32_t)args[0];
    }

    aom_codec_err_t res = AOM_CODEC_OK;

    switch (func_idx) {
        case 0:
            /* AV1E_SET_S_FRAME_MODE */
            res = aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, arg_val);
            break;
        case 1:
            /* AV1E_SET_COEFF_COST_UPD_FREQ */
            res = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, arg_val);
            break;
        case 2:
            /* AV1E_SET_FORCE_VIDEO_MODE */
            res = aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, arg_val);
            break;
        case 3:
            /* AV1E_SET_TARGET_SEQ_LEVEL_IDX */
            res = aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, arg_val);
            break;
        case 4:
            /* AV1E_SET_TIMING_INFO_TYPE */
            res = aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, arg_val);
            break;
        case 5:
            /* AV1E_SET_ENABLE_SUPERRES */
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, arg_val);
            break;
        default:
            break;
    }

    /*
     * Optionally, perform a single encode step to exercise more code paths
     * if the encoder is in a valid state.
     */
    if (res == AOM_CODEC_OK) {
        aom_image_t img;
        aom_codec_cx_pkt_t pkt;
        aom_codec_enc_cfg_t *enc_cfg = (aom_codec_enc_cfg_t *)ctx->config.enc;

        if (enc_cfg) {
            int w = enc_cfg->g_w;
            int h = enc_cfg->g_h;
            if (w > 0 && h > 0) {
                aom_img_alloc(&img, AOM_IMG_FMT_I420, w, h, 1);
                memset(img.planes[0], 128, w * h);
                memset(img.planes[1], 128, (w * h) / 4);
                memset(img.planes[2], 128, (w * h) / 4);

                aom_enc_frame_flags_t flags = 0;
                aom_codec_encode(ctx, &img, 0, 1, flags);

                /* Drain any output packets */
                while (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
                    if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                        /* No specific unref needed for the packet structure itself in this context */
                        /* The frame data is managed internally or via aom_img_free if extracted */
                    }
                }
                aom_img_free(&img);
            }
        }
    }

    cleanup_encoder(ctx);
    return 0;
}