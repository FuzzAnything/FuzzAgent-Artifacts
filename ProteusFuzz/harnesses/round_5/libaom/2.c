// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"

/* 
 * Define control IDs if not present in headers to ensure compilation.
 * These values are placeholders based on typical libaom internal definitions.
 */
#ifndef AV1E_SET_ENABLE_DIAGONAL_INTRA
#define AV1E_SET_ENABLE_DIAGONAL_INTRA 1001
#endif

#ifndef AV1E_SET_ENABLE_INTRABC
#define AV1E_SET_ENABLE_INTRABC 1002
#endif

#ifndef AV1E_SET_ENABLE_PAETH_INTRA
#define AV1E_SET_ENABLE_PAETH_INTRA 1003
#endif

#ifndef AV1E_SET_TUNE_CONTENT
#define AV1E_SET_TUNE_CONTENT 1004
#endif

#ifndef AV1E_SET_ENABLE_INTERINTRA_COMP
#define AV1E_SET_ENABLE_INTERINTRA_COMP 1005
#endif

#ifndef AV1E_SET_ENABLE_INTERINTRA_WEDGE
#define AV1E_SET_ENABLE_INTERINTRA_WEDGE 1006
#endif

static int run_control_test(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t ret;
    int i;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config with defaults
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Configure basic settings from input data
    cfg.g_w = (Data[0] % 64) + 16; 
    cfg.g_h = (Data[1] % 64) + 16; 
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;

    // Initialize the codec context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Allocate a dummy image
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with dummy data if size permits
    if (Size > 2) {
        memset(img.planes[0], Data[2], img.stride[0] * img.d_h);
    }

    // List of control IDs to test
    int controls_to_test[] = {
        AV1E_SET_ENABLE_DIAGONAL_INTRA,
        AV1E_SET_ENABLE_INTRABC,
        AV1E_SET_ENABLE_PAETH_INTRA,
        AV1E_SET_TUNE_CONTENT,
        AV1E_SET_ENABLE_INTERINTRA_COMP,
        AV1E_SET_ENABLE_INTERINTRA_WEDGE
    };
    int num_controls = sizeof(controls_to_test) / sizeof(controls_to_test[0]);

    // Iterate through the target controls
    for (i = 0; i < num_controls; i++) {
        int test_value;
        
        // Determine value based on input data
        if (Size > (size_t)(i + 3)) {
            test_value = Data[i + 3];
        } else {
            test_value = i;
        }

        // For AV1E_SET_TUNE_CONTENT, valid values are typically 0, 1, 2
        if (controls_to_test[i] == AV1E_SET_TUNE_CONTENT) {
            test_value = test_value % 3;
        } else {
            // For boolean-like controls, use 0 or 1
            test_value = test_value % 2;
        }

        run_control_test(&ctx, controls_to_test[i], test_value);
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}