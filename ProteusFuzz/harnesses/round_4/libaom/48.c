// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    /* Fuzz configuration parameters based on input data if size permits */
    if (size > 0) {
        cfg.g_w = 16 + (data[0] % 4096);
        cfg.g_h = 16 + (data[1] % 4096);
        /* Fixed typo: g_time_base -> g_timebase */
        cfg.g_timebase.num = 1 + (data[2] % 100);
        cfg.g_timebase.den = 1 + (data[3] % 100);
        cfg.rc_target_bitrate = 100 + (data[4] % 10000);
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;

    /* Initialize context structure */
    memset(&ctx, 0, sizeof(ctx));

    /* Ensure we have enough data to initialize basic config */
    if (Size < 5) {
        return 0;
    }

    /* Initialize the encoder */
    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    /*
     * Attempt to invoke the target control functions using the correct
     * constants found in aomcx.h as indicated by the compiler errors.
     */

    /* 1. AV1E_SET_BITRATE_ONE_PASS_CBR */
    ret = aom_codec_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, 1);
    (void)ret;

    /* 2. AV1E_SET_MODE_COST_UPD_FREQ */
    if (Size > 6) {
        int freq = Data[6] % 100;
        ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, freq);
        (void)ret;
    }

    /* 3. AOME_SET_CQ_LEVEL */
    if (Size > 7) {
        int cq = Data[7] % 64; /* Valid range typically 0-63 */
        ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq);
        (void)ret;
    }

    /* 4. AV1E_SET_ENABLE_TX_SIZE_SEARCH */
    if (Size > 8) {
        int enable = (Data[8] % 2) ? 1 : 0;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable);
        (void)ret;
    }

    /* 5. AOME_SET_MAX_INTRA_BITRATE_PCT */
    if (Size > 9) {
        int pct = Data[9] % 1000; /* Valid range typically 0-1000 */
        ret = aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, pct);
        (void)ret;
    }

    /* 6. AV1E_SET_ENABLE_GLOBAL_MOTION */
    if (Size > 10) {
        int enable = (Data[10] % 2) ? 1 : 0;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable);
        (void)ret;
    }

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}