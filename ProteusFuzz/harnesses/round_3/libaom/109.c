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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_CFL_INTRA
 * - AOME_SET_TUNING
 * - AV1E_SET_QM_MIN
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_MTU
 * - AV1E_SET_QM_U
 */

static int run_fuzz_test(const uint8_t *data, size_t size) {
    if (size < 24) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    uint32_t param1 = *(const uint32_t *)(data);
    uint32_t param2 = *(const uint32_t *)(data + 4);
    uint32_t param3 = *(const uint32_t *)(data + 8);
    uint32_t param4 = *(const uint32_t *)(data + 12);
    uint32_t param5 = *(const uint32_t *)(data + 16);
    uint32_t param6 = *(const uint32_t *)(data + 20);

    /* Initialize encoder context */
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    /* Get AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    /* Set default configuration */
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Apply some fuzzed values to configuration to explore different states */
    cfg.g_w = (param1 % 4096) + 16;
    cfg.g_h = (param2 % 4096) + 16;
    cfg.g_timebase.num = (param3 % 1000) + 1;
    cfg.g_timebase.den = (param4 % 1000) + 1;
    cfg.rc_target_bitrate = (param5 % 1000000) + 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;

    /* Initialize encoder */
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    /*
     * Fuzz the target control functions using aom_codec_control.
     */

    /* AV1E_SET_ENABLE_CFL_INTRA: expects int (0 or 1) */
    int cfl_intra = (param1 % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, cfl_intra);

    /* AOME_SET_TUNING: expects aom_tune_e (enum) */
    /* Valid values are typically 0 (AOM_TUNE_PSNR) and 1 (AOM_TUNE_SSIM) */
    int tuning = (param2 % 2);
    aom_codec_control(&ctx, AOME_SET_TUNING, tuning);

    /* AV1E_SET_QM_MIN: expects int */
    int qm_min = param3 % 64;
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, qm_min);

    /* AV1E_SET_ENABLE_INTRABC: expects int (0 or 1) */
    int intrabc = (param4 % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, intrabc);

    /* AV1E_SET_MTU: expects int */
    int mtu = (param5 % 1024) + 1;
    aom_codec_control(&ctx, AV1E_SET_MTU, mtu);

    /* AV1E_SET_QM_U: expects int */
    int qm_u = param6 % 64;
    aom_codec_control(&ctx, AV1E_SET_QM_U, qm_u);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 24) {
        return 0;
    }

    run_fuzz_test(Data, Size);

    return 0;
}