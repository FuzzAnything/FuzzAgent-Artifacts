// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 1;

    // Use fuzz data to tweak config slightly if size permits
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
        cfg->g_h = (data[1] % 1024) + 16;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_lag_in_frames = 250; // Corrected member name
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }
    return 0;
}

static int init_decoder(aom_codec_ctx_t *ctx, aom_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 1;

    // Initialize decoder config manually
    memset(cfg, 0, sizeof(aom_codec_dec_cfg_t));
    cfg->w = 64;
    cfg->h = 64;
    cfg->threads = 1;
    // Removed invalid members allow_highbitdepth and allow_intrabc

    // Use fuzz data to tweak config slightly if size permits
    if (size > 0) {
        cfg->w = (data[0] % 1024) + 16;
        cfg->h = (data[1] % 1024) + 16;
    }

    if (aom_codec_dec_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_dec_cfg_t dec_cfg;

    int enc_init_ok = init_encoder(&enc_ctx, &enc_cfg, Data, Size);
    int dec_init_ok = init_decoder(&dec_ctx, &dec_cfg, Data, Size);

    // Fuzz encoder controls if encoder initialized
    if (enc_init_ok == 0) {
        int32_t cq_level = (Data[0] % 64); // 0-63 range for CQ
        int32_t quant_b_adapt = (Data[1] % 2); // 0 or 1

        // AOME_SET_CQ_LEVEL
        aom_codec_control(&enc_ctx, AOME_SET_CQ_LEVEL, cq_level);

        // AOME_GET_LAST_QUANTIZER
        int32_t last_q;
        aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &last_q);

        // AV1E_SET_QUANT_B_ADAPT
        aom_codec_control(&enc_ctx, AV1E_SET_QUANT_B_ADAPT, quant_b_adapt);

        // AOME_GET_LAST_QUANTIZER_64
        int64_t last_q_64;
        aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &last_q_64);

        // Cleanup encoder
        aom_codec_destroy(&enc_ctx);
    }

    // Fuzz decoder controls if decoder initialized
    if (dec_init_ok == 0) {
        // AOMD_GET_BASE_Q_IDX
        int32_t base_q_idx;
        aom_codec_control(&dec_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);

        // AOMD_GET_LAST_QUANTIZER
        int32_t dec_last_q;
        aom_codec_control(&dec_ctx, AOMD_GET_LAST_QUANTIZER, &dec_last_q);

        // Cleanup decoder
        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}