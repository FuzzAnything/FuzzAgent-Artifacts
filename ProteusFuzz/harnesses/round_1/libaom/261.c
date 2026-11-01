// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - AV1E_SET_TRANSFER_CHARACTERISTICS
 * - AV1E_SET_ENABLE_DIST_WTD_COMP
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_QUANT_B_ADAPT
 * - AV1E_SET_INTRA_DEFAULT_TX_ONLY
 * - AV1E_SET_TUNE_CONTENT
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int w, h;

    if (!ctx || !data || size == 0) {
        return 0;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Safely extract width and height from input data */
    if (size >= 2) {
        w = (data[0] % 256) + 16;
        h = (data[1] % 256) + 16;
    } else if (size == 1) {
        w = (data[0] % 256) + 16;
        h = 16;
    } else {
        w = 128;
        h = 128;
    }

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_261(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    int val;

    if (!Data || Size == 0) {
        return 0;
    }

    /* Initialize context */
    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    /*
     * Extract values from input data to drive the control functions.
     * We use modulo operations to keep values within expected ranges.
     */

    /* 1. AV1E_SET_TRANSFER_CHARACTERISTICS */
    if (Size > 0) {
        val = Data[0];
        res = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
        (void)res;
    }

    /* 2. AV1E_SET_ENABLE_DIST_WTD_COMP */
    if (Size > 1) {
        val = Data[1] & 1;
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);
        (void)res;
    }

    /* 3. AV1E_SET_ENABLE_INTRABC */
    if (Size > 2) {
        val = Data[2] & 1;
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
        (void)res;
    }

    /* 4. AV1E_SET_QUANT_B_ADAPT */
    if (Size > 3) {
        val = Data[3] & 1;
        res = aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, val);
        (void)res;
    }

    /* 5. AV1E_SET_INTRA_DEFAULT_TX_ONLY */
    if (Size > 4) {
        val = Data[4] & 1;
        res = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
        (void)res;
    }

    /* 6. AV1E_SET_TUNE_CONTENT */
    if (Size > 5) {
        val = Data[5] % 3;
        res = aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val);
        (void)res;
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}