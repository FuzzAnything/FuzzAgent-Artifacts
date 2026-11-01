// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/* Helper function to initialize the codec context */
static aom_codec_ctx_t *init_codec_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzzing data to tweak configuration slightly if size allows */
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        if (size > 1) cfg.g_h = (data[1] % 256) + 16;
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper function to create a dummy image frame */
static aom_image_t *create_dummy_image(aom_codec_ctx_t *ctx) {
    if (!ctx) return NULL;
    
    const aom_codec_enc_cfg_t *cfg = ctx->config.enc;
    if (!cfg) return NULL;

    aom_image_t *img = (aom_image_t *)calloc(1, sizeof(aom_image_t));
    if (!img) return NULL;

    if (aom_img_alloc(img, AOM_IMG_FMT_I420, cfg->g_w, cfg->g_h, 1) != 0) {
        free(img);
        return NULL;
    }

    return img;
}

int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_codec_context(Data, Size);
    if (!ctx) return 0;

    aom_image_t *img = create_dummy_image(ctx);
    if (!img) {
        aom_codec_destroy(ctx);
        free(ctx);
        return 0;
    }

    /* Fuzz AV1E_SET_RTC_EXTERNAL_RC */
    int rc_val = (Data[0] % 256);
    aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, &rc_val);

    /* Fuzz AV1E_SET_ENABLE_CHROMA_DELTAQ */
    int deltaq_val = (Data[1 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, &deltaq_val);

    /* Fuzz AV1E_SET_MIN_PARTITION_SIZE */
    int min_part_val = (Data[2 % Size] % 64) + 4;
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, &min_part_val);

    /* Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR */
    int drop_ms_val = (Data[3 % Size] % 1000);
    aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, &drop_ms_val);

    /* Fuzz AV1E_SET_MAX_PARTITION_SIZE */
    int max_part_val = (Data[4 % Size] % 256) + 4;
    aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, &max_part_val);

    /* Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE */
    int wedge_val = (Data[5 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, &wedge_val);

    /* Encode a frame to exercise the state */
    aom_codec_enc_cfg_t enc_cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), &enc_cfg, 0) == AOM_CODEC_OK) {
            aom_codec_encode(ctx, img, 0, 1, 0);
        }
    }

    /* Cleanup */
    aom_img_free(img);
    free(img);
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}