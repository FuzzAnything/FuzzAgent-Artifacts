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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_MIN_CR
 * - AV1E_SET_ENABLE_DIAGONAL_INTRA
 * - AV1E_SET_QM_U
 * - AV1E_SET_LOSSLESS
 * - AV1E_SET_ENABLE_DIRECTIONAL_INTRA
 * - AV1E_SET_ENABLE_PAETH_INTRA
 */

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    int ret = 0;

    // Get the AV1 encoder interface
    // aom_codec_av1_cx() is defined in aom/aomcx.h
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to set some configuration parameters safely
    cfg.g_w = 64 + (Data[0] % 1024);
    cfg.g_h = 64 + (Data[1] % 1024);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100 + (Data[0] % 1000);

    // Initialize codec context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from fuzz input
    uint8_t min_cr_val = Data[0] % 100;
    int enable_diag = (Data[1] % 2);
    int enable_dir = (Data[1] % 2);
    int enable_paeth = (Data[1] % 2);
    int lossless = (Data[1] % 2);
    uint8_t qm_u_val = Data[0] % 256;

    // AV1E_SET_MIN_CR
    aom_codec_control(&ctx, AV1E_SET_MIN_CR, min_cr_val);

    // AV1E_SET_ENABLE_DIAGONAL_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_diag);

    // AV1E_SET_QM_U
    aom_codec_control(&ctx, AV1E_SET_QM_U, qm_u_val);

    // AV1E_SET_LOSSLESS
    aom_codec_control(&ctx, AV1E_SET_LOSSLESS, lossless);

    // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_dir);

    // AV1E_SET_ENABLE_PAETH_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth);

    // Cleanup
    aom_codec_destroy(&ctx);

    return ret;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    return fuzz_encoder_controls(Data, Size);
}