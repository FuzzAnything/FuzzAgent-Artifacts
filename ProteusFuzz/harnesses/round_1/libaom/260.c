// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_image.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"

static int fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1E_SET_ROW_MT: Sets row multi-threading for encoder
    int row_mt = 0;
    if (size > 0) row_mt = data[0] & 0x1;
    if (aom_codec_control(ctx, AV1E_SET_ROW_MT, row_mt) != AOM_CODEC_OK) {
        // Ignore error, fuzzing continues
    }

    // AV1E_SET_ENABLE_INTERINTER_WEDGE: Enable inter-intra wedge
    int wedge = 0;
    if (size > 1) wedge = data[1] & 0x1;
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, wedge) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_COEFF_COST_UPD_FREQ: Set coefficient cost update frequency
    int freq = 0;
    if (size > 2) freq = data[2] % 100; // Keep within reasonable range
    if (aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, freq) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_FP_MT: Set floating point multi-threading
    int fp_mt = 0;
    if (size > 3) fp_mt = data[3] & 0x1;
    if (aom_codec_control(ctx, AV1E_SET_FP_MT, fp_mt) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER: Enable intra edge filter
    int edge_filter = 0;
    if (size > 4) edge_filter = data[4] & 0x1;
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, edge_filter) != AOM_CODEC_OK) {
        // Ignore error
    }

    return 0;
}

static int fuzz_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1D_SET_ROW_MT: Sets row multi-threading for decoder
    int row_mt = 0;
    if (size > 0) row_mt = data[0] & 0x1;
    if (aom_codec_control(ctx, AV1D_SET_ROW_MT, row_mt) != AOM_CODEC_OK) {
        // Ignore error
    }

    return 0;
}

int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create encoder context
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    const aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder controls
    fuzz_encoder(&enc_ctx, Data, Size);

    // Cleanup encoder
    aom_codec_destroy(&enc_ctx);

    // Create decoder context
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    const aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz decoder controls
    fuzz_decoder(&dec_ctx, Data, Size);

    // Cleanup decoder
    aom_codec_destroy(&dec_ctx);

    return 0;
}