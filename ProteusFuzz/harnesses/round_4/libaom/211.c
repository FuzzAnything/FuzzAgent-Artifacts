// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS at aomcx.h:1942:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_MASKED_COMP at aomcx.h:2105:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT at aomcx.h:2078:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER at aomcx.h:2075:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY at aomcx.h:2150:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_REDUCED_TX_TYPE_SET at aomcx.h:2195:1 in aomcx.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_211(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure width/height to explore different states
    uint32_t width = (Data[0] % 256) + 16;
    uint32_t height = (Data[1] % 256) + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_TILE_COLUMNS
    // Signature: aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS(ctx, AV1E_SET_TILE_COLUMNS, value)
    int tile_cols = (Data[0] % 16) + 1;
    ret = aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_MASKED_COMP
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_MASKED_COMP(ctx, AV1E_SET_ENABLE_MASKED_COMP, value)
    int enable_masked_comp = (Data[1] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_MASKED_COMP(&ctx, AV1E_SET_ENABLE_MASKED_COMP, enable_masked_comp);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT(ctx, AV1E_SET_ENABLE_ORDER_HINT, value)
    int enable_order_hint = (Data[2] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT(&ctx, AV1E_SET_ENABLE_ORDER_HINT, enable_order_hint);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value)
    int enable_intra_edge_filter = (Data[3] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_intra_edge_filter);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY(ctx, AV1E_SET_ENABLE_OVERLAY, value)
    int enable_overlay = (Data[0] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
    (void)ret;

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    // Signature: aom_codec_control_typechecked_AV1E_SET_REDUCED_TX_TYPE_SET(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, value)
    int reduced_tx_type_set = (Data[1] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_REDUCED_TX_TYPE_SET(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, reduced_tx_type_set);
    (void)ret;

    cleanup_codec(&ctx);
    return 0;
}