// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/* Helper to initialize a codec context for encoding */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    if (size < 4) return NULL;

    /* Extract width and height from fuzz input to vary test scenarios */
    uint32_t w = *(uint32_t *)data;
    uint32_t h = *(uint32_t *)(data + 4);

    /* Sanity checks to avoid allocation failures or invalid states */
    if (w == 0 || h == 0 || w > 8192 || h > 8192) {
        w = 64;
        h = 64;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;
    cfg.g_threads = 1;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to cleanup codec context */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_232(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t ret;
    int value;
    aom_codec_enc_cfg_t cfg;

    /* Ensure minimum size for basic extraction */
    if (Size < 1) return 0;

    /* Initialize encoder context */
    ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    /* Ensure we have a valid config pointer to read/write if needed */
    if (ctx->config.enc) {
        memcpy(&cfg, ctx->config.enc, sizeof(aom_codec_enc_cfg_t));
    } else {
        /* Fallback default config */
        aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (iface) {
            aom_codec_enc_config_default(iface, &cfg, 0);
        } else {
            cleanup_encoder(ctx);
            return 0;
        }
    }

    /* Use fuzz data to determine which control to test and what value to set */
    uint8_t cmd = Data[0] % 6;
    uint8_t val_byte = (Size > 1) ? Data[1] : 0;

    /* Map byte to a reasonable range for boolean/int flags */
    value = (val_byte % 2) ? 1 : 0;

    switch (cmd) {
        case 0:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING */
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, value);
            break;

        case 1:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE */
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, value);
            break;

        case 2:
            /* aom_codec_control_typechecked_AV1E_SET_RENDER_SIZE */
            if (Size >= 8) {
                uint32_t rw = *(uint32_t *)(Data + 2);
                uint32_t rh = *(uint32_t *)(Data + 6);
                if (rw == 0) rw = cfg.g_w;
                if (rh == 0) rh = cfg.g_h;
                if (rw > 8192) rw = 8192;
                if (rh > 8192) rh = 8192;
                ret = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, rw, rh);
            } else {
                ret = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, cfg.g_w, cfg.g_h);
            }
            break;

        case 3:
            /* aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC */
            ret = aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, value);
            break;

        case 4:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_CHROMA_DELTAQ */
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, value);
            break;

        case 5:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT */
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, value);
            break;

        default:
            break;
    }

    /* Optionally encode a frame to exercise state changes */
    if (ret == AOM_CODEC_OK) {
        aom_image_t img;
        aom_codec_cx_pkt_t pkt;
        aom_codec_enc_cfg_t *enc_cfg = (aom_codec_enc_cfg_t *)ctx->config.raw;
        
        aom_img_alloc(&img, AOM_IMG_FMT_I420, enc_cfg->g_w, enc_cfg->g_h, 1);
        memset(img.planes[0], 128, enc_cfg->g_w * enc_cfg->g_h);
        memset(img.planes[1], 128, (enc_cfg->g_w * enc_cfg->g_h) / 4);
        memset(img.planes[2], 128, (enc_cfg->g_w * enc_cfg->g_h) / 4);
        
        aom_codec_encode(ctx, &img, 0, 1, 0);
        
        while (aom_codec_get_frame(ctx, &pkt) == AOM_CODEC_OK) {
            if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                /* The frame image is inside pkt.data.frame */
                aom_img_free(&pkt.data.frame);
            }
        }
        aom_img_free(&img);
    }

    cleanup_encoder(ctx);
    return 0;
}