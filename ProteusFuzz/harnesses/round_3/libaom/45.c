// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static int get_int(const uint8_t **d, size_t *s) {
    if (*s < sizeof(int)) return 0;
    int val;
    memcpy(&val, *d, sizeof(int));
    *d += sizeof(int);
    *s -= sizeof(int);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    const uint8_t *ptr = data;
    size_t remaining = size;

    // 1. AV1E_SET_ENABLE_CFL_INTRA
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    }

    // 2. AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
    }

    // 3. AV1E_SET_ERROR_RESILIENT_MODE
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, val);
    }

    // 4. AV1E_SET_ENABLE_DIAGONAL_INTRA
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
    }

    // 5. AV1E_GET_SEQ_LEVEL_IDX
    int level_idx;
    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);
    (void)level_idx; // Use variable to avoid unused warning

    // 6. AV1E_SET_ENABLE_PAETH_INTRA
    if (remaining >= sizeof(int)) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    
    // Get the AV1 encoder interface using the standard function
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz some config parameters safely
    if (Size > 0) cfg.g_w = (Data[0] % 4096) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 4096) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 1000) + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic on encoder controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}