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
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    if (res != AOM_CODEC_OK) return 0;
    return 1;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
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
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = (Size > 4) ? (int)(Data[4] % 2) : 0;
    int val2 = (Size > 5) ? (int)(Data[5] % 2) : 0;
    int val3 = (Size > 6) ? (int)(Data[6] % 2) : 0;
    int val4 = (Size > 7) ? (int)(Data[7] % 2) : 0;
    int val5 = (Size > 8) ? (int)(Data[8] % 2) : 0;
    int val6 = (Size > 9) ? (int)(Data[9] % 256) : 0;

    // Invoke target control functions
    fuzz_encoder_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val1);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val2);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val3);
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val4);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val5);
    fuzz_encoder_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}