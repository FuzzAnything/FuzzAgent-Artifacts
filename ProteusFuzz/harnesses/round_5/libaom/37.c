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

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 11) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to seed configuration
    cfg.g_w = (Data[0] % 32) + 16;
    cfg.g_h = (Data[1] % 32) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.g_threads = (Data[4] % 4) + 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Generate random values from input for fuzzing
    int val1 = (Data[5] % 2);
    int val2 = (Data[6] % 2);
    int val3 = (Data[7] % 2);
    int val4 = (Data[8] % 2);
    int val5 = (Data[9] % 2);
    int val6 = (Data[10] % 2);

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val1);

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val2);

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val3);

    // Fuzz AV1E_SET_S_FRAME_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_S_FRAME_MODE, val4);

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val5);

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    fuzz_encoder_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}