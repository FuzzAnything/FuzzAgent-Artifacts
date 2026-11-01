// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure the encoder
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = (Size % 4) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control ID and value from input
    int control_id = Data[0];
    int value = Data[1];

    // Map control IDs to known AV1 encoder controls
    // AV1E_SET_INTRA_DCT_ONLY
    if (control_id == 0) {
        fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, value);
    }
    // AV1E_SET_MATRIX_COEFFICIENTS
    else if (control_id == 1) {
        fuzz_encoder_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, value);
    }
    // AV1E_SET_ENABLE_GLOBAL_MOTION
    else if (control_id == 2) {
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, value);
    }
    // AV1E_SET_COLOR_RANGE
    else if (control_id == 3) {
        fuzz_encoder_control(&ctx, AV1E_SET_COLOR_RANGE, value);
    }
    // AV1E_SET_COLOR_PRIMARIES
    else if (control_id == 4) {
        fuzz_encoder_control(&ctx, AV1E_SET_COLOR_PRIMARIES, value);
    }
    // AV1E_SET_ENABLE_FLIP_IDTX
    else if (control_id == 5) {
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    }
    // Try all controls with random values from input
    else {
        fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, value);
        fuzz_encoder_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, value);
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, value);
        fuzz_encoder_control(&ctx, AV1E_SET_COLOR_RANGE, value);
        fuzz_encoder_control(&ctx, AV1E_SET_COLOR_PRIMARIES, value);
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}