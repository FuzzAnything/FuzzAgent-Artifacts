// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/*
 * Fuzz driver for libaom codec control functions.
 * This driver initializes encoder and decoder contexts and invokes
 * specific control commands using the standard aom_codec_control API.
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* --- Encoder Testing --- */
    aom_codec_ctx_t enc_ctx;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Use input data to configure encoder settings safely */
    cfg.g_w = (Size > 0) ? (Data[0] % 512) + 16 : 128;
    cfg.g_h = (Size > 1) ? (Data[1] % 512) + 16 : 128;
    /* Fixed: g_time_base -> g_timebase */
    cfg.g_timebase.num = (Size > 2) ? Data[2] : 1;
    cfg.g_timebase.den = (Size > 3) ? (Data[3] % 255) + 1 : 30;
    cfg.g_threads = (Size > 4) ? (Data[4] % 8) + 1 : 1;
    cfg.rc_target_bitrate = (Size > 5) ? (Data[5] * 1000) : 1000;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /*
     * Attempt to invoke the specific control commands.
     */

    /* 1. AOME_SET_SHARPNESS */
    int sharpness = (Size > 6) ? Data[6] % 8 : 0;
    aom_codec_control(&enc_ctx, AOME_SET_SHARPNESS, sharpness);

    /* 5. AV1E_SET_ROW_MT */
    int row_mt = (Size > 7) ? Data[7] % 2 : 0;
    aom_codec_control(&enc_ctx, AV1E_SET_ROW_MT, row_mt);

    /* 6. AV1E_SET_ENABLE_GLOBAL_MOTION */
    int global_motion = (Size > 8) ? Data[8] % 2 : 0;
    aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, global_motion);

    /* 3. AV1E_SET_MODE_COST_UPD_FREQ */
    int mode_cost_freq = (Size > 9) ? Data[9] % 100 : 0;
    aom_codec_control(&enc_ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_freq);

    /* 4. AV1E_SET_ENABLE_TX_SIZE_SEARCH */
    int tx_size_search = (Size > 10) ? Data[10] % 2 : 0;
    aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, tx_size_search);

    /* Cleanup Encoder */
    cleanup_encoder(&enc_ctx);

    /* --- Decoder Testing --- */
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        return 0;
    }

    aom_codec_dec_cfg_t dec_cfg;
    /* Initialize struct to zero to avoid undefined fields */
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    dec_cfg.threads = (Size > 11) ? (Data[11] % 8) + 1 : 1;
    /* Fixed: Removed non-existent fields allow_highbitdepth, allow_multiref, allow_screencontenttools */
    /* Only using valid field allow_lowbitdepth if needed, but default is usually fine for init */
    dec_cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* 2. AV1D_SET_ROW_MT */
    int dec_row_mt = (Size > 12) ? Data[12] % 2 : 0;
    aom_codec_control(&dec_ctx, AV1D_SET_ROW_MT, dec_row_mt);

    /* Cleanup Decoder */
    cleanup_decoder(&dec_ctx);

    return 0;
}