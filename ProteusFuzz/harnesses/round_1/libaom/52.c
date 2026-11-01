// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static int setup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (aom_codec_enc_config_default("av1", cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 1;
    cfg->rc_target_bitrate = 1000;

    aom_codec_err_t res = aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!setup_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Extract parameters from fuzz input
    uint32_t val1 = (Size > 0) ? (Data[0] & 0xFF) : 0;
    uint32_t val2 = (Size > 1) ? (Data[1] & 0xFF) : 0;
    int32_t val3 = (Size > 2) ? ((int32_t)(Data[2] << 24) >> 24) : 0; // sign extend byte

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val1);

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val2);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val1);

    // Fuzz AV1E_SET_S_FRAME_MODE
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val2);

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val1 & 0x7); // Limit to 0-7

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val2 & 0x1F); // Limit to 0-31

    cleanup_encoder(&ctx);
    return 0;
}