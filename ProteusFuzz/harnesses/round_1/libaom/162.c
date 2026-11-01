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
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static int32_t read_int32(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 4 > size) return 0;
    return *(const int32_t *)(data + offset);
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Extract parameters from fuzz input safely
    int32_t val1 = read_int32(Data, Size, 0);
    int32_t val2 = read_int32(Data, Size, 4);
    int32_t val3 = read_int32(Data, Size, 8);
    int32_t val4 = read_int32(Data, Size, 12);
    int32_t val5 = read_int32(Data, Size, 16);
    int32_t val6 = read_int32(Data, Size, 20);

    // Initialize encoder
    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val1);

    // Fuzz AOME_SET_ARNR_STRENGTH
    aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val2);

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val3);

    // Fuzz AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, val4);

    // Fuzz AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val5);

    // Fuzz AOME_SET_ARNR_MAXFRAMES
    aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val6);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}