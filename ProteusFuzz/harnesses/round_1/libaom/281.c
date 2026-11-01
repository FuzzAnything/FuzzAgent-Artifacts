// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->g_bit_depth = 8;
    cfg->g_input_bit_depth = 8;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_281(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int val;
    int control_idx;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) return 0;

    // Use input data to derive control values
    // We map the first byte to select which control to test
    control_idx = Data[0] % 6;

    // Safely derive a value from the input to pass to the control
    // Ensure we have enough data before accessing indices
    if (Size >= 2) {
        val = (int)(Data[1] % 256);
        if (Size > 2) {
            val += (int)(Data[2] % 256) * 256;
        }
    } else {
        val = 0;
    }

    switch (control_idx) {
        case 0:
            // AV1E_SET_QM_V
            ret = aom_codec_control(&ctx, AV1E_SET_QM_V, val);
            break;
        case 1:
            // AV1E_SET_INTER_DCT_ONLY
            ret = aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val % 2);
            break;
        case 2:
            // AV1E_SET_MIN_CR
            ret = aom_codec_control(&ctx, AV1E_SET_MIN_CR, val);
            break;
        case 3:
            // AV1E_SET_ENABLE_SMOOTH_INTRA
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val % 2);
            break;
        case 4:
            // AV1E_SET_CHROMA_SUBSAMPLING_X
            ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val % 3);
            break;
        case 5:
            // AV1E_SET_ENABLE_DIST_8X8 is not defined in standard headers.
            // Using AV1E_SET_ENABLE_RECT_TX as a fallback for testing encoder controls.
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val % 2);
            break;
        default:
            break;
    }

    // Handle return value
    if (ret != AOM_CODEC_OK) {
        // Ignore errors as they are expected in fuzzing
    }

    cleanup_encoder(&ctx);

    return 0;
}