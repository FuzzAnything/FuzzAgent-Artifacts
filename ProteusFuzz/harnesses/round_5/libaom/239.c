// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    // Try to set the control with the provided value
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    // We ignore the return value for fuzzing purposes, but in production code,
    // we would check for errors.
    (void)ret;
    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    // Try to set the control with the provided value
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    // We ignore the return value for fuzzing purposes, but in production code,
    // we would check for errors.
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Extract configuration from input data
    int use_encoder = Data[0] % 2;
    int value = (int)(Data[1] & 0xFF);
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        err = aom_codec_enc_config_default(iface, &cfg, 0);
        if (err != AOM_CODEC_OK) return 0;

        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 500;

        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz various encoder control functions
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, value);
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);
        fuzz_encoder_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, value);
        fuzz_encoder_control(&ctx, AV1E_SET_TUNE_CONTENT, value);
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_RECT_TX, value);
        fuzz_encoder_control(&ctx, AOME_SET_SHARPNESS, value);

        aom_codec_destroy(&ctx);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        memset(&dec_cfg, 0, sizeof(dec_cfg));
        err = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz decoder control functions (if applicable)
        // Note: The provided control IDs are encoder-specific, so we skip decoder controls for now.
        // If there are decoder-specific controls, they should be added here.

        aom_codec_destroy(&ctx);
    }

    return 0;
}