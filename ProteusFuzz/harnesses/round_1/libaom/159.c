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

static aom_codec_ctx_t *init_encoder(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder();
    if (!ctx) return 0;

    int val = 0;
    if (Size > 0) {
        val = Data[0];
    }

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);

    // Fuzz AV1E_SET_S_FRAME_MODE
    aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, val);

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val);

    // Fuzz AOME_SET_TUNING
    aom_codec_control(ctx, AOME_SET_TUNING, val);

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val);

    cleanup_encoder(ctx);
    return 0;
}