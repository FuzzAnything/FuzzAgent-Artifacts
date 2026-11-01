// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int val;
    aom_codec_err_t ret;

    // AV1E_SET_ENABLE_ORDER_HINT
    if (size >= 1) {
        val = data[0];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, val);
        (void)ret;
    }

    // AV1E_SET_COLOR_PRIMARIES
    if (size >= 2) {
        val = data[1];
        ret = aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, val);
        (void)ret;
    }

    // AV1E_SET_ALLOW_WARPED_MOTION
    if (size >= 3) {
        val = data[2];
        ret = aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_SUPERRES
    if (size >= 4) {
        val = data[3];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val);
        (void)ret;
    }

    // AV1E_SET_TIMING_INFO_TYPE
    if (size >= 5) {
        val = data[4];
        ret = aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, val);
        (void)ret;
    }

    // AOME_SET_SHARPNESS
    if (size >= 6) {
        val = data[5];
        ret = aom_codec_control(ctx, AOME_SET_SHARPNESS, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_248(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use a small fixed size for fuzzing stability
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_controls(&ctx, Data, Size);

    aom_codec_destroy(&ctx);
    return 0;
}