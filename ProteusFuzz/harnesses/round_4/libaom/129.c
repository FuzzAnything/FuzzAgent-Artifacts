// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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
 * Helper function to initialize an encoder context for testing.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface,
                                const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int width, height;

    if (ctx == NULL || iface == NULL) return -1;

    /* Parse width and height from input if possible, otherwise default */
    if (size >= 4) {
        width = (data[0] & 0xFF) | ((data[1] & 0xFF) << 8);
        height = (data[2] & 0xFF) | ((data[3] & 0xFF) << 8);
        if (width == 0) width = 64;
        if (height == 0) height = 64;
    } else {
        width = 64;
        height = 64;
    }

    /* Ensure dimensions are reasonable to avoid allocation failures */
    if (width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper function to initialize a decoder context for testing.
 * Returns 0 on success, -1 on failure.
 */
static int init_decoder_context(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    if (ctx == NULL || iface == NULL) return -1;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed cfg.cpu_used as it is not a member of aom_codec_dec_cfg in newer libaom versions

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t enc_ctx;
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *enc_iface;
    aom_codec_iface_t *dec_iface;
    aom_codec_err_t res;
    int quantizer_val = 0;
    int quantizer_val_64 = 0;
    int base_q_idx = 0;

    if (Size == 0) return 0;

    /* Get encoder and decoder interfaces */
    enc_iface = aom_codec_av1_cx();
    dec_iface = aom_codec_av1_dx();

    if (enc_iface == NULL || dec_iface == NULL) {
        return 0;
    }

    /* Initialize Encoder Context */
    if (init_encoder_context(&enc_ctx, enc_iface, Data, Size) == 0) {
        /* Test AOME_GET_LAST_QUANTIZER */
        res = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer_val);
        (void)res; /* Ignore return for fuzzing stability */

        /* Test AOME_GET_LAST_QUANTIZER_64 */
        res = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer_val_64);
        (void)res;

        /* Test AV1E_SET_QUANTIZER_ONE_PASS */
        if (Size > 0) {
            int q_val = Data[0] & 0x7F; /* Ensure valid range 0-127 */
            res = aom_codec_control(&enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, q_val);
            (void)res;
        }

        /* Test AOME_SET_CQ_LEVEL */
        if (Size > 1) {
            int cq_level = (Data[1] & 0x7F);
            res = aom_codec_control(&enc_ctx, AOME_SET_CQ_LEVEL, cq_level);
            (void)res;
        }

        /* Cleanup Encoder */
        if (aom_codec_destroy(&enc_ctx) != AOM_CODEC_OK) {
            /* Ignore errors during cleanup to avoid crashing */
        }
    }

    /* Initialize Decoder Context */
    if (init_decoder_context(&dec_ctx, dec_iface) == 0) {
        /* Test AOMD_GET_BASE_Q_IDX */
        res = aom_codec_control(&dec_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        (void)res;

        /* Test AOMD_GET_LAST_QUANTIZER */
        res = aom_codec_control(&dec_ctx, AOMD_GET_LAST_QUANTIZER, &quantizer_val);
        (void)res;

        /* Cleanup Decoder */
        if (aom_codec_destroy(&dec_ctx) != AOM_CODEC_OK) {
            /* Ignore errors during cleanup to avoid crashing */
        }
    }

    return 0;
}