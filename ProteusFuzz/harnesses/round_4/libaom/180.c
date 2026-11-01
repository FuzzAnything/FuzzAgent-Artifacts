// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to vary configuration parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[0] % 100) + 1;
    cfg.g_timebase.den = (Data[1] % 100) + 1;
    cfg.rc_target_bitrate = (Data[0] * 256 + Data[1]) % 10000 + 100;

    aom_codec_ctx_t *ctx = init_encoder(iface, &cfg);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    int min_partition_size = (Data[0] % 4) + 1; // 1, 2, 3, or 4
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, min_partition_size);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    int enable_interinter_wedge = (Data[1] % 2); // 0 or 1
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_interinter_wedge);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    int coeff_cost_upd_freq = (Data[0] % 10) + 1; // 1 to 10
    aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);

    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    int enable_order_hint = (Data[1] % 2); // 0 or 1
    aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, enable_order_hint);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    int enable_smooth_intra = (Data[0] % 2); // 0 or 1
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable_smooth_intra);

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    int enable_intra_edge_filter = (Data[1] % 2); // 0 or 1
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_intra_edge_filter);

    cleanup_encoder(ctx);
    return 0;
}