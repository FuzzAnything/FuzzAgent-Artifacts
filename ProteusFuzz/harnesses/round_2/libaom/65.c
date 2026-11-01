// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIRECTIONAL_INTRA at aomcx.h:2282:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES at aomcx.h:2213:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_AUTO_INTRA_TOOLS_OFF at aomcx.h:2303:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY at aomcx.h:1969:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX at aomcx.h:2084:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TARGET_SEQ_LEVEL_IDX at aomcx.h:2005:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int32_t val;
    int32_t val2;
    uint32_t val3;
    uint32_t val4;
    int32_t val5;
    int32_t val6;

    // Initialize codec context
    memset(&codec, 0, sizeof(codec));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration with defaults
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Initialize the codec
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz input to drive API calls
    val = (int32_t)Data[0];
    val2 = (int32_t)Data[1];
    val3 = (uint32_t)Data[2];
    val4 = (uint32_t)Data[3];
    val5 = (int32_t)Data[4];
    val6 = (int32_t)Data[5];

    // Call target API functions with fuzzed values
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_DIRECTIONAL_INTRA(aom_codec_ctx_t *ctx, int ctrl, int data)
    aom_codec_control_typechecked_AV1E_SET_ENABLE_DIRECTIONAL_INTRA(&codec, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
    
    // Signature: aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES(aom_codec_ctx_t *ctx, int ctrl, int data)
    aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, val2);
    
    // Signature: aom_codec_control_typechecked_AV1E_SET_AUTO_INTRA_TOOLS_OFF(aom_codec_ctx_t *ctx, int ctrl, unsigned int data)
    aom_codec_control_typechecked_AV1E_SET_AUTO_INTRA_TOOLS_OFF(&codec, AV1E_SET_AUTO_INTRA_TOOLS_OFF, val3);
    
    // Signature: aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY(aom_codec_ctx_t *ctx, int ctrl, unsigned int data)
    aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY(&codec, AV1E_SET_NOISE_SENSITIVITY, val4);
    
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX(aom_codec_ctx_t *ctx, int ctrl, int data)
    aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX(&codec, AV1E_SET_ENABLE_FLIP_IDTX, val5);
    
    // Signature: aom_codec_control_typechecked_AV1E_SET_TARGET_SEQ_LEVEL_IDX(aom_codec_ctx_t *ctx, int ctrl, int data)
    aom_codec_control_typechecked_AV1E_SET_TARGET_SEQ_LEVEL_IDX(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val6);

    // Cleanup
    cleanup_codec(&codec);

    return 0;
}