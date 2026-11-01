// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE at aomcx.h:2153:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH at aomcx.h:2285:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ at aomcx.h:2222:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG at aomcx.h:2252:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_CQ_LEVEL at aomcx.h:1917:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_STATIC_THRESHOLD at aomcx.h:1899:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int init_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx) != 0) {
        return 0;
    }

    // Extract values from input
    int val1 = Data[0];
    int val2 = Size > 1 ? Data[1] : 0;
    unsigned int val3 = Size > 2 ? Data[2] : 0;
    int val4 = Size > 3 ? Data[3] : 0;
    int val5 = Size > 4 ? Data[4] : 0;
    int val6 = Size > 5 ? Data[5] : 0;

    // 1. AV1E_SET_ENABLE_PALETTE
    aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE(&ctx, AV1E_SET_ENABLE_PALETTE, val1);

    // 2. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val2);

    // 3. AV1E_SET_MODE_COST_UPD_FREQ
    aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val3);

    // 4. AV1E_ENABLE_EXT_TILE_DEBUG
    aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val4);

    // 5. AOME_SET_CQ_LEVEL
    aom_codec_control_typechecked_AOME_SET_CQ_LEVEL(&ctx, AOME_SET_CQ_LEVEL, val5);

    // 6. AOME_SET_STATIC_THRESHOLD
    aom_codec_control_typechecked_AOME_SET_STATIC_THRESHOLD(&ctx, AOME_SET_STATIC_THRESHOLD, val6);

    aom_codec_destroy(&ctx);

    return 0;
}