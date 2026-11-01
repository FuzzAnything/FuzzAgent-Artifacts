// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static int try_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    // We don't necessarily fail on error, as the fuzzer might be testing error paths
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_190(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    int width = (Data[0] % 256) + 16;
    int height = (Data[1] % 256) + 16;
    int bit_depth = (Data[2] % 2) + 8; // 8 or 9 or 10
    int fps_num = (Data[3] % 120) + 1;
    int fps_den = 1;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the specific control functions
    // AV1E_SET_ENABLE_DIST_WTD_COMP
    if (Size > 4) {
        int val = Data[4] % 2;
        try_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);
    }

    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (Size > 5) {
        int val = Data[5] % 2;
        try_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);
    }

    // AV1E_SET_QUANT_B_ADAPT
    if (Size > 6) {
        int val = Data[6] % 2;
        try_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, val);
    }

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (Size > 7) {
        int val = Data[7] % 2;
        try_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
    }

    // AV1E_SET_TUNE_CONTENT
    if (Size > 8) {
        // AOM_CONTENT_TYPE has a few values, let's map to 0-2
        int val = Data[8] % 3; 
        try_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val);
    }

    // AV1E_SET_ENABLE_RECT_TX
    if (Size > 9) {
        int val = Data[9] % 2;
        try_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}