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
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) {
        return 0;
    }

    // Initialize basic fields to avoid immediate crashes
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    ctx->name = "aom_av1";
    ctx->iface = aom_codec_av1_cx();
    if (ctx->iface == NULL) {
        free(ctx);
        return 0;
    }

    // Initialize encoder config
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(ctx->iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }
    cfg.g_w = Data[0] ? Data[0] : 64;
    cfg.g_h = Data[1] ? Data[1] : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;

    // Initialize the codec context
    if (aom_codec_enc_init(ctx, ctx->iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    // Use fuzzing data to drive control parameters
    int val;
    int idx = 2; // Skip first 2 bytes used for width/height

    // Helper to safely extract int from data
    #define GET_INT() ((idx < Size) ? (int)Data[idx++] : 0)
    #define GET_INT_RANGE(max) ((idx < Size) ? (int)(Data[idx++] % ((max) + 1)) : 0)

    // 1. AV1E_SET_ENABLE_SUPERRES
    val = GET_INT_RANGE(1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val);

    // 2. AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    val = GET_INT_RANGE(1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);

    // 3. AV1E_SET_ENABLE_OVERLAY
    val = GET_INT_RANGE(1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, val);

    // 4. AV1E_GET_SEQ_LEVEL_IDX
    int level_idx;
    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);

    // 5. AV1E_SET_ENABLE_DNL_DENOISING
    val = GET_INT_RANGE(1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);

    // 6. AV1E_SET_ENABLE_CHROMA_DELTAQ
    val = GET_INT_RANGE(1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}