// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

/* Helper to initialize a valid encoder context for fuzzing */
static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_flags_t flags = 0;

    if (ctx == NULL) return -1;

    /* Initialize config with defaults */
    ret = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    /* Fuzz some config parameters based on input data if available */
    if (size > 0) {
        cfg.g_w = (data[0] % 128) * 16;
        if (cfg.g_w < 16) cfg.g_w = 16;
        if (size > 1) cfg.g_h = (data[1] % 128) * 16;
        if (cfg.g_h < 16) cfg.g_h = 16;
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
        if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 1000;
    }

    /* Get AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, flags);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_289(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    int val;

    /* Initialize context */
    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    /* Ensure we have enough data to generate diverse integer values */
    if (Size < 1) {
        val = 0;
    } else {
        val = Data[0] % 2; /* 0 or 1 for boolean-like flags */
    }

    /* 1. AV1E_SET_TIER_MASK */
    /* Assuming it takes an integer value (0 or 1) */
    ret = aom_codec_control(&ctx, AV1E_SET_TIER_MASK, val);
    (void)ret;

    /* 2. AV1E_SET_ENABLE_INTRABC */
    /* Assuming it takes an integer value (0 or 1) */
    val = (Size > 1) ? (Data[1] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)ret;

    /* 3. AV1E_SET_RTC_EXTERNAL_RC */
    /* Assuming it takes an integer value (0 or 1) */
    val = (Size > 2) ? (Data[2] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val);
    (void)ret;

    /* 4. AV1E_SET_ENABLE_CHROMA_DELTAQ */
    /* Assuming it takes an integer value (0 or 1) */
    val = (Size > 3) ? (Data[3] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);
    (void)ret;

    /* 5. AV1E_SET_SUPERBLOCK_SIZE */
    /* Assuming it takes an integer value representing size */
    val = (Size > 4) ? (Data[4] % 3) : 0; /* 0, 1, or 2 */
    ret = aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, val);
    (void)ret;

    /* 6. AV1E_SET_MTU */
    /* Assuming it takes an integer value for MTU */
    val = (Size > 5) ? (Data[5] % 100) + 100 : 100;
    ret = aom_codec_control(&ctx, AV1E_SET_MTU, val);
    (void)ret;

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}