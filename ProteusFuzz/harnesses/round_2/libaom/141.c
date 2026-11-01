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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret; // Suppress unused variable warning, fuzzing focuses on execution
    return 0;
}

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings slightly
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] % 100) + 100;
    cfg.g_error_resilient = (Data[5] % 3);

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to drive control parameters
    int val1 = Data[6] % 2;
    int val2 = Data[7] % 2;
    int val3 = Data[8] % 2;
    int val4 = Data[9] % 2;
    int val5 = Data[10] % 2;
    int val6 = Data[11] % 2;

    // Fuzz the target control functions
    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val1);

    // AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val2);

    // AV1E_SET_ENABLE_TX64
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_TX64, val3);

    // AV1E_SET_INTER_DCT_ONLY
    fuzz_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val4);

    // AV1E_SET_INTRA_DCT_ONLY
    fuzz_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val5);

    // AV1E_SET_REDUCED_TX_TYPE_SET
    fuzz_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}