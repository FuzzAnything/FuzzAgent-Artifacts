// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to vary configuration slightly
    cfg.g_w = (size_t)(size > 0 ? (data[0] % 100 + 16) : 16);
    cfg.g_h = (size_t)(size > 1 ? (data[1] % 100 + 16) : 16);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    aom_codec_err_t ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
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

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz various control functions with data-derived values
    int val = (int)(Data[0] % 2); // 0 or 1

    // 1. AV1E_SET_ENABLE_TPL_MODEL
    aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val);

    // 2. AOME_SET_SHARPNESS
    int sharpness = (int)(Data[1] % 7); // 0 to 6
    aom_codec_control(ctx, AOME_SET_SHARPNESS, sharpness);

    // 3. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int tx_search = (int)(Data[2 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, tx_search);

    // 4. AV1E_SET_MODE_COST_UPD_FREQ
    int freq = (int)(Data[3 % Size] % 100);
    aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, freq);

    // 5. AV1E_SET_ENABLE_GLOBAL_MOTION
    int global_motion = (int)(Data[4 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, global_motion);

    // 6. AV1E_SET_DELTALF_MODE
    int dlf_mode = (int)(Data[5 % Size] % 4); // Assuming 0-3 range
    aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, dlf_mode);

    cleanup_encoder_context(ctx);
    return 0;
}