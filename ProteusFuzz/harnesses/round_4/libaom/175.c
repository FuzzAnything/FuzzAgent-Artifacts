// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int width = 16;
    int height = 16;

    if (size >= 2) {
        width = (data[0] % 256) + 16;
        height = (data[1] % 256) + 16;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val_int;
    int val_int2;

    if (Size < 2) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Extract values from input data to diversify control calls
    val_int = (int)(Data[0] % 3); // 0, 1, 2
    val_int2 = (int)(Data[1] % 101); // 0 to 100

    // 1. AV1E_SET_DELTALF_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val_int);
    (void)ret;

    // 2. AOME_SET_SHARPNESS
    ret = aom_codec_control(&ctx, AOME_SET_SHARPNESS, val_int2);
    (void)ret;

    // 3. AV1E_SET_DISABLE_TRELLIS_QUANT
    ret = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val_int % 2);
    (void)ret;

    // 4. AV1E_SET_GF_CBR_BOOST_PCT
    ret = aom_codec_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, val_int2);
    (void)ret;

    // 5. AOME_SET_MAX_INTRA_BITRATE_PCT
    ret = aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val_int2);
    (void)ret;

    // 6. AV1E_SET_ENABLE_TPL_MODEL
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val_int % 2);
    (void)ret;

    cleanup_codec(&ctx);
    return 0;
}