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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    if (size > 0) {
        cfg.g_w = (size > 1) ? (data[0] % 1920) + 16 : 64;
        cfg.g_h = (size > 2) ? (data[1] % 1080) + 16 : 64;
        cfg.g_bit_depth = 8;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_usage = AOM_CQ;
        cfg.rc_target_bitrate = (size > 3) ? (data[2] * 1000) : 1000;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int tier_val, qm_min_val, min_cr_val, overlay_val, lossless_val, subsampling_val;

    if (Size < 6) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));
    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    tier_val = Data[0];
    qm_min_val = Data[1];
    min_cr_val = Data[2];
    overlay_val = Data[3];
    lossless_val = Data[4];
    subsampling_val = Data[5];

    ret = aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier_val);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_QM_MIN, qm_min_val);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_MIN_CR, min_cr_val);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, overlay_val);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_LOSSLESS, lossless_val);
    (void)ret;

    ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, subsampling_val);
    (void)ret;

    cleanup_codec(&ctx);

    return 0;
}