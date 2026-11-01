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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 16;
    cfg->g_h = 16;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_2pass_vbr_minsection_pct = 10;
    cfg->rc_2pass_vbr_maxsection_pct = 50;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_346(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    aom_codec_err_t ret;
    int val_int;
    unsigned int val_uint;

    if (Size < 2) {
        return 0;
    }

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    val_int = (int)(Data[0]);
    val_uint = (unsigned int)(Data[1]);

    ret = aom_codec_control(&ctx, AV1E_SET_QM_MAX, val_int);
    (void)ret;

    ret = aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val_uint);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, val_uint);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val_int);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, val_int);
    (void)ret;

    // AOME_SET_MAX_INTER_BITRATE_PCT is not a standard public constant in recent libaom versions.
    // It might be internal or renamed. We skip it to ensure compilation.
    // If it exists in a specific version, it would be defined in aom_encoder.h.
    // Since it's causing a compile error, we omit it.

    cleanup_encoder(&ctx);

    return 0;
}