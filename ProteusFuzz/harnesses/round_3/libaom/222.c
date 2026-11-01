// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t* init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Use AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    // Use fuzz data to tweak config slightly if size permits
    if (size > 0) {
        cfg.g_w = (data[0] % 16) + 16;
        cfg.g_h = (data[1] % 16) + 16;
        if (cfg.g_w > 4096) cfg.g_w = 4096;
        if (cfg.g_h > 4096) cfg.g_h = 4096;
    }

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_222(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz aom_codec_set_option
    // Extract key and value from data if possible
    size_t key_len = 0;
    size_t val_len = 0;
    const char *key = "cpu-used";
    const char *val = "0";

    if (Size >= 4) {
        key_len = Data[0] % 10;
        val_len = (Data[1] % 10) + 1;
        if (Size >= key_len + val_len + 2) {
            char *k_buf = malloc(key_len + 1);
            char *v_buf = malloc(val_len + 1);
            if (k_buf && v_buf) {
                memcpy(k_buf, Data + 2, key_len);
                k_buf[key_len] = '\0';
                memcpy(v_buf, Data + 2 + key_len, val_len);
                v_buf[val_len] = '\0';
                
                key = k_buf;
                val = v_buf;
                
                aom_codec_set_option(ctx, key, val);
                
                free(k_buf);
                free(v_buf);
            }
        }
    }

    // Fuzz aom_codec_control with various AV1E_* controls
    // Use the correct enum values from aomcx.h
    
    // AV1E_SET_S_FRAME_MODE
    int s_frame_mode = (Data[0] % 3); // 0, 1, or 2
    aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);

    // AV1E_SET_NOISE_SENSITIVITY
    int noise_sensitivity = (Data[1] % 100);
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    // AV1E_SET_ERROR_RESILIENT_MODE
    int error_resilient = (Data[2] % 2);
    aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient);

    // AV1E_SET_ENABLE_INTERINTRA_WEDGE
    int enable_wedge = (Data[3] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_wedge);

    // AV1E_SET_ENABLE_PAETH_INTRA
    int enable_paeth = (Data[4] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth);

    cleanup_codec(ctx);
    return 0;
}