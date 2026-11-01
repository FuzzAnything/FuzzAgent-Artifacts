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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return 1;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 1;

    /* Use input data to configure encoder settings if possible, or just set defaults */
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
        cfg->g_h = (data[1] % 1024) + 16;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_bit_depth = 8;
        cfg->g_threads = 1;
        cfg->g_error_resilient = 0;
        cfg->rc_target_bitrate = 1000;
        cfg->rc_min_quantizer = 0;
        cfg->rc_max_quantizer = 63;
        cfg->g_lag_in_frames = 0;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    /*
     * The prompt lists specific "typechecked" functions which are not standard public API.
     * We map the intent of these functions to the standard aom_codec_control() calls
     * using the appropriate AV1 encoder control constants.
     *
     * 1. AV1E_SET_QM_U
     * 2. AV1E_SET_MIN_GF_INTERVAL
     * 3. AV1E_SET_AQ_MODE
     * 4. AV1E_SET_QM_Y
     * 5. AV1E_SET_CDF_UPDATE_MODE
     * 6. AOME_SET_CQ_LEVEL (Generic for all codecs)
     */

    /* Extract parameters from fuzz input to vary the control calls */
    int val1 = (int)(Data[0] % 64);
    int val2 = (int)(Data[1] % 100);
    int val3 = (int)(Data[2 % Size] % 4); /* AQ_MODE usually 0-3 */
    int val4 = (int)(Data[3 % Size] % 64);
    int val5 = (int)(Data[4 % Size] % 2); /* CDF_UPDATE_MODE usually 0-1 */
    int val6 = (int)(Data[5 % Size] % 64);

    /* Call 1: AV1E_SET_QM_U */
    ret = aom_codec_control(&ctx, AV1E_SET_QM_U, val1);
    (void)ret;

    /* Call 2: AV1E_SET_MIN_GF_INTERVAL */
    ret = aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val2);
    (void)ret;

    /* Call 3: AV1E_SET_AQ_MODE */
    ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, val3);
    (void)ret;

    /* Call 4: AV1E_SET_QM_Y */
    ret = aom_codec_control(&ctx, AV1E_SET_QM_Y, val4);
    (void)ret;

    /* Call 5: AV1E_SET_CDF_UPDATE_MODE */
    ret = aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, val5);
    (void)ret;

    /* Call 6: AOME_SET_CQ_LEVEL */
    ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, val6);
    (void)ret;

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}