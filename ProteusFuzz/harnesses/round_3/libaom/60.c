// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int enable_qm, force_video_mode, disable_trellis, enable_ref_frame_mvs, 
        enable_filter_intra;
    int frame_periodic_boost;

    // Parse input data to control parameters
    enable_qm = Data[0] & 0x01;
    force_video_mode = (Data[0] >> 1) & 0x01;
    disable_trellis = (Data[0] >> 2) & 0x01;
    enable_ref_frame_mvs = (Data[0] >> 3) & 0x01;
    enable_filter_intra = (Data[0] >> 4) & 0x01;
    frame_periodic_boost = (Data[0] >> 5) & 0x01;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_QM
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_FORCE_VIDEO_MODE
    if (aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DISABLE_TRELLIS_QUANT
    if (aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_FRAME_PERIODIC_BOOST
    if (aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, frame_periodic_boost) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}