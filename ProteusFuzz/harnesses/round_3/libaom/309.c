// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

// Correct signature for aom_inspect_cb: void (*)(void *, void *)
static void dummy_inspect_cb(void *ctx, void *img) {
    (void)ctx;
    (void)img;
}

static int try_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val;
    aom_codec_err_t res;

    // AV1E_SET_ENABLE_OVERLAY
    if (size > 0) val = data[0];
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, val);
    (void)res;

    // AV1E_SET_ENABLE_PALETTE
    if (size > 1) val = data[1];
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
    (void)res;

    // AV1E_SET_ENABLE_CHROMA_DELTAQ
    if (size > 2) val = data[2];
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);
    (void)res;

    // AV1E_GET_NUM_OPERATING_POINTS
    int *num_op = malloc(sizeof(int));
    if (num_op) {
        res = aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, num_op);
        (void)res;
        free(num_op);
    }

    // AV1E_GET_BASELINE_GF_INTERVAL
    int *gf_interval = malloc(sizeof(int));
    if (gf_interval) {
        res = aom_codec_control(ctx, AV1E_GET_BASELINE_GF_INTERVAL, gf_interval);
        (void)res;
        free(gf_interval);
    }

    return 0;
}

static int try_decoder_inspection(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    aom_inspect_init inspect_init;
    inspect_init.inspect_cb = dummy_inspect_cb;
    inspect_init.inspect_ctx = (void *)(size > 0 ? (uintptr_t)data[0] : 0);

    aom_codec_err_t res = aom_codec_control(ctx, AV1_SET_INSPECTION_CALLBACK, &inspect_init);
    (void)res;

    return 0;
}

int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t encoder_ctx = {0};
    aom_codec_ctx_t decoder_ctx = {0};
    aom_codec_iface_t *enc_iface = NULL;
    aom_codec_iface_t *dec_iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    // Initialize encoder
    enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration slightly
    cfg.g_w = Size > 0 ? (16 + (Data[0] % 1024)) : 16;
    cfg.g_h = Size > 1 ? (16 + (Data[1] % 1024)) : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = Size > 2 ? (1 + Data[2]) : 30;
    cfg.rc_target_bitrate = Size > 3 ? (100 + Data[3]) : 100;

    if (aom_codec_enc_init(&encoder_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize decoder
    dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        cleanup_codec(&encoder_ctx);
        return 0;
    }

    memset(&dec_cfg, 0, sizeof(dec_cfg));
    if (aom_codec_dec_init(&decoder_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(&encoder_ctx);
        return 0;
    }

    // Fuzz encoder controls
    try_encoder_controls(&encoder_ctx, Data, Size);

    // Fuzz decoder inspection callback
    try_decoder_inspection(&decoder_ctx, Data, Size);

    // Cleanup
    cleanup_codec(&encoder_ctx);
    cleanup_codec(&decoder_ctx);

    return 0;
}