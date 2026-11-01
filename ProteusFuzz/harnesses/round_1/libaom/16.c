// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom focusing on aom_codec_control and related type-checked macros.
 * Corrected function names and control IDs based on libaom headers.
 */

static aom_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    // Use the correct function name aom_codec_av1_cx which returns the AV1 encoder/decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzz input to vary configuration slightly, but keep it valid
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
        cfg.g_h = (data[1] % 128) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int fuzz_codec_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    // Extract parameters from fuzz data
    uint8_t id_byte = data[0];
    uint8_t val_byte = data[1];

    // Map fuzz data to valid control IDs or invalid ones to test error handling
    int ctrl_id = 0;
    int arg_val = val_byte;

    // Select a control ID based on the first byte
    // We test a mix of valid AV1 encoder controls and invalid IDs
    switch (id_byte % 10) {
        case 0: ctrl_id = AOME_SET_CPUUSED; break;
        case 1: ctrl_id = AV1E_SET_ENABLE_CDEF; break;
        case 2: ctrl_id = AV1E_SET_ENABLE_RESTORATION; break;
        case 3: ctrl_id = AV1E_SET_ENABLE_PALETTE; break; // Target API 1
        case 4: ctrl_id = AV1E_SET_ENABLE_FILTER_INTRA; break; // Target API 5
        case 5: ctrl_id = AV1E_SET_ENABLE_ONESIDED_COMP; break; // Target API 4
        case 6: ctrl_id = AV1E_SET_POSTENCODE_DROP_RTC; break; // Target API 6
        case 7: ctrl_id = AV1E_SET_TIMING_INFO_TYPE; break; // Target API 3
        case 8: ctrl_id = 0; break; // Invalid ID (zero)
        case 9: ctrl_id = 0xFFFFFFFF; break; // Invalid ID (out of range)
    }

    // Call aom_codec_control
    // Note: The type-checked macros are wrappers around this function.
    // Since we don't have the macro definitions for the specific AV1E_* controls,
    // we invoke the base function directly to exercise the logic.
    aom_codec_err_t ret = aom_codec_control(ctx, ctrl_id, arg_val);

    // We don't strictly fail on errors, as we want to explore error paths too.
    (void)ret;

    return 1;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    // Perform multiple control operations to explore state
    // We slice the input data to generate different parameters for each call
    size_t chunk_size = Size / 10;
    if (chunk_size < 2) chunk_size = 2;

    for (size_t i = 0; i < 10; i++) {
        size_t offset = i * chunk_size;
        if (offset + chunk_size > Size) break;

        const uint8_t *chunk = Data + offset;
        fuzz_codec_control(ctx, chunk, chunk_size);
    }

    // Cleanup
    cleanup_encoder_context(ctx);

    return 0;
}