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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int initialize_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!initialize_encoder(&codec, &cfg)) {
        return 0;
    }

    int enable_intrabc = (Size > 0) ? (Data[0] & 1) : 0;
    int quant_b_adapt = (Size > 1) ? (Data[1] & 1) : 0;
    int intra_default_tx_only = (Size > 2) ? (Data[2] & 1) : 0;
    int tune_content = (Size > 3) ? (Data[3] % 4) : 0;
    int enable_dist_wtd_comp = (Size > 4) ? (Data[4] & 1) : 0;
    int enable_rect_tx = (Size > 5) ? (Data[5] & 1) : 0;

    aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    aom_codec_control(&codec, AV1E_SET_QUANT_B_ADAPT, quant_b_adapt);
    aom_codec_control(&codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);
    aom_codec_control(&codec, AV1E_SET_TUNE_CONTENT, tune_content);
    aom_codec_control(&codec, AV1E_SET_ENABLE_DIST_WTD_COMP, enable_dist_wtd_comp);
    aom_codec_control(&codec, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);

    cleanup_encoder(&codec);

    return 0;
}