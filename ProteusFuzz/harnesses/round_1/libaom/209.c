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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) return -1;
    return 0;
}

int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg) != 0) return 0;

    // Extract values from input to control codec parameters
    int val1 = Data[0];
    int val2 = Data[1 % Size];
    int val3 = Data[2 % Size];
    int val4 = Data[3 % Size];
    int val5 = Data[4 % Size];
    int val6 = Data[5 % Size];

    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_SUPERRES
    // Sets the enable superres flag (0 or 1)
    if (aom_codec_control(&codec, AV1E_SET_ENABLE_SUPERRES, val1 ? 1 : 0) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES
    // Sets color primaries (0-14 range typically)
    int primaries = val2 % 15;
    if (aom_codec_control(&codec, AV1E_SET_COLOR_PRIMARIES, primaries) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_TIMING_INFO_TYPE
    // Sets timing info type (0 or 1)
    if (aom_codec_control(&codec, AV1E_SET_TIMING_INFO_TYPE, val3 ? 1 : 0) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_ALLOW_WARPED_MOTION
    // Sets allow warped motion flag (0 or 1)
    if (aom_codec_control(&codec, AV1E_SET_ALLOW_WARPED_MOTION, val4 ? 1 : 0) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
    // Sets coefficient cost update frequency (0-255)
    int freq = val5;
    if (aom_codec_control(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, freq) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_TARGET_SEQ_LEVEL_IDX
    // Sets target sequence level index (0-127 typically)
    int level_idx = val6 % 128;
    if (aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}