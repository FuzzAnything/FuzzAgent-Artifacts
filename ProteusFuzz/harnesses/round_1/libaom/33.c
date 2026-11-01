// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_ENABLE_ONESIDED_COMP
 * - AV1E_SET_ENABLE_FILTER_INTRA
 * - AV1E_SET_ENABLE_SMOOTH_INTRA
 * - AV1E_SET_ENABLE_CFL_INTRA
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 */

static aom_codec_ctx_t *init_encoder(int width, int height) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return NULL;
    }

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = (width > 0 && width < 4096) ? width : 64;
    cfg.g_h = (height > 0 && height < 4096) ? height : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void fuzz_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL) return;

    // Control IDs to test (defined in aom/aom_encoder.h for AV1)
    const int control_ids[] = {
        AV1E_SET_INTER_DCT_ONLY,
        AV1E_SET_ENABLE_ONESIDED_COMP,
        AV1E_SET_ENABLE_FILTER_INTRA,
        AV1E_SET_ENABLE_SMOOTH_INTRA,
        AV1E_SET_ENABLE_CFL_INTRA,
        AV1E_SET_ENABLE_REF_FRAME_MVS
    };
    const int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

    // Iterate through controls, using fuzz data to determine the value
    for (int i = 0; i < num_controls; i++) {
        int value;
        
        // Derive a value from the input data to explore different states
        if (size > 0) {
            value = data[i % size] % 2; // Toggle between 0 and 1
        } else {
            value = i % 2;
        }

        // Invoke the control
        aom_codec_control(ctx, control_ids[i], value);
        
        // Also try with a wider range of values if the API accepts them
        if (size > 1) {
            int alt_value = (data[(i + 1) % size]) & 0xFF;
            aom_codec_control(ctx, control_ids[i], alt_value);
        }
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Extract dimensions from data if possible, otherwise use defaults
    int width = 64;
    int height = 64;
    
    if (Size >= 2) {
        width = Data[0] + 1; // Ensure > 0
        height = Data[1] + 1;
        // Cap dimensions to reasonable limits to avoid OOM
        if (width > 512) width = 512;
        if (height > 512) height = 512;
    }

    aom_codec_ctx_t *ctx = init_encoder(width, height);
    if (ctx == NULL) {
        return 0;
    }

    // Run the fuzzing logic on the controls
    fuzz_controls(ctx, Data, Size);

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}