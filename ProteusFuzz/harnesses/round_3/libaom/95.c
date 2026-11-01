// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom.h>

static int get_int(const uint8_t *d, size_t s) {
    if (s < sizeof(int)) return 0;
    int v;
    memcpy(&v, d, sizeof(int));
    return v;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int value;

    if (!ctx || !ctx->priv) return -1;

    // 1. AV1E_SET_S_FRAME_MODE
    if (size >= 4) {
        value = get_int(data, size);
        ret = aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, value);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 2. AV1E_SET_INTER_DCT_ONLY
    if (size >= 4) {
        value = get_int(data + 4, size > 4 ? size - 4 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, value);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 3. AV1E_SET_ENABLE_FLIP_IDTX
    if (size >= 4) {
        value = get_int(data + 8, size > 8 ? size - 8 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 4. AV1E_SET_NOISE_SENSITIVITY
    if (size >= 4) {
        value = get_int(data + 12, size > 12 ? size - 12 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, value);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 5. AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    if (size >= 4) {
        value = get_int(data + 16, size > 16 ? size - 16 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, value);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 6. AV1E_SET_REDUCED_TX_TYPE_SET
    if (size >= 4) {
        value = get_int(data + 20, size > 20 ? size - 20 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, value);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    aom_codec_flags_t flags = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_usage = AOM_USAGE_REALTIME;

    err = aom_codec_enc_init(&ctx, iface, &cfg, flags);
    if (err != AOM_CODEC_OK) return 0;

    fuzz_encoder_controls(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}