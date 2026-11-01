// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

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

    // Use fuzz data to vary configuration slightly if possible, but keep valid ranges
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16; // Width between 16 and 1039
        cfg.g_h = (data[1] % 1024) + 16; // Height between 16 and 1039
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[2] % 60) + 1; // 1 to 60
        cfg.rc_target_bitrate = (data[3] % 10000) + 100;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Try to invoke the target control functions with fuzzed values
    int val_int;
    if (Size > 4) {
        val_int = Data[4] % 2; // 0 or 1 for boolean-like settings
    } else {
        val_int = 0;
    }

    // 1. AV1E_SET_DISABLE_TRELLIS_QUANT
    aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val_int);

    // 2. AOME_SET_ARNR_STRENGTH (0-64 typically)
    int arnr_strength = (Size > 5) ? (Data[5] % 65) : 0;
    aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);

    // 3. AOME_SET_ARNR_MAXFRAMES (0-32 typically)
    int arnr_maxframes = (Size > 6) ? (Data[6] % 33) : 0;
    aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);

    // 4. AOME_SET_SHARPNESS (0-7 typically)
    int sharpness = (Size > 7) ? (Data[7] % 8) : 0;
    aom_codec_control(ctx, AOME_SET_SHARPNESS, sharpness);

    // 5. AV1E_SET_DENOISE_NOISE_LEVEL (0-63 typically)
    int noise_level = (Size > 8) ? (Data[8] % 64) : 0;
    aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, noise_level);

    // 6. AV1E_SET_ENABLE_GLOBAL_MOTION (0 or 1)
    int global_motion = (Size > 9) ? (Data[9] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, global_motion);

    // Encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 32, 32, 1);
    memset(img.planes[0], 128, 32 * 32);
    memset(img.planes[1], 128, 16 * 16);
    memset(img.planes[2], 128, 16 * 16);

    // Fixed signature: aom_codec_encode(ctx, img, pts, duration, flags)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_encoder_context(ctx);

    return 0;
}