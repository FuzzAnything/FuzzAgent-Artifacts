// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_FLIP_IDTX
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AV1E_SET_ENABLE_DNL_DENOISING
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_ENABLE_DIST_WTD_COMP
 * - AV1_SET_INSPECTION_CALLBACK
 */

/* Forward declaration of the callback function */
static void dummy_inspect_cb(void *ctx, aom_image_t *img);

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int width, height;

    if (ctx == NULL) return 0;

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize config with defaults
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Extract dimensions from fuzz data if available, otherwise use defaults
    if (size >= 4) {
        width = data[0] ? data[0] : 16;
        height = data[1] ? data[1] : 16;
        // Ensure alignment
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;
    } else {
        width = 16;
        height = 16;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    // Initialize encoder
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static aom_codec_err_t set_inspection_callback(aom_codec_ctx_t *ctx, void *cb, void *ctx_data) {
    aom_inspect_init init;
    init.inspect_cb = cb;
    init.inspect_ctx = ctx_data;
    // Use the correct constant name AV1_SET_INSPECTION_CALLBACK
    return aom_codec_control(ctx, AV1_SET_INSPECTION_CALLBACK, &init);
}

/* Definition of the callback function */
static void dummy_inspect_cb(void *ctx, aom_image_t *img) {
    (void)ctx;
    (void)img;
}

int LLVMFuzzerTestOneInput_271(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int initialized = 0;
    int ret;
    int val_int;
    uint8_t *data_ptr = NULL;
    size_t data_size = 0;

    if (Size == 0) return 0;

    // Initialize codec context
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder
    initialized = init_encoder(&ctx, Data, Size);
    if (!initialized) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Write dummy file if needed (for potential file-based operations)
    write_dummy_file(Data, Size);

    // Extract values from fuzz data for control parameters
    if (Size >= 1) val_int = Data[0];
    else val_int = 0;

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX (expects int)
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val_int);
    (void)ret;

    // Fuzz AV1E_SET_NOISE_SENSITIVITY (expects int)
    if (Size >= 2) val_int = Data[1];
    ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val_int);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING (expects int)
    if (Size >= 3) val_int = Data[2];
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val_int);
    (void)ret;

    // Fuzz AV1E_SET_S_FRAME_MODE (expects int)
    if (Size >= 4) val_int = Data[3];
    ret = aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val_int);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP (expects int)
    if (Size >= 5) val_int = Data[4];
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val_int);
    (void)ret;

    // Fuzz AV1_SET_INSPECTION_CALLBACK
    // Prepare a dummy callback context
    if (Size >= 6) {
        data_ptr = (uint8_t *)malloc(Size);
        if (data_ptr) {
            memcpy(data_ptr, Data, Size);
            data_size = Size;
        }
    }

    if (data_ptr != NULL) {
        ret = set_inspection_callback(&ctx, dummy_inspect_cb, data_ptr);
        (void)ret;
    } else {
        ret = set_inspection_callback(&ctx, dummy_inspect_cb, NULL);
        (void)ret;
    }

    // Cleanup
    if (data_ptr != NULL) {
        free(data_ptr);
    }

    cleanup_codec(&ctx);

    return 0;
}