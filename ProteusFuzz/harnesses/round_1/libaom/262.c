// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    (void)err;
    return 0;
}

static int fuzz_encoder_control_uint64(aom_codec_ctx_t *ctx, int control_id) {
    if (ctx == NULL) return 0;
    uint64_t value;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, &value);
    (void)err;
    (void)value;
    return 0;
}

int LLVMFuzzerTestOneInput_262(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure some encoder settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1 % Size] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2 % Size] * 100) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int value;
    if (Size > 0) {
        value = Data[0];
    } else {
        value = 0;
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);

    // Fuzz AOME_GET_LAST_QUANTIZER_64
    fuzz_encoder_control_uint64(&ctx, AOME_GET_LAST_QUANTIZER_64);

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRABC, value);

    // Fuzz AV1E_SET_QUANT_B_ADAPT
    fuzz_encoder_control(&ctx, AV1E_SET_QUANT_B_ADAPT, value);

    // Fuzz AV1E_ENABLE_SB_QP_SWEEP
    fuzz_encoder_control(&ctx, AV1E_ENABLE_SB_QP_SWEEP, value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}