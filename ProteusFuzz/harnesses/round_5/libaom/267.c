// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

/*
 * Fuzz driver for libaom encoder control functions.
 * Uses standard aom_codec_control API with corresponding control flags.
 */

static int initialize_codec(aom_codec_ctx_t *ctx) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Get the AV1 encoder interface using the correct function
    iface = aom_codec_av1_cx();
    if (!iface) return 1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 1;

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 1;

    return 0;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static int fuzz_control_flags(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret;
    int val;
    aom_rc_funcs_t rc_funcs;

    if (size < 1) return 0;

    // Use data to determine the control flag and value
    int flag_idx = data[0] % 6;
    val = (size > 1) ? data[1] : 0;

    switch (flag_idx) {
        case 0:
            // AV1E_SET_ENABLE_INTRABC
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val & 1);
            break;
        case 1:
            // AV1E_SET_ENABLE_TX_SIZE_SEARCH
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val & 1);
            break;
        case 2:
            // AV1E_SET_EXTERNAL_RATE_CONTROL
            memset(&rc_funcs, 0, sizeof(rc_funcs));
            rc_funcs.rc_type = (aom_rc_type_t)(val % 7);
            ret = aom_codec_control(ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
            break;
        case 3:
            // AOME_SET_TUNING
            ret = aom_codec_control(ctx, AOME_SET_TUNING, val % 3);
            break;
        case 4:
            // AV1E_SET_TUNE_CONTENT
            ret = aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, val % 3);
            break;
        case 5:
            // AV1E_SET_CDF_UPDATE_MODE
            ret = aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val % 3);
            break;
        default:
            return 0;
    }

    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int result;

    if (Size == 0) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (initialize_codec(&ctx) != 0) {
        return 0;
    }

    // Execute the fuzzing logic
    result = fuzz_control_flags(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return result;
}