// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
    if (cfg) {
        free(cfg);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx, aom_codec_dec_cfg_t *cfg) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
    if (cfg) {
        free(cfg);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    uint8_t mode = Data[0] % 2; // 0 for encoder, 1 for decoder
    int32_t val = (Size > 1) ? (int32_t)(Data[1] * 256 + (Size > 2 ? Data[2] : 0)) : 0;

    if (mode == 0) {
        // Encoder path
        aom_codec_ctx_t ctx = {0};
        aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t *cfg = (aom_codec_enc_cfg_t *)malloc(sizeof(aom_codec_enc_cfg_t));
        if (!cfg) return 0;

        if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
            cleanup_encoder(&ctx, cfg);
            return 0;
        }

        cfg->g_w = 32;
        cfg->g_h = 32;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 100;

        if (aom_codec_enc_init(&ctx, iface, cfg, 0) != AOM_CODEC_OK) {
            cleanup_encoder(&ctx, cfg);
            return 0;
        }

        // Fuzz AV1E_SET_ENABLE_QM
        aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val);
        
        // Fuzz AV1E_SET_ENABLE_PALETTE
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val);
        
        // Fuzz AV1E_SET_ENABLE_ORDER_HINT
        aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, val);
        
        // Fuzz AV1E_SET_TILE_COLUMNS
        aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, val);
        
        // Fuzz AV1E_SET_QM_U
        aom_codec_control(&ctx, AV1E_SET_QM_U, val);

        cleanup_encoder(&ctx, cfg);
    } else {
        // Decoder path
        aom_codec_ctx_t ctx = {0};
        aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t *cfg = (aom_codec_dec_cfg_t *)malloc(sizeof(aom_codec_dec_cfg_t));
        if (!cfg) return 0;

        if (aom_codec_dec_init(&ctx, iface, cfg, 0) != AOM_CODEC_OK) {
            cleanup_decoder(&ctx, cfg);
            return 0;
        }

        int32_t hint = 0;
        // Fuzz AOMD_GET_ORDER_HINT
        aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &hint);

        cleanup_decoder(&ctx, cfg);
    }

    return 0;
}