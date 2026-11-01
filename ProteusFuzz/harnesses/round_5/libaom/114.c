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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CQ;
    cfg.g_error_resilient = 1;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input data to control fuzzing parameters
    int val1 = Data[0];
    int val2 = Data[1];
    int val3 = Data[2];
    int val4 = Data[3];

    // Fuzz aom_codec_control_typechecked_AOME_SET_CQ_LEVEL
    // Assuming it expects an int for CQ level
    ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, val1);
    (void)ret;

    // Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_GF_INTERVAL
    // Assuming it expects an int for max GF interval
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val2);
    (void)ret;

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_TPL_MODEL
    // Assuming it expects an int (0 or 1) for enabling TPL model
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val3 & 1);
    (void)ret;

    // Fuzz aom_codec_control_typechecked_AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    // Assuming it expects an int for min pyramid height
    ret = aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val4);
    (void)ret;

    // Fuzz aom_codec_control_typechecked_AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    // Reuse val1 for max pyramid height
    ret = aom_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val1);
    (void)ret;

    // Fuzz aom_codec_control_typechecked_AV1E_SET_MIN_GF_INTERVAL
    // Reuse val2 for min GF interval
    ret = aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val2);
    (void)ret;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}