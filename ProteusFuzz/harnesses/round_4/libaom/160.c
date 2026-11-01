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
#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    int ret = 0;
    int32_t val;
    size_t offset = 0;

    // Helper to read int32_t safely
    #define READ_INT32(v) \
        do { \
            if (size - offset >= sizeof(int32_t)) { \
                memcpy(&(v), data + offset, sizeof(int32_t)); \
                offset += sizeof(int32_t); \
            } else { \
                (v) = 0; \
            } \
        } while (0)

    // 1. AV1E_SET_COEFF_COST_UPD_FREQ
    READ_INT32(val);
    ret = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    (void)ret;

    // 2. AOME_SET_ENABLEAUTOBWDREF
    READ_INT32(val);
    ret = aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, val);
    (void)ret;

    // 3. AV1E_SET_DELTAQ_MODE
    READ_INT32(val);
    ret = aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, val);
    (void)ret;

    // 4. AOME_SET_CQ_LEVEL
    READ_INT32(val);
    ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val);
    (void)ret;

    // 5. AV1E_SET_FORCE_VIDEO_MODE
    READ_INT32(val);
    ret = aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, val);
    (void)ret;

    // 6. AOME_SET_ENABLEAUTOALTREF
    READ_INT32(val);
    ret = aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config to ensure valid initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    // Initialize encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}