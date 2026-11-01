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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int test_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration from input data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = Data[2] % 100 + 1;
    cfg.g_timebase.den = Data[3] % 100 + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = (Size > 4) ? (Data[4] % 100) : 0;
    int val2 = (Size > 5) ? (Data[5] % 100) : 0;
    int val3 = (Size > 6) ? (Data[6] % 100) : 0;
    int val4 = (Size > 7) ? (Data[7] % 100) : 0;
    int val5 = (Size > 8) ? (Data[8] % 100) : 0;
    int val6 = (Size > 9) ? (Data[9] % 100) : 0;

    // Invoke target control functions with diverse values
    test_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val1);
    test_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val2);
    test_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val3);
    test_codec_control(&ctx, AV1E_SET_AQ_MODE, val4);
    test_codec_control(&ctx, AOME_SET_SHARPNESS, val5);
    test_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}