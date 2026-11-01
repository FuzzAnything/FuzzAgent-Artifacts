// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    if (size < 4) return 0;

    uint32_t width = (data[0] << 8) | data[1];
    uint32_t height = (data[2] << 8) | data[3];

    if (width == 0 || height == 0) {
        width = 128;
        height = 128;
    }
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder_context(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz control functions with diverse inputs derived from Data
    // 1. aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG
    // Assuming it takes an int value (0 or 1)
    if (Size > 0) {
        int val1 = Data[0] & 1;
        aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val1);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ
    // Assuming it takes an int value
    if (Size > 1) {
        int val2 = (Data[1] * Data[2]) % 100;
        aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val2);
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH
    // Assuming it takes an int value (0 or 1)
    if (Size > 2) {
        int val3 = Data[2] & 1;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val3);
    }

    // 4. aom_codec_control_typechecked_AOME_SET_MAX_INTRA_BITRATE_PCT
    // Assuming it takes an int value (percentage)
    if (Size > 3) {
        int val4 = Data[3] % 101; // 0 to 100
        aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val4);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_ENABLE_DIFF_WTD_COMP
    // Assuming it takes an int value (0 or 1)
    if (Size > 4) {
        int val5 = Data[4] & 1;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val5);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    // Assuming it takes an int value (0 or 1)
    if (Size > 5) {
        int val6 = Data[5] & 1;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val6);
    }

    // Cleanup
    cleanup_encoder_context(&ctx);

    return 0;
}