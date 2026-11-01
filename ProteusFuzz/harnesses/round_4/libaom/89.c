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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t codec_ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get the AV1 encoder interface explicitly
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config using the interface
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic config values from input data to vary states
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    // Initialize the encoder context
    ret = aom_codec_enc_init(&codec_ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    int coeff_cost_upd_freq = Data[4] % 100;
    aom_codec_control(&codec_ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    int target_seq_level_idx = Data[5] % 10;
    aom_codec_control(&codec_ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, target_seq_level_idx);

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    int enable_ref_frame_mvs = (Data[6] % 2) ? 1 : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs);

    // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
    int allow_warped_motion = (Data[7] % 2) ? 1 : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_ALLOW_WARPED_MOTION, allow_warped_motion);

    // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
    int enable_filter_intra = (Data[8] % 2) ? 1 : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra);

    // Fuzz AV1E_SET_S_FRAME_MODE
    int s_frame_mode = Data[9] % 3; // Assuming valid modes are 0, 1, 2
    aom_codec_control(&codec_ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);

    // Cleanup
    cleanup_codec(&codec_ctx);

    return 0;
}