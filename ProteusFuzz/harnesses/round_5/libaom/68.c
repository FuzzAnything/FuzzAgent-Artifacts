// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // The type aom_film_grain_test_vector_t is internal or not exposed in standard headers.
    // We cast the data to void* and pass it directly, assuming the structure size is reasonable.
    // This is a heuristic approach for fuzzing when exact types are unknown.
    if (size >= 1) {
        // Pass the data directly as the argument. The library will handle the cast internally
        // or fail gracefully if the structure is invalid.
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)data);
    }

    // AV1E_SET_PARTITION_INFO_PATH
    // Requires: const char * path
    if (size > 0) {
        char *path = (char *)malloc(size + 1);
        if (path) {
            memcpy(path, data, size);
            path[size] = '\0';
            aom_codec_control(ctx, AV1E_SET_PARTITION_INFO_PATH, path);
            free(path);
        }
    }

    // AV1E_SET_LOOPFILTER_CONTROL
    // Requires: aom_loopfilter_control_t *
    // Since the struct definition might be missing or internal, we pass a pointer to data
    // if size is sufficient to hold at least a few bytes.
    if (size >= sizeof(int)) {
        aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, (void*)data);
    }

    // AV1E_SET_FILM_GRAIN_TABLE
    // Requires: aom_film_grain_table_t *
    if (size >= sizeof(int)) {
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TABLE, (void*)data);
    }

    // AV1E_SET_ENABLE_WARPED_MOTION
    // Requires: int * (0 or 1)
    if (size >= sizeof(int)) {
        int val = *(const int *)data;
        int normalized = (val != 0) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, &normalized);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder config
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to influence config if possible, but keep valid ranges
    cfg.g_w = (Size % 2048) + 16; // Width
    cfg.g_h = (Size % 2048) + 16; // Height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize the encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Test aom_codec_get_global_headers
    // This should be called after configuration is finalized
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        // We must free the buffer as per documentation
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}