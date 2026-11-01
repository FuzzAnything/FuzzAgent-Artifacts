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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Generate random control values from input
    int palette_val = (Size > 2) ? (Data[2] % 2) : 0;
    int tx_size_val = (Size > 3) ? (Data[3] % 2) : 0;
    int cfl_val = (Size > 4) ? (Data[4] % 2) : 0;
    int global_motion_val = (Size > 5) ? (Data[5] % 2) : 0;
    int intra_tx_val = (Size > 6) ? (Data[6] % 2) : 0;
    int tune_content_val = (Size > 7) ? (Data[7] % 3) : 0;

    // Fuzz AV1E_SET_ENABLE_PALETTE
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PALETTE, palette_val);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, tx_size_val);

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, cfl_val);

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, global_motion_val);

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_tx_val);

    // Fuzz AV1E_SET_TUNE_CONTENT
    fuzz_encoder_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content_val);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}