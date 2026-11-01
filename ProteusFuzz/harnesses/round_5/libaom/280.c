// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec) return 0;

    // Helper to safely read integers from fuzz input
    int32_t val;
    int32_t *ptr = (int32_t *)data;
    size_t idx = 0;

    // Function 1: aom_codec_control_typechecked_AV1E_SET_QM_U
    // Assuming it takes an int32_t value for quantization matrix U
    if (idx + sizeof(int32_t) <= size) {
        val = ptr[idx++];
        aom_codec_control(codec, AV1E_SET_QM_U, val);
    }

    // Function 2: aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH
    // Assuming it takes an int32_t (0 or 1) to enable/disable
    if (idx + sizeof(int32_t) <= size) {
        val = ptr[idx++];
        aom_codec_control(codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    }

    // Function 3: aom_codec_control_typechecked_AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
    // Assuming it takes an int32_t flag
    if (idx + sizeof(int32_t) <= size) {
        val = ptr[idx++];
        aom_codec_control(codec, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, val);
    }

    // Function 4: aom_codec_control_typechecked_AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP
    // Assuming it takes an int32_t value
    if (idx + sizeof(int32_t) <= size) {
        val = ptr[idx++];
        aom_codec_control(codec, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, val);
    }

    // Function 5: aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    // Assuming it takes an int32_t (0 or 1)
    if (idx + sizeof(int32_t) <= size) {
        val = ptr[idx++];
        aom_codec_control(codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
    }

    // Function 6: aom_codec_control_typechecked_AV1E_SET_QM_Y
    // Assuming it takes an int32_t value for quantization matrix Y
    if (idx + sizeof(int32_t) <= size) {
        val = ptr[idx++];
        aom_codec_control(codec, AV1E_SET_QM_Y, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_280(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Initialize configuration with some default values
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the encoder control functions
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}