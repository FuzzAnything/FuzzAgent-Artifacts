// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions and set_option.
 * Target APIs:
 * 1. aom_codec_control_typechecked_AV1E_SET_VMAF_MODEL_PATH
 * 2. aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES
 * 3. aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY
 * 4. aom_codec_control_typechecked_AV1E_SET_SVC_PARAMS
 * 5. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
 * 6. aom_codec_set_option
 */

static aom_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzz data to vary configuration slightly if possible
    // Ensure we have enough data before accessing
    if (size >= 6) {
        cfg.g_w = (data[0] % 4096) + 16;
        cfg.g_h = (data[1] % 4096) + 16;
        // Fixed: g_time_base -> g_timebase
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
        cfg.g_error_resilient = (data[5] % 3);
    } else if (size > 0) {
        // Minimal valid config if data is small
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 1000;
        cfg.rc_target_bitrate = 100;
    }

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void destroy_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int fuzz_codec_control_vmaf_model_path(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    /*
     * aom_codec_control_typechecked_AV1E_SET_VMAF_MODEL_PATH
     * Expects a path string.
     */
    if (!ctx || !data || size == 0) return 0;

    // Create a null-terminated string from fuzz data
    size_t len = size < 256 ? size : 256;
    char path[257];
    memcpy(path, data, len);
    path[len] = '\0';

    // Ensure path is not empty and doesn't contain nulls in the middle (already handled)
    if (len == 0) return 0;

    aom_codec_err_t res = aom_codec_control(ctx, AV1E_SET_VMAF_MODEL_PATH, path);
    (void)res; // Ignore result for fuzzing, just trigger execution
    return 1;
}

static int fuzz_codec_control_max_ref_frames(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    /*
     * aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES
     * Expects an integer value.
     */
    if (!ctx || !data || size == 0) return 0;

    int val = 0;
    if (size >= 4) {
        val = *(const int32_t *)(data);
    } else if (size >= 1) {
        val = data[0];
    }

    // Clamp to reasonable range for AV1 (0 to 7 typically)
    if (val < 0) val = 0;
    if (val > 7) val = 7;

    aom_codec_err_t res = aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val);
    (void)res;
    return 1;
}

static int fuzz_codec_control_noise_sensitivity(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    /*
     * aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY
     * Expects an integer value.
     */
    if (!ctx || !data || size == 0) return 0;

    int val = 0;
    if (size >= 4) {
        val = *(const int32_t *)(data);
    } else if (size >= 1) {
        val = data[0];
    }

    // Clamp to reasonable range (0-64 typically)
    if (val < 0) val = 0;
    if (val > 64) val = 64;

    aom_codec_err_t res = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
    (void)res;
    return 1;
}

static int fuzz_codec_control_svc_params(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    /*
     * aom_codec_control_typechecked_AV1E_SET_SVC_PARAMS
     * Expects a pointer to aom_svc_params_t.
     */
    if (!ctx || !data || size == 0) return 0;

    aom_svc_params_t params;
    memset(&params, 0, sizeof(params));

    // Initialize with defaults
    params.number_spatial_layers = 1;
    params.number_temporal_layers = 1;

    if (size >= 2) {
        params.number_spatial_layers = (data[0] % 4) + 1; // 1 to 4
        params.number_temporal_layers = (data[1] % 4) + 1; // 1 to 4
    }

    // Fill arrays with fuzz data if available
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        int idx = i * 2;
        if (idx < size) params.max_quantizers[i] = data[idx] % 64;
        if (idx + 1 < size) params.min_quantizers[i] = data[idx + 1] % 64;
    }

    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        int idx = i * 2 + 2 * AOM_MAX_LAYERS;
        if (idx < size) params.scaling_factor_num[i] = (data[idx] % 255) + 1;
        if (idx + 1 < size) params.scaling_factor_den[i] = (data[idx + 1] % 255) + 1;
    }

    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        int idx = i * 2 + 2 * AOM_MAX_LAYERS + 2 * AOM_MAX_SS_LAYERS;
        if (idx < size) params.layer_target_bitrate[i] = (data[idx] % 1000) + 100;
    }

    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        int idx = i * 2 + 2 * AOM_MAX_LAYERS + 2 * AOM_MAX_SS_LAYERS + AOM_MAX_LAYERS;
        if (idx < size) params.framerate_factor[i] = (data[idx] % 10) + 1;
    }

    aom_codec_err_t res = aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &params);
    (void)res;
    return 1;
}

static int fuzz_codec_control_enable_intrabc(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    /*
     * aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
     * Expects an integer (0 or 1).
     */
    if (!ctx || !data || size == 0) return 0;

    int val = 0;
    if (size >= 1) {
        val = data[0] % 2;
    }

    aom_codec_err_t res = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)res;
    return 1;
}

static int fuzz_codec_set_option(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    /*
     * aom_codec_set_option
     * Expects key and value strings.
     */
    if (!ctx || !data || size == 0) return 0;

    // Split data into key and value
    size_t key_len = size / 2;
    size_t val_len = size - key_len;

    if (key_len == 0 || val_len == 0) {
        key_len = 1;
        val_len = 1;
    }

    char key[257], value[257];
    memset(key, 0, sizeof(key));
    memset(value, 0, sizeof(value));

    // Safety check: ensure we don't read past the end of the input buffer
    // Calculate safe copy lengths
    size_t k_copy = key_len < 256 ? key_len : 256;
    size_t v_copy = val_len < 256 ? val_len : 256;

    // Ensure we don't read beyond 'Size' when copying value
    // The value starts at 'data + key_len', so we need 'key_len + v_copy' <= 'Size'
    if (key_len + v_copy > size) {
        v_copy = size - key_len;
    }
    
    // Re-check if we have enough data after adjustment
    if (v_copy <= 0) return 0;

    memcpy(key, data, k_copy);
    memcpy(value, data + key_len, v_copy);

    // Ensure null termination
    key[k_copy] = '\0';
    value[v_copy] = '\0';

    // Avoid empty keys or values
    if (key[0] == '\0') key[0] = 'a';
    if (value[0] == '\0') value[0] = 'b';

    aom_codec_err_t res = aom_codec_set_option(ctx, key, value);
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create encoder context
    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz each target API
    fuzz_codec_control_vmaf_model_path(ctx, Data, Size);
    fuzz_codec_control_max_ref_frames(ctx, Data, Size);
    fuzz_codec_control_noise_sensitivity(ctx, Data, Size);
    fuzz_codec_control_svc_params(ctx, Data, Size);
    fuzz_codec_control_enable_intrabc(ctx, Data, Size);
    fuzz_codec_set_option(ctx, Data, Size);

    // Cleanup
    destroy_encoder_context(ctx);

    return 0;
}