// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_encoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int quant_value = 0;
    int64_t quant_value_64 = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    if (Size > 0) {
        quant_value = (int)(Data[0] % 256);
        res = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, quant_value);
        (void)res;
    }

    if (Size > 1) {
        quant_value = (int)(Data[1] % 256);
        res = aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quant_value);
        (void)res;
    }

    res = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quant_value);
    (void)res;

    res = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quant_value_64);
    (void)res;

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int base_q_idx = 0;
    int last_quantizer = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    if (Size > 0) {
        res = aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        (void)res;
    }

    if (Size > 1) {
        res = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer);
        (void)res;
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    if (mode == 0) {
        fuzz_encoder_control(payload, payload_size);
    } else {
        fuzz_decoder_control(payload, payload_size);
    }

    return 0;
}