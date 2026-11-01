// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
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
    }
}

static aom_codec_ctx_t *init_encoder_context(int width, int height, int fps_num, int fps_den) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    int fps_num = Data[4] | (Data[5] << 8);
    int fps_den = Data[6] | (Data[7] << 8);

    if (width <= 0 || height <= 0 || fps_num <= 0 || fps_den <= 0) {
        width = 64; height = 64; fps_num = 30; fps_den = 1;
    }

    aom_codec_ctx_t *ctx = init_encoder_context(width, height, fps_num, fps_den);
    if (!ctx) return 0;

    // Extract parameters from input data
    int enable_rect_tx = Data[8];
    int tuning = Data[9];
    int enable_chroma_deltaq = Data[10];
    int inter_dct_only = Data[11];
    int postencode_drop_rtc = Data[12];
    int enable_interinter_wedge = Data[13];

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_TX
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);

    // Fuzz aom_codec_control_typechecked_AOME_SET_TUNING
    aom_codec_control(ctx, AOME_SET_TUNING, tuning);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_CHROMA_DELTAQ
    aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_chroma_deltaq);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_INTER_DCT_ONLY
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_POSTENCODE_DROP_RTC
    aom_codec_control(ctx, AV1E_SET_POSTENCODE_DROP_RTC, postencode_drop_rtc);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_interinter_wedge);

    // Attempt to encode a dummy frame to exercise more states
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(img.planes[0], 128, width * height);
    memset(img.planes[1], 128, (width * height) / 4);
    memset(img.planes[2], 128, (width * height) / 4);

    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(ctx, &img, 0, 1, flags);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}