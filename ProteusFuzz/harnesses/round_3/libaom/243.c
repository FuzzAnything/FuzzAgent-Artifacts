// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

// Helper to safely free codec context
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_243(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    uint32_t width = Data[0] | (Data[1] << 8);
    uint32_t height = Data[2] | (Data[3] << 8);
    
    // Ensure valid dimensions
    if (width == 0 || height == 0) {
        width = 64;
        height = 64;
    }
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Initialize Encoder Context
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    // Use the standard function to get the AV1 codec interface
    // aom_codec_av1_cx() returns the interface for both encoder and decoder
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz the control commands
    uint32_t val = 0;
    if (Size > 4) {
        val = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
    }

    // 1. AV1E_SET_ENABLE_ONESIDED_COMP (Takes an int)
    aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);

    // 2. AV1_GET_ACCOUNTING (Takes a pointer to Accounting struct)
    // We allocate a dummy buffer. The actual struct size is unknown but we pass a valid pointer.
    void *acc_ptr = malloc(64); 
    if (acc_ptr) {
        aom_codec_control(ctx, AV1_GET_ACCOUNTING, acc_ptr);
        free(acc_ptr);
    }

    // 3. AV1E_GET_NUM_OPERATING_POINTS (Takes a pointer to int)
    int num_ops = 0;
    aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);

    // 4. AV1E_SET_SVC_FRAME_DROP_MODE (Takes an int)
    aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val & 0xFF);

    // 5. AV1E_SET_FILM_GRAIN_TEST_VECTOR (Takes a pointer to data)
    // We pass a pointer to the fuzz input data if available
    if (Size > 8) {
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (const void *)(Data + 8));
    }

    // 6. AV1E_SET_INTRA_DEFAULT_TX_ONLY (Takes an int)
    aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val & 0x1);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}