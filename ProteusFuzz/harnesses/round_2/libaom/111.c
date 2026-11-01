// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_control(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec || !data || size == 0) return 0;

    // AV1E_ENABLE_SB_QP_SWEEP
    if (size >= 1) {
        int val = data[0] % 2;
        aom_codec_control(codec, AV1E_ENABLE_SB_QP_SWEEP, &val);
    }

    // AV1E_SET_ENABLE_TPL_MODEL
    if (size >= 2) {
        int val = (data[1] % 2);
        aom_codec_control(codec, AV1E_SET_ENABLE_TPL_MODEL, &val);
    }

    // AV1E_SET_ENABLE_ONESIDED_COMP
    if (size >= 3) {
        int val = (data[2] % 2);
        aom_codec_control(codec, AV1E_SET_ENABLE_ONESIDED_COMP, &val);
    }

    // AV1E_SET_LOOPFILTER_CONTROL
    if (size >= 4) {
        int val = data[3];
        aom_codec_control(codec, AV1E_SET_LOOPFILTER_CONTROL, &val);
    }

    // AV1E_SET_CDF_UPDATE_MODE
    if (size >= 5) {
        int val = data[4] % 4; 
        aom_codec_control(codec, AV1E_SET_CDF_UPDATE_MODE, &val);
    }

    // AV1E_SET_ENABLE_WARPED_MOTION
    if (size >= 6) {
        int val = (data[5] % 2);
        aom_codec_control(codec, AV1E_SET_ENABLE_WARPED_MOTION, &val);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Initialize encoder config
    ret = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Set some basic config values
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize the codec context
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz the control functions
    fuzz_encoder_control(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}