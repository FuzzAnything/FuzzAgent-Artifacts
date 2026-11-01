// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    // Get encoder interface first to ensure it exists and is valid
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to tweak config if Size is sufficient
    if (Size >= 6) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1] % 1024) + 16;
        cfg.g_threads = (Data[2] % 8) + 1;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Data[3] % 60) + 1;
        cfg.rc_target_bitrate = ((uint32_t)Data[4] * 256 + Data[5]) * 1000;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_threads = 1;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
    }

    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Helper to safely call control with fuzzed value
    #define FUZZ_CONTROL(ctrl_id, max_val) \
        do { \
            int val = 0; \
            if (Size > 0) { \
                val = (int)(Data[0] % (max_val + 1)); \
            } \
            aom_codec_control(&ctx, ctrl_id, val); \
        } while(0)

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE (0 or 1)
    FUZZ_CONTROL(AV1E_SET_ENABLE_INTERINTER_WEDGE, 1);

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST (0 or 1)
    FUZZ_CONTROL(AV1E_SET_FRAME_PERIODIC_BOOST, 1);

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS (0 or 1)
    FUZZ_CONTROL(AV1E_SET_ENABLE_REF_FRAME_MVS, 1);

    // Fuzz AV1E_SET_INTER_DCT_ONLY (0 or 1)
    FUZZ_CONTROL(AV1E_SET_INTER_DCT_ONLY, 1);

    // Fuzz AV1E_SET_S_FRAME_MODE (0, 1, or 2)
    FUZZ_CONTROL(AV1E_SET_S_FRAME_MODE, 2);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL (0 or 1)
    FUZZ_CONTROL(AV1E_SET_ENABLE_TPL_MODEL, 1);

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run encoder control fuzzing
    fuzz_encoder_controls(Data, Size);

    return 0;
}