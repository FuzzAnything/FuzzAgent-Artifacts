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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_FLIP_IDTX
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AV1E_SET_ENABLE_INTERINTRA_WEDGE
 * - AV1E_SET_ENABLE_DIRECTIONAL_INTRA
 * - AV1E_SET_MAX_REFERENCE_FRAMES
 * - AV1E_SET_TARGET_SEQ_LEVEL_IDX
 */

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Use the standard interface function to get the AV1 encoder
    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Set minimal valid config for encoding
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    // Interpret input data to drive diverse control function calls
    uint8_t control_selector = Data[0] % 6;
    int value;

    if (Size > 1) {
        value = (int)Data[1];
    } else {
        value = 0;
    }

    aom_codec_err_t ret = AOM_CODEC_OK;

    switch (control_selector) {
        case 0:
            // AV1E_SET_ENABLE_FLIP_IDTX (0 or 1)
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_FLIP_IDTX, value % 2);
            break;
        case 1:
            // AV1E_SET_NOISE_SENSITIVITY (0-63 typically)
            ret = aom_codec_control(&codec, AV1E_SET_NOISE_SENSITIVITY, value % 64);
            break;
        case 2:
            // AV1E_SET_ENABLE_INTERINTRA_WEDGE (0 or 1)
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value % 2);
            break;
        case 3:
            // AV1E_SET_ENABLE_DIRECTIONAL_INTRA (0 or 1)
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value % 2);
            break;
        case 4:
            // AV1E_SET_MAX_REFERENCE_FRAMES (0-7 typically)
            ret = aom_codec_control(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, value % 8);
            break;
        case 5:
            // AV1E_SET_TARGET_SEQ_LEVEL_IDX (0-10 typically)
            ret = aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, value % 11);
            break;
        default:
            break;
    }

    cleanup_encoder(&codec);

    return 0;
}