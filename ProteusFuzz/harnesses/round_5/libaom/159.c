// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
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
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    int width = 16;
    int height = 16;

    if (size >= 2) {
        width = data[0] % 256;
        height = data[1] % 256;
        if (width < 8 || height < 8) {
            width = 16;
            height = 16;
        }
    }

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder_context(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    val = Data[0] % 2;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    val = Data[0] % 4;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, val);
    (void)ret;

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    val = Data[0] % 2;
    ret = aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
    (void)ret;

    // Fuzz AV1E_SET_TILE_ROWS
    val = Data[0] % 8;
    ret = aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val);
    (void)ret;

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    val = Data[0] % 4;
    ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
    (void)ret;

    // Fuzz AV1E_SET_ROW_MT
    val = Data[0] % 2;
    ret = aom_codec_control(&ctx, AV1E_SET_ROW_MT, val);
    (void)ret;

    // Try with different values from input data if available
    if (Size > 1) {
        val = Data[1] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
        (void)ret;

        val = Data[1] % 4;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, val);
        (void)ret;

        val = Data[1] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
        (void)ret;

        val = Data[1] % 8;
        ret = aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val);
        (void)ret;

        val = Data[1] % 4;
        ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
        (void)ret;

        val = Data[1] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ROW_MT, val);
        (void)ret;
    }

    cleanup_codec(&ctx);
    return 0;
}