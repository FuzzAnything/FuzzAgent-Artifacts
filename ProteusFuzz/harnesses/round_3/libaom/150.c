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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
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

    // Use fuzzed data to set some config parameters safely
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        if (size > 1) cfg.g_h = (data[1] % 256) + 16;
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
        if (size > 4) cfg.g_threads = (data[4] % 8) + 1;
        if (size > 5) cfg.rc_target_bitrate = (data[5] % 1000) + 100;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Extract parameters from fuzzed data
    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    int val2 = (Size > 1) ? (Data[1] % 2) : 0;
    int val3 = (Size > 2) ? (Data[2] % 3) : 0;
    int val4 = (Size > 3) ? (Data[3] % 8) : 0;
    int val5 = (Size > 4) ? (Data[4] % 2) : 0;
    int val6 = (Size > 5) ? (Data[5] % 2) : 0;

    // Call target control functions using the correct enum values from aomcx.h
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, val1);
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val2);
    aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, val3);
    aom_codec_control(ctx, AOME_SET_CPUUSED, val4);
    aom_codec_control(ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, val5);
    aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, val6);

    // Optionally encode a dummy frame to exercise state
    if (Size > 6) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                      (Size > 7 ? Data[7] % 256 + 16 : 16),
                      (Size > 8 ? Data[8] % 256 + 16 : 16), 1);
        
        aom_enc_frame_flags_t flags = 0;
        aom_codec_encode(ctx, &img, 0, 1, flags);
        
        aom_img_free(&img);
    }

    cleanup_encoder(ctx);
    return 0;
}