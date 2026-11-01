// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec && codec->priv) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int value;

    // Initialize codec context
    memset(&codec, 0, sizeof(codec));

    // Get AV1 encoder interface using the correct function name
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal configuration from fuzz input if possible
    if (Size > 0) {
        cfg.g_w = (Data[0] % 256) + 16; // Ensure width is at least 16
        cfg.g_h = (Size > 1 ? Data[1] % 256 + 16 : 16); // Ensure height is at least 16
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
    }

    // Initialize the codec
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract a value from input for control commands
    value = (Size > 0) ? Data[0] : 0;

    // Fuzz AV1E_SET_ENABLE_MASKED_COMP
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_MASKED_COMP, value);
    (void)ret; // Ignore return for fuzzing exploration

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_COMP, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_PAETH_INTRA, value);
    (void)ret;

    // Cleanup
    cleanup_codec(&codec);

    return 0;
}