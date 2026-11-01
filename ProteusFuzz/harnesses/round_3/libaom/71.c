// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_ENABLE_INTERINTER_WEDGE
 * - AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
 * - AV1E_SET_FRAME_PERIODIC_BOOST
 * - AV1E_SET_ENABLE_FILTER_INTRA
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    int control_id = Data[0] % 6;
    int value = 0;
    if (Size > 1) {
        value = Data[1];
    }

    aom_codec_err_t ret = AOM_CODEC_OK;

    switch (control_id) {
        case 0:
            /* AV1E_SET_S_FRAME_MODE */
            /* Value: 0 (OFF), 1 (ON), 2 (AUTO) */
            value = value % 3;
            ret = aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, value);
            break;

        case 1:
            /* AV1E_SET_INTER_DCT_ONLY */
            /* Value: 0 (OFF), 1 (ON) */
            value = value % 2;
            ret = aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, value);
            break;

        case 2:
            /* AV1E_SET_ENABLE_INTERINTER_WEDGE */
            /* Value: 0 (OFF), 1 (ON) */
            value = value % 2;
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, value);
            break;

        case 3:
            /* AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR */
            /* Value: integer milliseconds, clamp to reasonable range */
            value = value % 1000;
            ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, value);
            break;

        case 4:
            /* AV1E_SET_FRAME_PERIODIC_BOOST */
            /* Value: integer, clamp to reasonable range */
            value = value % 100;
            ret = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, value);
            break;

        case 5:
            /* AV1E_SET_ENABLE_FILTER_INTRA */
            /* Value: 0 (OFF), 1 (ON) */
            value = value % 2;
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, value);
            break;

        default:
            break;
    }

    cleanup_encoder(&ctx);

    return 0;
}