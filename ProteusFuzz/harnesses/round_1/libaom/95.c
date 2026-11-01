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

static int test_codec_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, ctrl_id, value);
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = Data[5] % 2; // 0 or 1
    int val2 = Data[6] % 100;
    int val3 = Data[7] % 2;
    int val4 = Data[8] % 16;
    int val5 = Data[9] % 2;
    int val6 = Data[10] % 255;

    // Test various control functions using correct enum names
    test_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val1);
    test_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val2);
    test_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val3);
    test_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val4);
    test_codec_control(&ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, val5);
    test_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}