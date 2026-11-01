// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

static int invoke_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_191(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    int width = (Data[0] % 32) + 16;
    int height = (Data[1] % 32) + 16;

    if (init_encoder(&ctx, &cfg, width, height) != 0) {
        return 0;
    }

    int value1 = (int)(Data[2] & 0xFF);
    int value2 = (int)(Data[3] & 0xFF);
    int value3 = (int)((Data[2] ^ Data[3]) & 0xFF);

    // AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    invoke_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, value1);
    // AV1E_SET_ERROR_RESILIENT_MODE
    invoke_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, value2);
    // AV1E_SET_MIN_GF_INTERVAL
    invoke_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, value3);
    // AV1E_SET_DELTAQ_STRENGTH
    invoke_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, value1);
    // AV1E_SET_MAX_GF_INTERVAL
    invoke_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, value2);
    // AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    invoke_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, value3);

    cleanup_codec(&ctx);
    return 0;
}