// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static void invoke_codec_control_int(aom_codec_ctx_t *ctx, int ctrl, int val) {
    if (ctx) {
        aom_codec_control(ctx, ctrl, val);
    }
}

static void invoke_codec_control_ptr(aom_codec_ctx_t *ctx, int ctrl, void *val) {
    if (ctx) {
        aom_codec_control(ctx, ctrl, val);
    }
}

int LLVMFuzzerTestOneInput_315(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height/bitrate to avoid trivial configs
    // Ensure we have enough data before accessing indices
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.rc_target_bitrate = ((int)Data[2] * 256 + (int)Data[3]);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int val = (int)(Data[4] % 2); // 0 or 1 for boolean-like controls

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH (expects int)
    invoke_codec_control_int(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);

    // Fuzz AOME_SET_MAX_INTRA_BITRATE_PCT (expects int)
    int pct = (int)(Data[5] % 101);
    invoke_codec_control_int(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, pct);

    // Fuzz AV1E_ENABLE_EXT_TILE_DEBUG (expects int)
    int debug = (int)(Data[6] % 2);
    invoke_codec_control_int(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, debug);

    // Fuzz AV1E_SET_BITRATE_ONE_PASS_CBR (expects int)
    int cbr = (int)(Data[7] % 2);
    invoke_codec_control_int(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, cbr);

    // Fuzz AOME_SET_CQ_LEVEL (expects int)
    int cq = (int)(Data[8] % 64);
    invoke_codec_control_int(&ctx, AOME_SET_CQ_LEVEL, cq);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ (expects int)
    int freq = (int)(Data[9] % 100);
    invoke_codec_control_int(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, freq);

    aom_codec_destroy(&ctx);

    return 0;
}