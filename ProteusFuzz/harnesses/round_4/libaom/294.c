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
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/* Helper to safely extract integers from fuzz data */
static inline int32_t get_int32(const uint8_t **ptr, size_t *rem) {
    if (*rem < 4) return 0;
    int32_t val = 0;
    memcpy(&val, *ptr, 4);
    *ptr += 4;
    *rem -= 4;
    return val;
}

static inline int64_t get_int64(const uint8_t **ptr, size_t *rem) {
    if (*rem < 8) return 0;
    int64_t val = 0;
    memcpy(&val, *ptr, 8);
    *ptr += 8;
    *rem -= 8;
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || ctx->iface == NULL) {
        return 0;
    }

    const uint8_t *ptr = data;
    size_t rem = size;

    /* 1. AOME_SET_TUNING */
    if (rem > 0) {
        int32_t tuning = get_int32(&ptr, &rem);
        aom_codec_control(ctx, AOME_SET_TUNING, tuning);
    }

    /* 2. AV1E_SET_MTU */
    if (rem > 0) {
        int32_t mtu = get_int32(&ptr, &rem);
        aom_codec_control(ctx, AV1E_SET_MTU, mtu);
    }

    /* 3. AV1E_SET_QM_U */
    if (rem > 0) {
        int32_t qm_u = get_int32(&ptr, &rem);
        aom_codec_control(ctx, AV1E_SET_QM_U, qm_u);
    }

    /* 4. AV1E_SET_SUPERBLOCK_SIZE */
    if (rem > 0) {
        int32_t sb_size = get_int32(&ptr, &rem);
        aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
    }

    /* 5. AV1E_SET_ENABLE_CHROMA_DELTAQ */
    if (rem > 0) {
        int32_t enable = get_int32(&ptr, &rem);
        aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable);
    }

    /* 6. AV1E_SET_TIER_MASK */
    if (rem > 0) {
        int32_t tier_mask = get_int32(&ptr, &rem);
        aom_codec_control(ctx, AV1E_SET_TIER_MASK, tier_mask);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_294(const uint8_t *Data, size_t Size) {
    /* Ensure minimum size to avoid out-of-bounds access */
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Get AV1 Encoder Interface */
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    /* Create default encoder config */
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Fuzz config parameters based on input data with bounds checking */
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 100) + 1000;
    cfg.g_threads = (Data[4] % 16) + 1;
    cfg.g_error_resilient = (Data[5] % 3);
    cfg.g_lag_in_frames = (Data[6] % 32);
    cfg.g_pass = (Data[7] % 3);

    /* Initialize Encoder */
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Fuzz the control functions */
    fuzz_encoder_controls(&ctx, Data, Size);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}