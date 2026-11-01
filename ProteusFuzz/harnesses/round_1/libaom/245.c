// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"

// Forward declaration
static void dummy_inspect_cb(void *ctx, void *buf);

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    // Helper to safely read integers from fuzz data
    int val;
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    } else {
        val = 0;
    }

    // 1. AV1E_SET_INTER_DCT_ONLY
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);

    // 2. AV1E_SET_ENABLE_RECT_TX
    if (size >= sizeof(int) + 1) {
        int val2;
        memcpy(&val2, data + sizeof(int), sizeof(int));
        aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, val2);
    }

    // 3. AV1E_SET_QUANT_B_ADAPT
    if (size >= sizeof(int) * 2 + 1) {
        int val3;
        memcpy(&val3, data + sizeof(int) * 2, sizeof(int));
        aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, val3);
    }

    // 5. AV1E_SET_ENABLE_TX64
    if (size >= sizeof(int) * 3 + 1) {
        int val4;
        memcpy(&val4, data + sizeof(int) * 3, sizeof(int));
        aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, val4);
    }

    // 6. AV1E_SET_TUNE_CONTENT
    if (size >= sizeof(int) * 4 + 1) {
        int val5;
        memcpy(&val5, data + sizeof(int) * 4, sizeof(int));
        aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, val5);
    }

    return 1;
}

static int fuzz_decoder_inspection(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    // 4. AV1_SET_INSPECTION_CALLBACK
    aom_inspect_init inspect_init;
    inspect_init.inspect_cb = dummy_inspect_cb;
    inspect_init.inspect_ctx = (void*)(uintptr_t)((size > 0) ? (data[0] & 0xFF) : 0);

    aom_codec_control(ctx, AV1_SET_INSPECTION_CALLBACK, &inspect_init);

    return 1;
}

// Definition of the callback function matching the expected signature: void (*)(void *, void *)
static void dummy_inspect_cb(void *ctx, void *buf) {
    (void)ctx;
    (void)buf;
}

int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_err_t ret;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    int is_encoder = 1;

    // Determine if we should test encoder or decoder based on data
    if (Data[0] & 1) {
        is_encoder = 1;
        iface = aom_codec_av1_cx(); // Encoder
    } else {
        is_encoder = 0;
        iface = aom_codec_av1_dx(); // Decoder
    }

    if (!iface) return 0;

    if (is_encoder) {
        ret = aom_codec_enc_config_default(iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        // Fuzz encoder controls
        fuzz_encoder_controls(&ctx, Data, Size);

        // Cleanup encoder
        aom_codec_destroy(&ctx);
    } else {
        // Decoder setup
        ret = aom_codec_dec_init(&ctx, iface, NULL, 0);
        if (ret != AOM_CODEC_OK) return 0;

        // Fuzz decoder inspection
        fuzz_decoder_inspection(&ctx, Data, Size);

        // Cleanup decoder
        aom_codec_destroy(&ctx);
    }

    return 0;
}