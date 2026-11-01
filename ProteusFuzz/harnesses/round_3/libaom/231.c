// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int get_int(const uint8_t **d, size_t *s) {
    if (*s < sizeof(int)) return 0;
    int val;
    memcpy(&val, *d, sizeof(int));
    *d += sizeof(int);
    *s -= sizeof(int);
    return val;
}

static int get_uint32(const uint8_t **d, size_t *s) {
    if (*s < sizeof(uint32_t)) return 0;
    uint32_t val;
    memcpy(&val, *d, sizeof(uint32_t));
    *d += sizeof(uint32_t);
    *s -= sizeof(uint32_t);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    const uint8_t *ptr = data;
    size_t remaining = size;

    // 1. aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS
    // Expected: aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, int)
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_TUNE_CONTENT
    // Expected: aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, int)
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, val);
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
    // Expected: aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, int)
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP
    // Expected: aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, int)
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_QUANT_B_ADAPT
    // Expected: aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, int)
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, val);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY
    // Expected: aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, int)
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config from fuzz data to vary states
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1 % Size] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2 % Size] % 1000) + 100;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzz logic
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}