// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data) return 0;

    int ret = 0;
    int val;

    // AV1E_SET_ENABLE_FILTER_INTRA
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);
        data += sizeof(int);
        size -= sizeof(int);
    }

    // AV1E_SET_REDUCED_REFERENCE_SET
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret |= aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
        data += sizeof(int);
        size -= sizeof(int);
    }

    // AV1E_SET_ENABLE_REF_FRAME_MVS
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
        data += sizeof(int);
        size -= sizeof(int);
    }

    // AV1E_SET_ENABLE_ONESIDED_COMP
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);
        data += sizeof(int);
        size -= sizeof(int);
    }

    // AV1E_SET_COLOR_PRIMARIES
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret |= aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, val);
        data += sizeof(int);
        size -= sizeof(int);
    }

    // AV1E_SET_ALLOW_WARPED_MOTION
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret |= aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);
        data += sizeof(int);
        size -= sizeof(int);
    }

    return ret;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Initialize encoder config
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use fuzzer data to set some config values safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 8) + 1;
    cfg.g_error_resilient = (Data[6] % 2);
    cfg.g_lag_in_frames = (Data[7] % 32);
    cfg.g_pass = (Data[8] % 3);
    cfg.g_usage = (Data[9] % 4);

    // Initialize encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data + 10, Size - 10);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}