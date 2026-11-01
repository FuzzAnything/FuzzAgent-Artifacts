// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

/*
 * Note: The provided function signatures for the target APIs were empty in the prompt.
 * Based on standard libaom patterns, these functions are likely wrappers or internal
 * implementations of aom_codec_control with specific type checking.
 * Since the exact signatures are unknown, this fuzzer attempts to initialize
 * encoder and decoder contexts and invokes the generic aom_codec_control
 * with the known control IDs corresponding to the requested features.
 */

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    int32_t value = 0;
    if (size >= 4) {
        value = *(int32_t *)data;
    } else if (size >= 1) {
        value = (int32_t)data[0];
    }

    // AOME_SET_SHARPNESS
    aom_codec_control(ctx, AOME_SET_SHARPNESS, value);
    
    // AOME_SET_ENABLEAUTOBWDREF
    aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, value);

    // AV1E_SET_ROW_MT
    aom_codec_control(ctx, AV1E_SET_ROW_MT, value);

    // AV1E_SET_ENABLE_TPL_MODEL
    aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, value);

    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    int32_t value = 0;
    if (size >= 4) {
        value = *(int32_t *)data;
    } else if (size >= 1) {
        value = (int32_t)data[0];
    }

    // AV1D_SET_ROW_MT
    aom_codec_control(ctx, AV1D_SET_ROW_MT, value);

    // AV1D_GET_BIT_DEPTH (GET controls usually take a pointer to store result)
    int32_t bit_depth;
    aom_codec_control(ctx, AV1D_GET_BIT_DEPTH, &bit_depth);

    return 1;
}

int LLVMFuzzerTestOneInput_246(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic configuration
    if (Size < 10) return 0;

    // Determine if we should test encoder or decoder based on input
    int mode = Data[0] % 2;
    
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;

    if (mode == 0) {
        // Encoder path
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

        // Use input data to vary config slightly with bounds checking
        cfg.g_w = (Data[1] % 100) + 16;
        cfg.g_h = (Data[2] % 100) + 16;
        // Fixed: g_time_base -> g_timebase
        cfg.g_timebase.num = Data[3] % 30 + 1;
        cfg.g_timebase.den = 1;
        cfg.g_threads = (Data[4] % 4) + 1;
        cfg.rc_target_bitrate = (Data[5] % 1000) + 100;

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_encoder_control(&ctx, Data + 1, Size - 1);

        aom_codec_destroy(&ctx);

    } else {
        // Decoder path
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = (Data[1] % 4) + 1;
        // Fixed: allow_highbitdepth -> allow_lowbitdepth (based on error message)
        cfg.allow_lowbitdepth = (Data[2] % 2);
        // Removed: allow_intrabc (member does not exist in this version)

        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_decoder_control(&ctx, Data + 1, Size - 1);

        aom_codec_destroy(&ctx);
    }

    return 0;
}