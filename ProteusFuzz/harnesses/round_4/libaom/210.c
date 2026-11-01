// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    int value = (int)Data[0];

    // Fuzz AV1E_SET_DELTALF_MODE
    try_control(&ctx, AV1E_SET_DELTALF_MODE, value);

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    try_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, value);

    // Fuzz AV1E_SET_GF_CBR_BOOST_PCT
    try_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, value);

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    try_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, value);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    try_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, value);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    try_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, value);

    // Try with different values derived from input
    for (size_t i = 2; i < Size; i++) {
        int fuzz_val = (int)Data[i];
        try_control(&ctx, AV1E_SET_DELTALF_MODE, fuzz_val);
        try_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, fuzz_val);
        try_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, fuzz_val);
        try_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, fuzz_val);
        try_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, fuzz_val);
        try_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, fuzz_val);
    }

    cleanup_codec(&ctx);
    return 0;
}