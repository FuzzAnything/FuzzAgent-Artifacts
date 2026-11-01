// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to diversify configuration slightly if possible
    if (Size > 0) {
        cfg.g_w = (Data[0] % 256) + 16; // Ensure width is at least 16
        cfg.g_h = (Data[1 % Size] % 256) + 16; // Ensure height is at least 16
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Data[2 % Size] % 255) + 1;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    int inter_dct_only = (Data[3 % Size] % 2);
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    // Fuzz AV1E_SET_ENABLE_TX64
    int enable_tx64 = (Data[4 % Size] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX64, enable_tx64);

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    // Assuming it takes an int enum for matrix coefficients
    int matrix_coeff = (Data[5 % Size] % 10); // Limit to reasonable range
    aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coeff);

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    int enable_flip_idtx = (Data[6 % Size] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);

    // Fuzz AV1E_SET_TUNE_CONTENT
    // Assuming it takes an int enum for content type
    int tune_content = (Data[7 % Size] % 5); // Limit to reasonable range
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);

    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    int enable_diff_wtd_comp = (Data[8 % Size] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd_comp);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}