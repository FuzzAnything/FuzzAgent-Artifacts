// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int32_t val;

    if (size >= sizeof(int32_t)) {
        memcpy(&val, data, sizeof(int32_t));
    } else {
        val = 0;
    }

    // AV1E_SET_TILE_COLUMNS
    ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, val);
    (void)ret;

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
    (void)ret;

    // AV1E_ENABLE_EXT_TILE_DEBUG
    ret = aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
    (void)ret;

    // AV1E_SET_SINGLE_TILE_DECODING
    ret = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
    (void)ret;

    // AV1_SET_TILE_MODE (if applicable to encoder context)
    ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, val);
    (void)ret;

    return 0;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int32_t val;

    if (size >= sizeof(int32_t)) {
        memcpy(&val, data, sizeof(int32_t));
    } else {
        val = 0;
    }

    // AV1D_SET_IS_ANNEXB
    ret = aom_codec_control(ctx, AV1D_SET_IS_ANNEXB, val);
    (void)ret;

    // AV1_SET_TILE_MODE (if applicable to decoder context)
    ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t encoder_ctx;
    aom_codec_ctx_t decoder_ctx;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();

    if (!enc_iface || !dec_iface) {
        return 0;
    }

    aom_codec_enc_cfg_t ecfg;
    aom_codec_dec_cfg_t dcfg;

    // Initialize encoder config
    if (aom_codec_enc_config_default(enc_iface, &ecfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    ecfg.g_w = 128;
    ecfg.g_h = 128;
    ecfg.g_timebase.num = 1;
    ecfg.g_timebase.den = 30;

    // Initialize decoder config
    memset(&dcfg, 0, sizeof(dcfg));
    dcfg.threads = 0;
    dcfg.allow_lowbitdepth = 1;

    // Initialize encoder context
    if (aom_codec_enc_init(&encoder_ctx, enc_iface, &ecfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize decoder context
    if (aom_codec_dec_init(&decoder_ctx, dec_iface, &dcfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }

    // Fuzz encoder controls
    fuzz_encoder_control(&encoder_ctx, Data, Size);

    // Fuzz decoder controls
    fuzz_decoder_control(&decoder_ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&encoder_ctx);
    aom_codec_destroy(&decoder_ctx);

    return 0;
}