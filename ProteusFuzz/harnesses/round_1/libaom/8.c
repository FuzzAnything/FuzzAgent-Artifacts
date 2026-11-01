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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Ensure minimum size to safely access Data[0] through Data[9]
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use the standard interface retrieval function for AV1 encoder
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    int inter_dct_only = (Data[4] % 2);
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    // Fuzz AV1E_SET_ENABLE_RECT_TX
    int enable_rect_tx = (Data[5] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);

    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    int enable_diff_wtd_comp = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd_comp);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    int cdf_update_mode = (Data[7] % 3); // Assuming 3 modes
    aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);

    // Fuzz AV1E_SET_ENABLE_TX64
    int enable_tx64 = (Data[8] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX64, enable_tx64);

    // Fuzz AV1E_SET_TUNE_CONTENT
    int tune_content = (Data[9] % 3); // Assuming 3 content types
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}