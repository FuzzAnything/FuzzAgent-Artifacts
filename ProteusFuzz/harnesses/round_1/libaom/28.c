// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_QM_V at aomcx.h:2051:1 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_QM at aomcx.h:2032:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_INTER_DCT_ONLY at aomcx.h:2201:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_REDUCED_TX_TYPE_SET at aomcx.h:2195:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS at aomcx.h:1942:1 in aomcx.h
// aom_codec_control_typechecked_AV1_SET_TILE_MODE at aomdx.h:528:1 in aomdx.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    memset(cfg, 0, sizeof(*cfg));
    aom_codec_enc_config_default(iface, cfg, 0);

    if (size > 0) {
        cfg->g_w = (data[0] % 128) + 16;
        cfg->g_h = (size > 1 ? data[1] % 128 : 0) + 16;
    } else {
        cfg->g_w = 128;
        cfg->g_h = 128;
    }
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    unsigned int val;

    if (Size < 8) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    val = (Data[2] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_QM(&ctx, AV1E_SET_ENABLE_QM, val);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    val = (Data[3] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_INTER_DCT_ONLY(&ctx, AV1E_SET_INTER_DCT_ONLY, val);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    val = (Data[4] % 2);
    ret = aom_codec_control_typechecked_AV1E_SET_REDUCED_TX_TYPE_SET(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    val = (Data[5] % 8) + 1;
    ret = aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS(&ctx, AV1E_SET_TILE_COLUMNS, val);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    val = (Data[6] % 4);
    ret = aom_codec_control_typechecked_AV1_SET_TILE_MODE(&ctx, AV1_SET_TILE_MODE, val);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    val = (Data[7] % 256);
    ret = aom_codec_control_typechecked_AV1E_SET_QM_V(&ctx, AV1E_SET_QM_V, val);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    cleanup_encoder(&ctx);
    return 0;
}