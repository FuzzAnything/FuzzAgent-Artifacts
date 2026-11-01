// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void setup_encoder_config(aom_codec_enc_cfg_t *cfg, size_t size) {
    aom_codec_enc_config_default(NULL, cfg, 0);
    cfg->g_w = size % 1024 + 16;
    cfg->g_h = size % 1024 + 16;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use the correct function name aom_codec_av1_cx()
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    setup_encoder_config(&cfg, Size);

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Helper to safely call control functions with fuzzed data
    int result = AOM_CODEC_OK;

    // Extract control type from input if possible, otherwise cycle through
    unsigned int ctrl_type = Data[0] % 6;
    int arg_val = 0;
    if (Size > 1) {
        arg_val = (int)(Data[1] & 0xFF);
    }

    // Map ctrl_type to specific control commands
    // Note: The actual macro names are used here based on standard libaom controls
    // The user provided names are typechecked wrappers which map to these standard controls
    switch (ctrl_type) {
        case 0: // AV1E_SET_ENABLE_OVERLAY
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_OVERLAY, arg_val ? 1 : 0);
            break;
        case 1: // AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
            result = aom_codec_control(&codec, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, arg_val ? 1 : 0);
            break;
        case 2: // AV1E_SET_ENABLE_PALETTE
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_PALETTE, arg_val ? 1 : 0);
            break;
        case 3: // AV1_SET_TILE_MODE
            // AV1_SET_TILE_MODE usually takes a struct, but we try to pass an int or pointer to fuzzed data
            // For safety in fuzzing, we might just pass a dummy value or try to interpret data as struct
            // Since we don't have the struct layout fully guaranteed, we pass a simple int cast or 0
            result = aom_codec_control(&codec, AV1_SET_TILE_MODE, (void*)(uintptr_t)arg_val);
            break;
        case 4: // AV1E_SET_QM_MIN
            result = aom_codec_control(&codec, AV1E_SET_QM_MIN, arg_val);
            break;
        case 5: // AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
            result = aom_codec_control(&codec, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, arg_val ? 1 : 0);
            break;
        default:
            break;
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}