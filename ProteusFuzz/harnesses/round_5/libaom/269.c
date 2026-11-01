// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_269(const uint8_t *Data, size_t Size) {
    if (Size < 7) {
        return 0;
    }

    // Use first byte to decide between encoder and decoder
    int use_encoder = Data[0] & 1;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;
    aom_codec_iface_t *iface = NULL;

    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;

        res = aom_codec_enc_init(ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        // Fixed: Removed invalid fields allow_high_bitdepth, allow_intrabc, allow_screencontenttools
        // Only using valid field allow_lowbitdepth if needed, but default 0 is fine for fuzzing
        // cfg.allow_lowbitdepth = 1; 

        res = aom_codec_dec_init(ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }
    }

    // Fuzz various control commands based on input data

    // 1. AV1E_SET_TILE_ROWS (Encoder only)
    if (use_encoder) {
        int tile_rows = (Data[1] % 16) + 1;
        res = aom_codec_control(ctx, AV1E_SET_TILE_ROWS, &tile_rows);
        (void)res;
    }

    // 2. AOMD_GET_TILE_INFO (Decoder only)
    if (!use_encoder) {
        aom_tile_info info;
        memset(&info, 0, sizeof(info));
        res = aom_codec_control(ctx, AOMD_GET_TILE_INFO, &info);
        (void)res;
    }

    // 3. AV1_SET_TILE_MODE (Both)
    {
        int tile_mode = (Data[2] % 2);
        res = aom_codec_control(ctx, AV1_SET_TILE_MODE, &tile_mode);
        (void)res;
    }

    // 4. AV1_INVERT_TILE_DECODE_ORDER (Both)
    {
        int invert = (Data[3] % 2);
        res = aom_codec_control(ctx, AV1_INVERT_TILE_DECODE_ORDER, &invert);
        (void)res;
    }

    // 5. AV1E_SET_TILE_COLUMNS (Encoder only)
    if (use_encoder) {
        int tile_cols = (Data[4] % 16) + 1;
        res = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, &tile_cols);
        (void)res;
    }

    // 6. AV1E_SET_SINGLE_TILE_DECODING (Encoder only)
    if (use_encoder) {
        int single_tile = (Data[5] % 2);
        res = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, &single_tile);
        (void)res;
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}