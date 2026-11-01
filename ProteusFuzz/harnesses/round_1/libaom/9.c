// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static aom_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int width = 16;
    int height = 16;
    int fps_num = 30;
    int fps_den = 1;

    if (size >= 12) {
        width = (data[0] % 128) + 16;
        height = (data[1] % 128) + 16;
        fps_num = (data[2] % 120) + 1;
        fps_den = (data[3] % 120) + 1;
        if (fps_den == 0) fps_den = 1;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 0;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 100;
    cfg.rc_overshoot_pct = 100;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    int val;
    aom_codec_err_t ret;

    // Extract values from input data to vary control parameters
    if (Size >= 1) val = Data[0] % 2;
    else val = 0;

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    (void)ret;

    if (Size >= 2) val = Data[1] % 4;
    else val = 0;

    // Fuzz AV1E_SET_DELTALF_MODE
    ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val);
    (void)ret;

    if (Size >= 3) val = Data[2] % 4;
    else val = 0;

    // Fuzz AV1E_SET_AQ_MODE
    ret = aom_codec_control(ctx, AV1E_SET_AQ_MODE, val);
    (void)ret;

    if (Size >= 4) val = Data[3] % 2;
    else val = 0;

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    (void)ret;

    if (Size >= 5) val = Data[4] % 2;
    else val = 0;

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    ret = aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val);
    (void)ret;

    if (Size >= 6) val = Data[5] % 16;
    else val = 0;

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    ret = aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
    (void)ret;

    // Additional diversity: try invalid values
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, -1);
    (void)ret;
    ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, 100);
    (void)ret;

    cleanup_encoder_context(ctx);
    return 0;
}