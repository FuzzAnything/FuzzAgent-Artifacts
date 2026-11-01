// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>

static aom_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzzing data to set some configuration values safely
    cfg.g_w = (size_t)(data[0] % 128) + 16;
    cfg.g_h = (size_t)(data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (data[2] % 1000) + 100;
    cfg.g_threads = (data[3] % 4) + 1;
    cfg.g_profile = data[4] % 4;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_189(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz various control functions with derived integer values from input
    int val1 = (int)(Data[0] % 2); // 0 or 1
    int val2 = (int)(Data[1] % 2);
    int val3 = (int)(Data[2] % 2);
    int val4 = (int)(Data[3] % 10);
    int val5 = (int)(Data[4] % 100);
    int val6 = (int)(Data[5 % Size] % 100);

    // Invoke the target control functions
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val2);
    aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val3);
    aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val4);
    aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val5);
    aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val6);

    // Attempt to encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                  ctx->config.enc->g_w, ctx->config.enc->g_h, 1);
    memset(&img.planes[0][0], 0, img.stride[0] * img.d_h);
    memset(&img.planes[1][0], 128, img.stride[1] * img.d_h / 2);
    memset(&img.planes[2][0], 128, img.stride[2] * img.d_h / 2);

    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(ctx, &img, 0, 1, flags);

    aom_img_free(&img);
    cleanup_encoder_context(ctx);

    return 0;
}