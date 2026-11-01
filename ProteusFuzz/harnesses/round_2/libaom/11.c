// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_TILE_COLUMNS
 * - AV1E_SET_MV_COST_UPD_FREQ
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_ENABLE_QM
 * - AV1E_SET_QM_V
 */

static void setup_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 0;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->g_error_resilient = 0;
    cfg->g_pass = 0;
    cfg->g_profile = 0;
    cfg->g_bit_depth = 8;
    cfg->g_input_bit_depth = 8;
    cfg->g_usage = AOM_USAGE_REALTIME;
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;
    
    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
    
    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec && codec->priv) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    setup_encoder(&codec, &cfg);
    if (codec.err != AOM_CODEC_OK) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    int tile_cols = (Data[0] % 64) + 1; // 1 to 64
    int mv_cost_upd_freq = (Data[1] % 10) + 1; // 1 to 10
    int inter_dct_only = (Data[2] % 2); // 0 or 1
    int enable_qm = (Data[3] % 2); // 0 or 1
    int qm_v = (Data[4] % 256); // 0 to 255

    /* Apply controls using the correct enum values from aomcx.h */
    aom_codec_control(&codec, AV1E_SET_TILE_COLUMNS, tile_cols);
    aom_codec_control(&codec, AV1E_SET_MV_COST_UPD_FREQ, mv_cost_upd_freq);
    aom_codec_control(&codec, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);
    aom_codec_control(&codec, AV1E_SET_ENABLE_QM, enable_qm);
    aom_codec_control(&codec, AV1E_SET_QM_V, qm_v);

    /* Attempt to encode a dummy frame to exercise state changes */
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    memset(img.planes[0], 128, 64 * 64);
    memset(img.planes[1], 128, 32 * 32);
    memset(img.planes[2], 128, 32 * 32);

    /* Fixed: aom_codec_encode takes 5 arguments, not 6 */
    aom_codec_encode(&codec, &img, 0, 1, 0);

    aom_img_free(&img);

    /* Cleanup */
    cleanup_encoder(&codec);

    return 0;
}