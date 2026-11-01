// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int try_encode_and_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL) return 0;

    // Determine control command based on input data to diversify testing
    int cmd;
    int value;
    
    if (size > 0) {
        value = data[0];
        cmd = (size > 1) ? (data[1] % 6) + 1 : 1; // Map to 1-6 for our 6 target functions
    } else {
        value = 0;
        cmd = 1;
    }

    aom_codec_err_t res;

    switch (cmd) {
        case 1: // AV1E_SET_ENABLE_DIST_WTD_COMP
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value);
            break;
        case 2: // AV1E_SET_FILM_GRAIN_TEST_VECTOR
            // This control usually expects a pointer to a struct, but for fuzzing we try passing an int cast
            // or a pointer to the fuzz data if size is sufficient.
            if (size >= sizeof(int)) {
                res = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void *)(uintptr_t)value);
            } else {
                res = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, NULL);
            }
            break;
        case 3: // AV1E_SET_ENABLE_DUAL_FILTER
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);
            break;
        case 4: // AV1E_SET_QUANT_B_ADAPT
            res = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, value);
            break;
        case 5: // AV1E_SET_ENABLE_ANGLE_DELTA
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value);
            break;
        case 6: // AV1E_SET_TUNE_CONTENT
            // Tune content usually expects an enum, we fuzz with int
            res = aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, value);
            break;
        default:
            return 0;
    }

    return (res == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_263(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices used for configuration
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int ret = 0;

    // 1. Prepare environment
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize default configuration
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Fuzz configuration parameters safely based on input
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.rc_target_bitrate = (Data[0] % 10000) + 100;

    // Initialize the encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // 2. & 3. Invoke target functions diversely
    // We pass a slice of the data to try and trigger different code paths
    ret = try_encode_and_control(&ctx, Data, Size);

    // Try again with modified data (flip bits) to explore more states
    if (Size > 0) {
        uint8_t modified_data[256];
        size_t copy_size = (Size < 256) ? Size : 256;
        memcpy(modified_data, Data, copy_size);
        modified_data[0] ^= 0xFF; // Flip first byte
        try_encode_and_control(&ctx, modified_data, copy_size);
    }

    // 4. Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}