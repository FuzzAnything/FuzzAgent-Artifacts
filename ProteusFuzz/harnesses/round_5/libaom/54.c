// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 1;

    // Use fuzzing data to configure encoder parameters safely
    if (size > 0) cfg.g_w = (data[0] % 4096) + 16;
    if (size > 1) cfg.g_h = (data[1] % 4096) + 16;
    if (size > 2) cfg.g_threads = (data[2] % 8) + 1;
    if (size > 3) cfg.g_timebase.num = (data[3] % 100) + 1;
    if (size > 4) cfg.g_timebase.den = (data[4] % 100) + 1;
    if (size > 5) cfg.rc_target_bitrate = (data[5] % 10000) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 1;

    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all the control parameters we want to test
    // We need at least 12 bytes to safely access Data[0] through Data[11]
    if (Size < 12) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx_size_search = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    int disable_trellis_quant = (Data[7] % 2);
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis_quant);

    // Fuzz AV1E_SET_QM_MIN
    int qm_min = (Data[8] % 64);
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, qm_min);

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    int enable_global_motion = (Data[9] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    int mode_cost_upd_freq = (Data[10] % 100);
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    int cdf_update_mode = (Data[11] % 4);
    aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}