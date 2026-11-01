// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int initialize_encoder(aom_codec_ctx_t **ctx, aom_codec_enc_cfg_t *cfg) {
    *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!*ctx) return -1;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(*ctx);
        return -1;
    }

    if (aom_codec_enc_init(*ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(*ctx);
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration slightly
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1 % Size] % 1024) + 16;
    // Use rc_target_bitrate instead of g_bitrate
    cfg.rc_target_bitrate = (Data[2 % Size] % 1000) + 100;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (initialize_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_MIN_CR
    if (Size >= 1) {
        int min_cr = (int)(Data[0] % 1000);
        aom_codec_control(ctx, AV1E_SET_MIN_CR, min_cr);
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size >= 2) {
        int enable_palette = (Data[1 % Size] > 127) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);
    }

    // Fuzz AV1E_SET_LOSSLESS
    if (Size >= 3) {
        int lossless = (Data[2 % Size] > 127) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_LOSSLESS, lossless);
    }

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    if (Size >= 4) {
        int enable_overlay = (Data[3 % Size] > 127) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
    }

    // Fuzz AV1E_SET_QM_MIN
    if (Size >= 5) {
        int qm_min = (int)(Data[4 % Size] % 256);
        aom_codec_control(ctx, AV1E_SET_QM_MIN, qm_min);
    }

    // Fuzz AV1E_SET_FP_MT
    if (Size >= 6) {
        int fp_mt = (Data[5 % Size] > 127) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_FP_MT, fp_mt);
    }

    cleanup_codec(ctx);
    return 0;
}