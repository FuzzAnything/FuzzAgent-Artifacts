// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure basic settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = Data[2] % 100 + 1;
    cfg.g_timebase.den = Data[3] % 100 + 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = Data[4] % 3; // 0, 1, 2
    int val2 = Data[5] % 2; // 0, 1
    int val3 = Data[6] % 100; // 0-99
    int val4 = Data[7] % 2; // 0, 1
    int val5 = Data[8] % 2; // 0, 1
    int val6 = Data[9] % 3; // 0, 1, 2

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, val1);

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val2);

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    fuzz_encoder_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val3);

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val4);

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val5);

    // Fuzz AV1E_SET_S_FRAME_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, val6);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}