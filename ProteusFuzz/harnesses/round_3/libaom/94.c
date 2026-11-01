// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || size < 1) return 0;

    // Extract values from input data
    int val1 = data[0];
    int val2 = (size > 1) ? data[1] : 0;
    int val3 = (size > 2) ? data[2] : 0;
    int val4 = (size > 3) ? data[3] : 0;
    int val5 = (size > 4) ? data[4] : 0;
    int val6 = (size > 5) ? data[5] : 0;

    // AV1E_SET_MAX_REFERENCE_FRAMES
    if (val1 <= 8) {
        aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val1);
    }

    // AV1E_SET_ENABLE_DIRECTIONAL_INTRA (0 or 1)
    if (val2 <= 1) {
        aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val2);
    }

    // AV1E_SET_ENABLE_SUPERRES (0 or 1)
    if (val3 <= 1) {
        aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val3);
    }

    // AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (val4 >= 0 && val4 <= 127) {
        aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val4);
    }

    // AV1E_GET_SEQ_LEVEL_IDX (output to dummy buffer)
    int level_idx = 0;
    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);
    (void)level_idx; // Suppress unused warning

    // AV1E_SET_ENABLE_FLIP_IDTX (0 or 1)
    if (val5 <= 1) {
        aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val5);
    }

    // Additional random control calls to explore states
    if (val6 <= 1) {
        aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val6);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface using the correct API
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to set some configuration parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}