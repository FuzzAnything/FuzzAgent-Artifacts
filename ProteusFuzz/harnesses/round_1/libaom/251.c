// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int width = 32;
    int height = 32;

    // Safely extract width and height if enough data is available
    if (size >= 8) {
        width = *(const int32_t *)(data);
        height = *(const int32_t *)(data + 4);
    }

    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 8192 || height > 8192) {
        width = 32;
        height = 32;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_251(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder context
    if (!init_encoder_context(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    val = Data[0] % 2;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    (void)ret;

    // Fuzz AOME_SET_ARNR_STRENGTH
    if (Size > 1) {
        val = Data[1];
        ret = aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_COLOR_RANGE
    if (Size > 2) {
        val = Data[2] % 2; // 0 or 1
        ret = aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, val);
        (void)ret;
    }

    // Fuzz AOME_SET_ARNR_MAXFRAMES
    if (Size > 3) {
        val = Data[3];
        ret = aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val);
        (void)ret;
    }

    // Fuzz AOME_SET_CQ_LEVEL
    if (Size > 4) {
        val = Data[4];
        ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    if (Size > 5) {
        val = Data[5];
        ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
        (void)ret;
    }

    cleanup_codec(&ctx);
    return 0;
}