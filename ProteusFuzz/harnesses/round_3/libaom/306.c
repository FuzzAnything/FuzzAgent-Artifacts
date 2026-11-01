// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

static int try_fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int64_t val;
    uint8_t q_idx;
    uint8_t cdef;

    if (ctx == NULL) return 0;

    // AOMD_GET_LAST_QUANTIZER
    if (aom_codec_control(ctx, AOMD_GET_LAST_QUANTIZER, &val) == AOM_CODEC_OK) {
        (void)val;
    }

    // AOMD_GET_BASE_Q_IDX
    if (aom_codec_control(ctx, AOMD_GET_BASE_Q_IDX, &q_idx) == AOM_CODEC_OK) {
        (void)q_idx;
    }

    // AOME_GET_LAST_QUANTIZER_64
    if (aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER_64, &val) == AOM_CODEC_OK) {
        (void)val;
    }

    // AV1E_GET_LUMA_CDEF_STRENGTH
    // The API expects an array of size CDEF_MAX_STRENGTHS (typically 8), not a single uint8_t.
    // Using a local array to prevent stack-buffer-overflow.
    uint8_t cdef_buf[8];
    memset(cdef_buf, 0, sizeof(cdef_buf));
    if (aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_buf) == AOM_CODEC_OK) {
        (void)cdef_buf[0]; // Use the first element if needed
    }

    // AV1E_SET_QUANTIZER_ONE_PASS
    int q_val = 0;
    if (size > 0) {
        q_val = data[0] & 0xFF;
    }
    if (aom_codec_control(ctx, AV1E_SET_QUANTIZER_ONE_PASS, q_val) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AOME_GET_LAST_QUANTIZER
    if (aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER, &val) == AOM_CODEC_OK) {
        (void)val;
    }

    return ret;
}

static int try_fuzz_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int64_t val;
    uint8_t q_idx;

    if (ctx == NULL) return 0;

    // AOMD_GET_LAST_QUANTIZER
    if (aom_codec_control(ctx, AOMD_GET_LAST_QUANTIZER, &val) == AOM_CODEC_OK) {
        (void)val;
    }

    // AOMD_GET_BASE_Q_IDX
    if (aom_codec_control(ctx, AOMD_GET_BASE_Q_IDX, &q_idx) == AOM_CODEC_OK) {
        (void)q_idx;
    }

    // AOME_GET_LAST_QUANTIZER_64 (Encoder specific, likely fails on decoder)
    if (aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER_64, &val) != AOM_CODEC_OK) {
        // Expected to fail
    }

    // AV1E_GET_LUMA_CDEF_STRENGTH (Encoder specific)
    uint8_t cdef_buf[8];
    memset(cdef_buf, 0, sizeof(cdef_buf));
    if (aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_buf) != AOM_CODEC_OK) {
        // Expected to fail
    }

    // AV1E_SET_QUANTIZER_ONE_PASS (Encoder specific)
    int q_val = 0;
    if (size > 0) {
        q_val = data[0] & 0xFF;
    }
    if (aom_codec_control(ctx, AV1E_SET_QUANTIZER_ONE_PASS, q_val) != AOM_CODEC_OK) {
        // Expected to fail
    }

    // AOME_GET_LAST_QUANTIZER (Encoder specific)
    if (aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER, &val) != AOM_CODEC_OK) {
        // Expected to fail
    }

    return ret;
}

int LLVMFuzzerTestOneInput_306(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_iface_t *enc_iface = NULL;
    aom_codec_iface_t *dec_iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_err_t err;

    // Initialize Encoder
    enc_iface = aom_codec_av1_cx();
    if (enc_iface == NULL) return 0;

    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    
    // Use fuzz data to tweak config slightly
    if (Size > 0) cfg.g_w = (Data[0] % 256) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    err = aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Try to continue with decoder if encoder fails
    } else {
        // Run encoder fuzz logic
        try_fuzz_encoder(&enc_ctx, Data, Size);
    }

    // Initialize Decoder
    dec_iface = aom_codec_av1_dx();
    if (dec_iface == NULL) {
        if (err == AOM_CODEC_OK) aom_codec_destroy(&enc_ctx);
        return 0;
    }

    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    err = aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Cleanup encoder if it was initialized
        if (enc_ctx.iface != NULL) aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Run decoder fuzz logic
    try_fuzz_decoder(&dec_ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);

    return 0;
}