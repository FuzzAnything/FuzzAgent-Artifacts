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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_289(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    uint32_t value = 0;
    if (Size > 4) {
        memcpy(&value, Data, 4);
    }

    int8_t flag = (value & 0xFF) ? 1 : 0;

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, flag);

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    int16_t noise_sensitivity = (int16_t)(value & 0xFFFF);
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    uint32_t max_drop = value & 0xFFFFFFFF;
    aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    int8_t intrabc_flag = (value >> 8) & 0xFF ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, intrabc_flag);

    // Fuzz AV1E_SET_RATE_DISTRIBUTION_INFO
    // This typically requires a struct, we'll try to pass a pointer to part of the fuzz data if size allows
    if (Size >= 16) {
        aom_codec_control(ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, (void*)(Data + 4));
    }

    // Fuzz AV1E_SET_AUTO_INTRA_TOOLS_OFF
    int8_t auto_intra_off = (value >> 16) & 0xFF ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, auto_intra_off);

    cleanup_codec(ctx);
    return 0;
}