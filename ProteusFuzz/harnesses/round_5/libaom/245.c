// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int fuzz_encoder_control(const uint8_t *Data, size_t Size, int control_id, int value) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic configuration values
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CQ;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Call the control function with the fuzzed value
    err = aom_codec_control(&ctx, control_id, value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return (err == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use first byte to select which control to fuzz
    int control_id = Data[0] % 6;
    int value = 0;

    // Extract value from input if available
    if (Size > 1) {
        value = (int)(Data[1] & 0xFF);
    }

    // Map control_id to actual control constants
    switch (control_id) {
        case 0:
            // AV1E_SET_ENABLE_INTRABC
            fuzz_encoder_control(Data, Size, AV1E_SET_ENABLE_INTRABC, value);
            break;
        case 1:
            // AV1E_SET_QM_U
            fuzz_encoder_control(Data, Size, AV1E_SET_QM_U, value);
            break;
        case 2:
            // AV1E_SET_SKIP_POSTPROC_FILTERING
            fuzz_encoder_control(Data, Size, AV1E_SET_SKIP_POSTPROC_FILTERING, value);
            break;
        case 3:
            // AV1E_SET_ENABLE_KEYFRAME_FILTERING
            fuzz_encoder_control(Data, Size, AV1E_SET_ENABLE_KEYFRAME_FILTERING, value);
            break;
        case 4:
            // AV1E_SET_CHROMA_SUBSAMPLING_Y
            fuzz_encoder_control(Data, Size, AV1E_SET_CHROMA_SUBSAMPLING_Y, value);
            break;
        case 5:
            // AV1E_SET_ENABLE_CDEF
            fuzz_encoder_control(Data, Size, AV1E_SET_ENABLE_CDEF, value);
            break;
        default:
            break;
    }

    return 0;
}