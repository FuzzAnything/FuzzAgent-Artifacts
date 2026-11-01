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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_codec_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (size < 4) return NULL;

    uint32_t width = data[0] | (data[1] << 8);
    uint32_t height = data[2] | (data[3] << 8);

    if (width == 0 || height == 0 || width > 8192 || height > 8192) {
        width = 128;
        height = 128;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_codec_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static uint8_t safe_get_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_codec_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    int val_trellis = safe_get_byte(Data, Size, 0) % 2;
    aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val_trellis);

    // Fuzz AV1E_SET_DELTALF_MODE
    int val_deltalf = safe_get_byte(Data, Size, 1) % 3; // Assuming 0-2 range
    aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val_deltalf);

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    int val_gm = safe_get_byte(Data, Size, 2) % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val_gm);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    int val_cdf = safe_get_byte(Data, Size, 3) % 2;
    aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val_cdf);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    int val_tpl = safe_get_byte(Data, Size, 4) % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val_tpl);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    int val_cost = safe_get_byte(Data, Size, 5) % 100; // Assuming 0-99 range
    aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val_cost);

    cleanup_codec_context(ctx);
    return 0;
}