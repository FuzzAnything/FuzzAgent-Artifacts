// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

/*
 * Fuzz driver for libaom control functions.
 * Targets:
 * - AV1E_SET_ENABLE_INTERINTRA_COMP
 * - AV1E_SET_ENABLE_INTERINTRA_WEDGE
 * - AV1E_SET_ENABLE_OBMC
 * - AOMD_GET_ORDER_HINT
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 *
 * Note: AV1E_SET_ENABLE_DIST_8X8 is not a valid constant in the standard libaom API.
 * It has been removed from the test cases to ensure compilation.
 */

static void setup_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return;

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

static void setup_decoder_context(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;

    if (aom_codec_dec_init(ctx, iface, NULL, 0) != AOM_CODEC_OK) {
        return;
    }
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Use the first byte to determine which test path to take
    int mode = Data[0] % 10;

    if (mode < 5) {
        // Encoder control tests
        setup_encoder_context(&enc_ctx, &cfg);
        if (enc_ctx.err != AOM_CODEC_OK) return 0;

        int32_t val = 0;
        if (Size > 1) val = (int32_t)(Data[1] % 2); // 0 or 1 for boolean flags

        switch (Data[0] % 4) {
            case 0: // AV1E_SET_ENABLE_INTERINTRA_COMP
                aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
                break;
            case 1: // AV1E_SET_ENABLE_INTERINTRA_WEDGE
                aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
                break;
            case 2: // AV1E_SET_ENABLE_OBMC
                aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_OBMC, val);
                break;
            case 3: // AV1E_SET_COEFF_COST_UPD_FREQ
                // This might expect a frequency value, not just 0/1
                int32_t freq = (Size > 2) ? (int32_t)(Data[2] * 10) : 0;
                aom_codec_control(&enc_ctx, AV1E_SET_COEFF_COST_UPD_FREQ, freq);
                break;
        }
        aom_codec_destroy(&enc_ctx);
    } else {
        // Decoder control tests
        setup_decoder_context(&dec_ctx);
        if (dec_ctx.err != AOM_CODEC_OK) return 0;

        switch (Data[0] % 5) {
            case 0: // AOMD_GET_ORDER_HINT
                {
                    int32_t order_hint;
                    aom_codec_control(&dec_ctx, AOMD_GET_ORDER_HINT, &order_hint);
                    (void)order_hint;
                }
                break;
            // Fallback to other encoder controls on decoder to test error handling
            // or just do nothing for remaining cases if specific decoder controls aren't listed
            default:
                break;
        }
        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}