// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 1;
}

static int fuzz_encoder_control_uint(aom_codec_ctx_t *ctx, int control_id, uint32_t value) {
    if (!ctx) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    uint32_t seed = *(const uint32_t *)Data;
    Data += 4;
    Size -= 4;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Configure based on seed to explore different states
    cfg.g_w = (seed % 512) + 16;
    cfg.g_h = ((seed >> 8) % 512) + 16;
    cfg.g_timebase.num = (seed >> 16) % 100 + 1;
    cfg.g_timebase.den = ((seed >> 24) % 1000) + 1;
    cfg.g_threads = (seed % 4) + 1;
    cfg.g_error_resilient = (seed & 0x100) ? 1 : 0;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control functions with diverse values
    int32_t val_i = (int32_t)(seed & 0x7FFFFFFF) - (1 << 30);
    uint32_t val_u = seed & 0xFFFFFFFF;

    // AV1E_SET_GF_MAX_PYRAMID_HEIGHT (typically takes int)
    fuzz_encoder_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val_i);

    // AV1E_SET_ERROR_RESILIENT_MODE (typically takes int)
    fuzz_encoder_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val_i);

    // AV1E_SET_GF_MIN_PYRAMID_HEIGHT (typically takes int)
    fuzz_encoder_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val_i);

    // AV1E_SET_MIN_GF_INTERVAL (typically takes int)
    fuzz_encoder_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val_i);

    // AV1E_SET_NUM_TG (typically takes int)
    fuzz_encoder_control(&ctx, AV1E_SET_NUM_TG, val_i);

    // Try with uint values as well for robustness
    fuzz_encoder_control_uint(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val_u);
    fuzz_encoder_control_uint(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val_u);
    fuzz_encoder_control_uint(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val_u);
    fuzz_encoder_control_uint(&ctx, AV1E_SET_MIN_GF_INTERVAL, val_u);
    fuzz_encoder_control_uint(&ctx, AV1E_SET_NUM_TG, val_u);

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        // Free the buffer as per documentation
        free(headers->buf);
        free(headers);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}