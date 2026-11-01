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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_TILE_COLUMNS
 * - AV1E_SET_MIN_CR
 * - AV1E_SET_QM_V
 * - AV1E_SET_CHROMA_SUBSAMPLING_X
 * - AV1E_SET_ENABLE_OVERLAY
 * - AV1E_SET_QM_MIN
 */

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                                 const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_flags_t flags = 0;

    if (size > 0 && data[0] & 1) {
        flags |= AOM_CODEC_USE_PSNR;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }

    aom_codec_enc_config_default(iface, cfg, 0);

    // Use fuzz data to tweak config slightly
    if (size > 0) {
        cfg->g_w = (data[0] % 256) + 16;
        cfg->g_h = (data[1 % size] % 256) + 16;
        cfg->g_timebase.num = (data[2 % size] % 255) + 1;
        cfg->g_timebase.den = (data[3 % size] % 255) + 1;
        cfg->g_threads = (data[4 % size] % 8) + 1;
    }

    res = aom_codec_enc_init(ctx, iface, cfg, flags);
    if (res != AOM_CODEC_OK) {
        // Initialization failed, context is invalid
        memset(ctx, 0, sizeof(*ctx));
    }
}

int LLVMFuzzerTestOneInput_205(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    init_encoder_context(&ctx, &cfg, Data, Size);

    if (ctx.iface == NULL) {
        return 0;
    }

    // AV1E_SET_TILE_COLUMNS
    if (Size > 0) {
        int tile_cols = Data[0] % 8; // Limit to reasonable range
        aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);
    }

    // AV1E_SET_MIN_CR
    if (Size > 1) {
        int min_cr = (int)(Data[1] * 256); // Scale to int range
        aom_codec_control(&ctx, AV1E_SET_MIN_CR, min_cr);
    }

    // AV1E_SET_QM_V
    if (Size > 2) {
        int qm_v = Data[2] % 256;
        aom_codec_control(&ctx, AV1E_SET_QM_V, qm_v);
    }

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    if (Size > 3) {
        int subsampling_x = Data[3] % 2; // 0 or 1
        aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, subsampling_x);
    }

    // AV1E_SET_ENABLE_OVERLAY
    if (Size > 4) {
        int enable_overlay = Data[4] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
    }

    // AV1E_SET_QM_MIN
    if (Size > 5) {
        int qm_min = Data[5] % 256;
        aom_codec_control(&ctx, AV1E_SET_QM_MIN, qm_min);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}