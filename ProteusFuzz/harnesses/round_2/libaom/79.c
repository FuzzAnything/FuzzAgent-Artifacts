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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.rc_target_bitrate = (Data[4 % Size] % 1000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from input data
    int value1 = (Size > 4) ? (Data[4] % 2) : 0;
    int value2 = (Size > 5) ? (Data[5] % 2) : 0;
    int value3 = (Size > 6) ? (Data[6] % 2) : 0;
    int value4 = (Size > 7) ? (Data[7] % 2) : 0;
    int value5 = (Size > 8) ? (Data[8] % 2) : 0;
    int value6 = (Size > 9) ? (Data[9] % 2) : 0;

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value1);
    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, value2);
    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value3);
    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value4);
    // Fuzz AV1E_SET_INTER_DCT_ONLY
    fuzz_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, value5);
    // Fuzz AV1E_SET_QUANT_B_ADAPT
    fuzz_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, value6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}