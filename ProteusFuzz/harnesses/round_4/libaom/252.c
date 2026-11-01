// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AOME_SET_TUNING at aomcx.h:1914:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION at aomcx.h:2020:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_TX64 at aomcx.h:2081:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DISABLE_TRELLIS_QUANT at aomcx.h:2029:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ROW_MT at aomcx.h:1939:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_TX at aomcx.h:2087:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t* init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;

    if (!data || size == 0) {
        return NULL;
    }

    // Use the correct function name for AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return NULL;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to set some config parameters safely
    cfg.g_w = (data[0] % 256) + 16;
    cfg.g_h = (data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 100) + 1;
    cfg.rc_target_bitrate = (data[3] % 100) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_252(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    // Extract values from input for control commands
    int tuning = Data[0] % 10;
    unsigned int enable_restoration = (Data[1] % 2) ? 1 : 0;
    int enable_tx64 = (Data[2] % 2) ? 1 : 0;
    int disable_trellis_quant = (Data[3] % 2) ? 1 : 0;
    int row_mt = Data[4] % 4;
    int enable_rect_tx = (Data[5] % 2) ? 1 : 0;

    // Invoke target API functions with type-checked control
    // The macro expands to: aom_codec_control_typechecked_ID(ctx, ID, data)
    aom_codec_control_typechecked_AOME_SET_TUNING(ctx, AOME_SET_TUNING, tuning);
    aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION(ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
    aom_codec_control_typechecked_AV1E_SET_ENABLE_TX64(ctx, AV1E_SET_ENABLE_TX64, enable_tx64);
    aom_codec_control_typechecked_AV1E_SET_DISABLE_TRELLIS_QUANT(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis_quant);
    aom_codec_control_typechecked_AV1E_SET_ROW_MT(ctx, AV1E_SET_ROW_MT, row_mt);
    aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_TX(ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}