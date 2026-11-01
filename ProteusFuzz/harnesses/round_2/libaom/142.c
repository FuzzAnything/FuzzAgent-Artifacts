// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_ENABLE_SMOOTH_INTRA
 * - AV1E_SET_MIN_PARTITION_SIZE
 * - AV1E_SET_ENABLE_INTRA_EDGE_FILTER
 * - AV1E_SET_ENABLE_ORDER_HINT
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 * - AV1E_SET_ENABLE_INTERINTER_WEDGE
 */

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 6) return 0;

    // Extract values from fuzz input to drive control parameters
    int enable_smooth_intra = (data[0] % 2);
    int min_partition_size = (data[1] % 4) + 1; // 1 to 4
    int enable_intra_edge_filter = (data[2] % 2);
    int enable_order_hint = (data[3] % 2);
    int coeff_cost_upd_freq = (data[4] % 10) + 1; // 1 to 10
    int enable_interinter_wedge = (data[5] % 2);

    aom_codec_err_t ret;

    // Control 1: Enable Smooth Intra
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable_smooth_intra);
    (void)ret;

    // Control 2: Min Partition Size
    ret = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, min_partition_size);
    (void)ret;

    // Control 3: Enable Intra Edge Filter
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_intra_edge_filter);
    (void)ret;

    // Control 4: Enable Order Hint
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, enable_order_hint);
    (void)ret;

    // Control 5: Coeff Cost Update Freq
    ret = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);
    (void)ret;

    // Control 6: Enable Inter-Inter Wedge
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_interinter_wedge);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default configuration
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_usage = 0;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic on the controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}