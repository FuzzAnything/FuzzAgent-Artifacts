// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static inline int32_t read_int32_safe(uint8_t **ptr, size_t *offset, size_t size) {
    if (*offset + sizeof(int32_t) > size) return 0;
    int32_t v = 0;
    memcpy(&v, *ptr + *offset, sizeof(int32_t));
    *offset += sizeof(int32_t);
    return v;
}

static inline int read_int_safe(uint8_t **ptr, size_t *offset, size_t size) {
    if (*offset + sizeof(int) > size) return 0;
    int v = 0;
    memcpy(&v, *ptr + *offset, sizeof(int));
    *offset += sizeof(int);
    return v;
}

static int fuzz_codec_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int ret = 0;
    int32_t val = 0;
    uint8_t *ptr = (uint8_t *)data;
    size_t offset = 0;

    // 1. AV1E_SET_NOISE_SENSITIVITY (int32_t)
    if (offset < size) {
        val = read_int32_safe(&ptr, &offset, size);
        aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
        ret = 1;
    }

    // 2. AV1E_ENABLE_RATE_GUIDE_DELTAQ (int)
    if (offset < size) {
        val = read_int_safe(&ptr, &offset, size);
        aom_codec_control(ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val);
        ret = 1;
    }

    // 3. AV1E_SET_AQ_MODE (int)
    if (offset < size) {
        val = read_int_safe(&ptr, &offset, size);
        aom_codec_control(ctx, AV1E_SET_AQ_MODE, val);
        ret = 1;
    }

    // 4. AV1E_SET_ENABLE_QM (int)
    if (offset < size) {
        val = read_int_safe(&ptr, &offset, size);
        aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
        ret = 1;
    }

    // 5. AV1E_SET_QM_MAX (int32_t)
    if (offset < size) {
        val = read_int32_safe(&ptr, &offset, size);
        aom_codec_control(ctx, AV1E_SET_QM_MAX, val);
        ret = 1;
    }

    // 6. AV1E_SET_DELTAQ_STRENGTH (int32_t)
    if (offset < size) {
        val = read_int32_safe(&ptr, &offset, size);
        aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val);
        ret = 1;
    }

    return ret;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder config
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_lag_in_frames = 0;
    cfg.g_pass = 0;

    // Initialize codec
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_codec_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}