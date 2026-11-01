// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                        uint8_t width, uint8_t height) {
    if (aom_codec_enc_config_default("av1", cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    // Removed g_row_mt as it is not present in the struct
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Extract dimensions from input to vary test states
    uint8_t width = Data[0] % 256;
    uint8_t height = Data[1] % 256;
    if (width == 0) width = 16;
    if (height == 0) height = 16;

    if (!init_encoder(&ctx, &cfg, width, height)) {
        return 0;
    }

    // Extract control values from input data
    int val1 = (Size > 2) ? (Data[2] % 2) : 0;
    int val2 = (Size > 3) ? (Data[3] % 2) : 0;
    int val3 = (Size > 4) ? (Data[4] % 2) : 0;
    int val4 = (Size > 5) ? (Data[5] % 2) : 0;
    int val5 = (Size > 6) ? (Data[6] % 2) : 0;
    int val6 = (Size > 7) ? (Data[7] % 2) : 0;

    // Invoke target control functions
    // Corrected identifiers based on error message (removed AOM_CODEC_ prefix)
    
    // AV1E_SET_ENABLE_DNL_DENOISING
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val1);
    
    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val2);
    
    // AV1E_SET_ENABLE_ORDER_HINT
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, val3);
    
    // AV1E_SET_RENDER_SIZE: Requires width and height
    int render_w = (Size > 8) ? (Data[8] % 256) : width;
    int render_h = (Size > 9) ? (Data[9] % 256) : height;
    if (render_w == 0) render_w = width;
    if (render_h == 0) render_h = height;
    aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_w, render_h);
    
    // AV1E_SET_ENABLE_OVERLAY
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val4);
    
    // AV1E_SET_REDUCED_TX_TYPE_SET
    aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val5);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}