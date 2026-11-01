// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return 0;
    
    // Try to invoke the control with the provided argument
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, arg);
    
    // We don't care about the return value for fuzzing, just that it didn't crash
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    
    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration with default values
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration from fuzz input
    cfg.g_w = Data[0] > 0 ? Data[0] * 16 : 16;
    cfg.g_h = (Size > 1 ? Data[1] : 1) > 0 ? (Size > 1 ? Data[1] : 1) * 16 : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize the encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare arguments for control functions based on input data
    int enable_val = (Size > 0) ? (Data[0] % 2) : 0;
    int boost_val = (Size > 1) ? (Data[1] % 100) : 0;
    int cfl_val = (Size > 2) ? (Data[2] % 2) : 0;
    int tpl_val = (Size > 3) ? (Data[3] % 2) : 0;
    int sb_qp_val = (Size > 4) ? (Data[4] % 2) : 0;
    int qm_v_val = (Size > 5) ? (Data[5] % 100) : 0;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRABC, &enable_val);

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    fuzz_encoder_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, &boost_val);

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, &cfl_val);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, &tpl_val);

    // Fuzz AV1E_ENABLE_SB_QP_SWEEP
    fuzz_encoder_control(&ctx, AV1E_ENABLE_SB_QP_SWEEP, &sb_qp_val);

    // Fuzz AV1E_SET_QM_V
    fuzz_encoder_control(&ctx, AV1E_SET_QM_V, &qm_v_val);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}