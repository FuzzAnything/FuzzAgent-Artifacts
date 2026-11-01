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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int id, int32_t value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, id, value);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int fuzz_encoder_control_int(aom_codec_ctx_t *ctx, int id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, id, value);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int32_t val32 = 0;
    int val = 0;

    if (Size >= 4) {
        val32 = *(const int32_t *)(Data);
    }
    if (Size >= 8) {
        val = *(const int *)(Data + 4);
    }

    // Fuzz AOME_SET_ENABLEAUTOBWDREF
    fuzz_encoder_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val32);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    fuzz_encoder_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val32);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val32);

    // Fuzz AV1E_SET_DELTAQ_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_DELTAQ_MODE, val32);

    // Fuzz AOME_SET_CQ_LEVEL
    fuzz_encoder_control(&ctx, AOME_SET_CQ_LEVEL, val32);

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    fuzz_encoder_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val32);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}