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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

// Helper to initialize the encoder context
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
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
    
    // Use fuzzing data to set some config values if available
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1 % size] % 256) + 16;
        cfg.g_timebase.num = (data[2 % size] % 255) + 1;
        cfg.g_timebase.den = (data[3 % size] % 255) + 1;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to cleanup encoder context
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_QM
    // Assuming it takes an int (0 or 1)
    int enable_qm = (Data[0] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_QM, &enable_qm);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_QM_MAX
    // Assuming it takes an int
    int qm_max = (Data[1 % Size] % 100) + 1;
    aom_codec_control(ctx, AV1E_SET_QM_MAX, &qm_max);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY
    // Assuming it takes an int
    int noise_sensitivity = (Data[2 % Size] % 100);
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, &noise_sensitivity);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE
    // Assuming it takes an int
    int denoise_block_size = (Data[3 % Size] % 64) + 1;
    aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, &denoise_block_size);

    // Fuzz aom_codec_control_typechecked_AV1E_ENABLE_RATE_GUIDE_DELTAQ
    // Assuming it takes an int (0 or 1)
    int enable_rate_guide_deltaq = (Data[4 % Size] % 2);
    aom_codec_control(ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, &enable_rate_guide_deltaq);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE
    // Assuming it takes an int (mode)
    int deltaq_mode = (Data[5 % Size] % 4);
    aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, &deltaq_mode);

    cleanup_encoder_context(ctx);
    return 0;
}