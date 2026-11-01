// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    unsigned int width, height;

    if (size < 4) return 0;

    width = data[0] | (data[1] << 8);
    height = data[2] | (data[3] << 8);

    if (width == 0 || height == 0) {
        width = 64;
        height = 64;
    }
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int initialized = 0;
    int8_t val8;
    int val;
    aom_codec_err_t ret;

    if (Size == 0) return 0;

    val8 = Data[0];
    if (Size >= 4) {
        val = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    } else {
        val = 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size)) {
        initialized = 1;

        // Fuzz AV1E_SET_ENABLE_QM
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val8);
        (void)ret;

        // Fuzz AV1E_SET_QM_MAX
        ret = aom_codec_control(&ctx, AV1E_SET_QM_MAX, val);
        (void)ret;

        // Fuzz AV1E_SET_NOISE_SENSITIVITY
        ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val8);
        (void)ret;

        // Fuzz AV1E_SET_DELTAQ_STRENGTH
        ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val8);
        (void)ret;

        // Fuzz AV1E_SET_AQ_MODE
        ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, val8);
        (void)ret;

        // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
        ret = aom_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val8);
        (void)ret;

        cleanup_codec(&ctx);
    }

    return 0;
}