// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    uint8_t val = data[0];
    int ret = 0;

    // AOME_SET_CQ_LEVEL: expects an int (0-255)
    ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val);
    if (ret != AOM_CODEC_OK) return 0;

    // AV1E_SET_ENABLE_REF_FRAME_MVS: expects an int (0 or 1)
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val & 1);
    if (ret != AOM_CODEC_OK) return 0;

    // AV1E_SET_COLOR_PRIMARIES: expects an int (AOM_CP_* enum)
    ret = aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, val % 10);
    if (ret != AOM_CODEC_OK) return 0;

    // AV1E_SET_ALLOW_WARPED_MOTION: expects an int (0 or 1)
    ret = aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val & 1);
    if (ret != AOM_CODEC_OK) return 0;

    // AV1E_SET_TRANSFER_CHARACTERISTICS: expects an int (AOM_TC_* enum)
    ret = aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val % 10);
    if (ret != AOM_CODEC_OK) return 0;

    // AOME_SET_SHARPNESS: expects an int (0-7)
    ret = aom_codec_control(ctx, AOME_SET_SHARPNESS, val % 8);
    if (ret != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = Data[0] + 16;
    cfg.g_h = Data[1] + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}